from ics_parser import ICS

def convert_dtstart_iso(dtstart):
    # Assuming the format of dtstart is '20230412T165722Z'
    # We remove the 'Z' as it indicates UTC and 'fromisoformat' does not support 'Z'
    dtstart = dtstart.rstrip('Z')
    date_part = dtstart[:8]
    time_part = dtstart[9:]
    
    # Insert hyphens and colons to match ISO 8601 format
    date_iso = "{}-{}-{}".format(date_part[:4], date_part[4:6], date_part[6:])
    time_iso = "{}:{}:{}".format(time_part[:2], time_part[2:4], time_part[4:])

    return "{}T{}".format(date_iso, time_iso)


def toDict(event_tuple):
    if not event_tuple:
        return None
    
    return {
        "start": {"date": convert_dtstart_iso(event_tuple[1])},
        "summary": event_tuple[0]
    }

class Calendar(ICS):
    
    def __init__(self, filename=None, start=None, end=None):
        super().__init__()
        self.reset()
        if start is not None:
            self.setStartDate(start)
            
        if end is not None:
            self.setEndDate(end)
            
        if filename is not None:
            self.parseFile(filename)
    
    def parseFile(self, filename):
        f=open(filename)
        count = self.parse(f.read())
        f.close()
        return count
    
    def first(self):
        return toDict(self.getFirst())
        
    def next(self):
        return toDict(self.getNext())
    
    def start(self, startDate):
        self.setStartDate(startDate)
        
    def end(self, endDate):
        self.setEndDate(endDate)
        
# c.setStartDate('20240501T000000Z')