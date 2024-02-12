
import unittest
from unittest.mock import patch
import sys
# from datetime_ext import *
import _datetime

sys.modules['datetime'] = _datetime

# def timetuple():
#     return (2024,3,4,5,6,0)
# Your test cases for main_module.py
class TestDateTime(unittest.TestCase):
    # @patch("datetime_ext.datetime")
    def test_strftime(self):
        # with patch('datetime.datetime', _datetime.datetime):
        from datetime_ext import datetime, timedelta, date
        # mock_datetime.timetuple = timetuple
        assert datetime(2023, 12, 31).strftime("%Y-%m-%d") == "2023-12-31"
    
    
if __name__ == '__main__':
    # Run the tests
    unittest.main()