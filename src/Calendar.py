from ics_parser import ICS
import urequests

def dtStrToIso(dtstart):
    # Assuming the format of dtstart is '20230412T165722Z'
    # We remove the 'Z' as it indicates UTC and 'fromisoformat' does not support 'Z'
    dtstart = dtstart.rstrip('Z')
    date_part = dtstart[:8]
    time_part = dtstart[9:]
    
    # Insert hyphens and colons to match ISO 8601 format
    date_iso = "{}-{}-{}".format(date_part[:4], date_part[4:6], date_part[6:])
    time_iso = "{}:{}:{}".format(time_part[:2], time_part[2:4], time_part[4:])

    return "{}T{}".format(date_iso, time_iso)


def toDtStr(date_input):
    if isinstance(date_input, str):
        # Assume the string is already in the correct format.
        return date_input
    
    elif isinstance(date_input, tuple) and len(date_input) >= 6:
        # Format the time tuple to the datetime string.
        return "{:04d}{:02d}{:02d}T{:02d}{:02d}{:02d}Z".format(
            date_input[0], date_input[1], date_input[2],
            date_input[3], date_input[4], date_input[5]) 
   
    else:
        raise ValueError("Invalid date input type. Must be datetime string or time tuple.")

def toDict(event_tuple):
    if not event_tuple:
        return None
    
    return {
        "start": {"date": dtStrToIso(event_tuple[1])},
        "summary": event_tuple[0]
    }

class Calendar(ICS):
    
    def __init__(self, filename=None, start=None, end=None):
        super().__init__()
        self.reset()
        if start is not None:
            self.start(start)
            
        if end is not None:
            self.end(end)
            
        if filename is not None:
            self.parseFile(filename)
    
    def parseFile(self, filename):
        f=open(filename)
        count = self.parse(f.read())
        f.close()
        return count

    def parseURL(self, url):
        url = url.replace('webcal://', 'http://')
        response = urequests.get(url)
        if response.status_code == 200:
            # Successfully fetched the calendar data, now parse it.
            count = self.parse(response.text)
            response.close()
            return count
        else:
            response.close()
            raise Exception(f"Failed to fetch calendar data, status code: {response.status_code}")

    
    def first(self):
        return toDict(self.getFirst())
        
    def next(self):
        return toDict(self.getNext())
    
    def start(self, startDate):
        self.setStartDate(toDtStr(startDate))
        
    def end(self, endDate):
        self.setEndDate(toDtStr(endDate))
        
# c.setStartDate('20240501T000000Z')