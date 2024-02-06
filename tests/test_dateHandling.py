from modules.dateHandling import isNowInTimePeriod, dayText
from modules.DateTimeExt import DateTimeExt as datetime
from modules.datetime import timezone, timedelta


def test_isNowInTimePeriod():
    startTime = datetime(2023,10,22,10,12)
    endTime = datetime(2023,10,25,10,12)
    today = datetime(2023,10,23,10,12)
    before = datetime(2023,10,22,10,11)
    after = datetime(2023,10,25,10,13)
    
    assert isNowInTimePeriod(startTime, endTime, today) == True
    assert isNowInTimePeriod(startTime, endTime, before) == False
    assert isNowInTimePeriod(startTime, endTime, after) == False


def test_dayText_tomorrow():
    event = {
        'start': {
            'date': '2023-10-24'
        },
        'summary': 'Event summary'
    }
    today = datetime(2023,10,23,10,12).replace(tzinfo=timezone.utc)
    
    assert dayText(event, today) == "I morgen: Event summary"

def test_dayText_today():
    event = {
        'start': {
            'date': '2023-10-23'
        },
        'summary': 'Event summary'
    }
    today = datetime(2023,10,23,10,12)
    today = today.replace(tzinfo=timezone.utc)
    
    assert dayText(event, today) == "I dag: Event summary"
    
def test_dayText_today_with_time():
    event = {
        'start': {
            'date': '2023-10-23',
            'dateTime': '2023-10-23T10:22:18'
        },
        'summary': 'Event summary'
    }
    today = datetime(2023,10,23,10,12).replace(tzinfo=timezone.utc)
    
    assert dayText(event, today) == "I dag: Event summary kl. 10:22"
    
def test_dayText_friday():
    event = {
        'start': {
            'date': '2023-10-27'
        },
        'summary': 'Event summary'
    }
    today = datetime(2023,10,23,10,12).replace(tzinfo=timezone.utc)
    
    assert dayText(event, today) == "Fredag: Event summary"
    
def test_dayText_friday_plus_one_week():
    event = {
        'start': {
            'date': '2023-11-03'
        },
        'summary': 'Event summary'
    }
    today = datetime(2023,10,23,10,12).replace(tzinfo=timezone.utc)
    
    assert dayText(event, today) == "Fredag 03/11: Event summary"

def test_dayText_today_with_time_and_timezone():
    event = {
        'start': {
            'date': '2023-10-23',
            'dateTime': '2023-10-23T10:22:18+01:00'
        },
        'summary': 'Event summary'
    }
    today = datetime(2023,10,23,10,12,tzinfo=timezone(timedelta(hours=1)))
    # print(today)
    # dt:  2023-11-09 13:15:00+01:00
    # today:  2023-11-04 18:54:37.512734

    assert dayText(event, today) == "I dag: Event summary kl. 10:22"
    
def test_dayText_today_with_datetime_and_timezone():
    event = {
        'start': {
            'date': datetime.today().strftime('%Y-%m-%d'),
            'dateTime': datetime.today().strftime('%Y-%m-%d') + 'T10:22:18+01:00'
        },
        'summary': 'Event summary'
    }

    assert dayText(event) == "I dag: Event summary kl. 10:22"

def test_dayText_today_with_date_and_no_timezone():
    event = {
        'start': {
            'date': datetime.today().strftime('%Y-%m-%d'),
        },
        'summary': 'Event summary'
    }

    assert dayText(event) == "I dag: Event summary"