import unittest, time
from Calendar import Calendar, dtStrToIso, toDtStr, toDict
from dateHandling import dayText

class TestCalendar(unittest.TestCase):

    def test_dtStrToIso(self):
        self.assertEqual(dtStrToIso('20230412T165722Z'), '2023-04-12T16:57:22')
        self.assertEqual(dtStrToIso('20230412'), '2023-04-12')
        self.assertEqual(dtStrToIso('20230412T165722'), '2023-04-12T16:57:22')

    def test_toDtStr(self):
        self.assertEqual(toDtStr('20230412T165722Z'), '20230412T165722Z')
        self.assertEqual(toDtStr((2023, 4, 12, 16, 57, 22)), '20230412T165722Z')
        self.assertIsNone(toDtStr(None))
        with self.assertRaises(ValueError):
            toDtStr('invalid')

    def test_toDict(self):
        event_tuple = ('Event Summary', '20230412T165722Z')
        self.assertEqual(toDict(event_tuple), {'start': {'dateTime': '2023-04-12T16:57:22'}, 'summary': 'Event Summary'})
        event_tuple = ('Event Summary', '20230412')
        self.assertEqual(toDict(event_tuple), {'start': {'date': '2023-04-12'}, 'summary': 'Event Summary'})
        self.assertIsNone(toDict(None))

    def test_parseFile(self):
        calendar = Calendar()
        # Assuming you have a sample .ics file for testing purposes
        count = calendar.parseFile('/Users/sten.johnsen/git/dot-matrix-calendar/tests/test-hendelse.ics')
        self.assertEqual(count, 1)

    def test_parseFile_large(self):
        calendar = Calendar()
        # Assuming you have a sample .ics file for testing purposes
        # calendar.end((2024,8,3,0,0,0,0,0,0))
        count = calendar.parseFile('/Users/sten.johnsen/git/dot-matrix-calendar/tests/moon.ics')
        self.assertEqual(count, 148)

    def test_parseURL(self):
        calendar = Calendar()
        url = 'http://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics'
        # Mock the mrequests.get to return a dummy response
        calendar._parse = lambda url: 1
        count = calendar.parseURL(url)
        self.assertEqual(count, 56)
        self.assertIn(url, calendar.sources)

    def test_parseURL_chunks(self):
        calendar = Calendar()
        calendar.start((2024,7,3,0,0,0,0,0,0))
        calendar.end((2024,8,3,0,0,0,0,0,0))
        calendar.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
        count = calendar.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
        self.assertEqual(count, 24)
        event = calendar.first()
        self.assertEqual(event['summary'], "F3: Practice (British)")
        event=calendar.next()
        print(event)
        self.assertEqual(event['summary'], "F2: Practice (British)")
        calendar.refresh()
        event = calendar.first()
        self.assertEqual(event['summary'], "F3: Practice (Bahrain)")
        print(dayText(event))
        

    def test_parseURL_large_chunks(self):
        calendar = Calendar()
        calendar.start((2025,5,3,0,0,0,0,0,0))
        # calendar.end((2025,8,3,0,0,0,0,0,0))
        url = 'https://calendar.google.com/calendar/ical/ht3jlfaac5lfd6263ulfh4tql8%40group.calendar.google.com/public/basic.ics'
        # Mock the mrequests.get to return a dummy response
        count = calendar.parseURL(url)
        self.assertEqual(count, 33)
        self.assertIn(url, calendar.sources)
        
    def test_parseURL_Home_Cal(self):
        
        calendar = Calendar()
        calendar.start((2024,7,2,0,0,0,0,0,0))
        calendar.end((2024,8,3,0,0,0,0,0,0))
        # calendar.parseURL('https://calendar.google.com/calendar/ical/c_eae215482ecd0bf862ff838cb81657e12281bff2f104c0986f78b20d90e4917c%40group.calendar.google.com/private-5cf4e55067a529ddd245d8a2f15a5e49/basic.ics')
        calendar.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
        count = calendar.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
        self.assertEqual(count, 24)
        

    def test_refresh(self):
        calendar = Calendar()
        url = 'webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics'
        calendar.sources.append(url)
        calendar._parse = lambda url: 1
        items = calendar.refresh()
        self.assertEqual(items, 1)

    def test_first(self):
        calendar = Calendar()
        calendar.getFirst = lambda: ('Event Summary', '20230412T165722Z')
        first_event = calendar.first()
        self.assertEqual(first_event, {'start': {'dateTime': '2023-04-12T16:57:22'}, 'summary': 'Event Summary'})

    def test_next(self):
        calendar = Calendar()
        calendar.getNext = lambda: ('Event Summary', '20230412T165722Z')
        next_event = calendar.next()
        self.assertEqual(next_event, {'start': {'dateTime': '2023-04-12T16:57:22'}, 'summary': 'Event Summary'})

    def test_start(self):
        calendar = Calendar()
        calendar.setStartDate = lambda x: self.assertEqual(x, '20230412')
        calendar.start('20230412')

    def test_end(self):
        calendar = Calendar()
        calendar.setEndDate = lambda x: self.assertEqual(x, '20230412')
        calendar.end('20230412')

if __name__ == "__main__":
    unittest.main()
