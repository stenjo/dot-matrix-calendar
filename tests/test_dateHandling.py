from modules.datetime import datetime

def test_strftime():
    startTime = datetime.strptime("2023-10-22T10:12", "%Y-%m-%dT%H:%M")
    dt = datetime.strftime(startTime, "%d/%m")
    assert dt == "22/10"
    
