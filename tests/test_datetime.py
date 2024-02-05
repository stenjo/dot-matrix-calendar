from modules.datetime import datetime, timedelta, timezone
import datetime as _datetime

def test_strftime():
    startTime = datetime(2023, 10, 22)
    dt = datetime.strftime(startTime, "%d/%m")
    assert dt == "22/10"
    
def test_now():
    assert datetime.now(timezone.utc).year == _datetime.datetime.now(_datetime.timezone.utc).year
    
def test_timedelta():
    today = datetime(2023,10,23)
    tomorrow = datetime(2023,10,24)
    assert today + timedelta(1) == tomorrow
