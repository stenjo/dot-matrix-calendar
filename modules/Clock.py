import time
from datetime import datetime, timezone
class Clock:
    
    lastSec = None
    interval = None
    
    def __init__(self, interval=None, date=None):
        self.interval = interval
        self.date = date
        self._test_time = None  # For testing purposes

    def set_test_time(self, test_time):
        self._test_time = test_time

    def display(self, matrix):
        current_time = self._test_time if self._test_time else time.localtime()
        (year, month, day, hour, min, sec, _, _) = current_time
        
        if sec % self.interval == 0:
            self.lastSec = sec
            return True
        dtStr = ""
        if sec is not self.lastSec:
            if self.date == True:
                dtStr = "{:02}/{:02} ".format(day, month)
            
            if sec % 2 == 0:
                matrix.write("{}{:02}:{:02}".format(dtStr, hour, min), True)
            else:
                matrix.write("{}{:02} {:02}".format(dtStr, hour, min), True)
        self.lastSec = sec
        return False
