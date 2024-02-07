from modules.DateTimeExt import timedelta, datetime
from datetime import timezone
import datetime as _datetime

def test_strftime():
    ts = datetime(2023, 10, 22, 9, 18, 59)
    assert ts.strftime("%d/%m") == "22/10"
    assert ts.strftime(' kl. %H:%M') == " kl. 09:18"
    assert ts.strftime('%Y-%y') == "2023-23"
    assert ts.strftime("%d/%m/%Y, %H:%M:%S") == "22/10/2023, 09:18:59"
    
def test_now():
    assert datetime.now(timezone.utc).year == _datetime.datetime.now(_datetime.timezone.utc).year
    
def test_timedelta():
    today = datetime(2023,10,23)
    tomorrow = datetime(2023,10,24)
    assert today + timedelta(1) == tomorrow

def test_timezone():
    _today = _datetime.datetime(2023,10,23,10,12).replace(tzinfo=_datetime.timezone.utc)
    today = datetime(2023,10,23,10,12).replace(tzinfo=timezone.utc)
    assert today.hour == _today.hour