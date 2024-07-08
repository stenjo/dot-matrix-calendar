from ics_parser import ICS
import mrequests, re

def dtStrToIso(dtstart):
    # Assuming the format of dtstart is '20230412T165722Z'
    # We remove the 'Z' as it indicates UTC and 'fromisoformat' does not support 'Z'
    dtstart = dtstart.rstrip('Z')
    time_iso = None
    date_part = dtstart[:8]
    # Insert hyphens and colons to match ISO 8601 format
    date_iso = "{}-{}-{}".format(date_part[:4], date_part[4:6], date_part[6:])
    if len(dtstart) > 10:
        time_part = dtstart[9:]
        time_iso = "{}:{}:{}".format(time_part[:2], time_part[2:4], time_part[4:])
        return "{}T{}".format(date_iso, time_iso)
    else:
        return date_iso

def toDtStr(date_input):
    if date_input is None:
        return None
    
    if isinstance(date_input, str):
        # Assume the string is already in the correct format.
        pattern = re.compile("^([0-9]+T*[0-9]*)Z*$")        
        if pattern.match(date_input):
            return date_input
    
    if isinstance(date_input, tuple) and len(date_input) >= 6:
        # Format the time tuple to the datetime string.
        return "{:04d}{:02d}{:02d}T{:02d}{:02d}{:02d}Z".format(
            date_input[0], date_input[1], date_input[2],
            date_input[3], date_input[4], date_input[5]) 
   
    raise ValueError("Invalid date input type. Must be datetime string or time tuple.")

def toDict(event_tuple):
    # print(event_tuple)
    if not event_tuple:
        return None
    
    dt = dtStrToIso(event_tuple[1])
    if len(dt) > 10:
        return {
            "start": {"dateTime": dt},
            "summary": event_tuple[0]
        }
    else:
        return {
            "start": {"date": dt},
            "summary": event_tuple[0]
        }


class ResponseWithProgress(mrequests.Response):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._total_read = 0

    def readinto(self, buf, size=0):
        bytes_read = super().readinto(buf, size)
        self._total_read += bytes_read
        print("Progress: {:.2f}%".format(self._total_read / (self._content_size * 0.01)))
        return bytes_read

class Calendar(ICS):
    def __init__(self, filename=None, start=None, end=None):
        super().__init__()
        self.reset()
        self.sources = []
        if start is not None:
            self.start(start)
            
        if end is not None:
            self.end(end)
            
        if filename is not None:
            self.parseFile(filename)
    
    def parseFile(self, filename):
        print(f"Parsing file: {filename}")
        with open(filename) as f:
            count = self.parseIcs(f.read())
        print(f"Parsed {count} items from file")
        return count

    def parseURL(self, url):
        print(f"Parsing URL: {url}")
        url = url.replace('webcal://', 'http://')
        if url not in self.sources:
            self.sources.append(url)
        return self._parseChunks(url)

    def _parse(self, url):
        print(f"Fetching URL: {url}")
        response = mrequests.get(url)
        if response.status_code == 200:
            print("Fetched calendar data successfully")
            count = self.parseIcs(response.text)
            response.close()
            print(f"Parsed {count} items from URL")
            return count
        else:
            response.close()
            raise Exception(f"Failed to fetch calendar data, status code: {response.status_code}")
        
    def _parseChunks(self, url, chunkSize=1024):
        print(f"Fetching URL in chunks: {url}")
        response = mrequests.get(url, headers={b"accept": b"text/html"}, response_class=ResponseWithProgress)
        if response.status_code == 200:
            count = 0
            while True:
                chunk = response.read(chunkSize)
                if not chunk:
                    break
                count = self.parseIcs(chunk.decode('utf-8'))
            response.close()
            print(f"Parsed {count} items from URL in chunks")
            return count
        else:
            response.close()
            raise Exception(f"Failed to fetch calendar data in chunks, status code: {response.status_code}")

    def refresh(self, start_date=None, end_date=None):
        print("Refreshing calendar")
        self.reset()
        print("reset")
        items = 0
        if start_date is not None:
            self.start(start_date)
            
        if end_date is not None:
            self.end(end_date)

        for url in self.sources:
            try:
                items = self._parseChunks(url)
            except Exception as e:
                print(f"Error parsing url: {url}, {e}")
        print(f"Refreshed {items} items")
        return items
    
    def first(self):
        return toDict(self.getFirst())
        
    def next(self):
        return toDict(self.getNext())
    
    def start(self, startDate):
        self.setStartDate(toDtStr(startDate))
        
    def end(self, endDate):
        self.setEndDate(toDtStr(endDate))
