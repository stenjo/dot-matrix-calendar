import unittest
from Calendar import Calendar, dtStrToIso, toDtStr, toDict

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
        count = calendar.parseFile('../../test/test-hendelse.ics')
        self.assertGreater(count, 0)

    def test_parseURL(self):
        calendar = Calendar()
        url = 'webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics'
        # Mock the mrequests.get to return a dummy response
        calendar._parse = lambda url: 1
        count = calendar.parseURL(url)
        self.assertEqual(count, 1)
        self.assertIn(url, calendar.sources)

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
        calendar.setStartDate = lambda x: self.assertEqual(x, '20230412T000000Z')
        calendar.start('20230412')

    def test_end(self):
        calendar = Calendar()
        calendar.setEndDate = lambda x: self.assertEqual(x, '20230412T000000Z')
        calendar.end('20230412')

if __name__ == "__main__":
    unittest.main()
