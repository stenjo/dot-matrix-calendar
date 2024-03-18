import sys
sys.path.append('./modules')
sys.path.append('./modules/ics')
import unittest


from ics.icalendar import Calendar

class TestParseIcsFile(unittest.TestCase):
    def test_ReadLocalFile(self):
        c = Calendar(open('tests/test-hendelse.ics').read())
        
        c
        
        print(c.events)
        
        # e = list(c.timeline)[0]
        # "Event '{}' started {}".format(e.name, e.begin.humanize())