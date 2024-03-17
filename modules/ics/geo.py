

class Geo():
    latitude = None
    longitude = None
    
    def __init__(self, latitude, longitude):
        self.latitude = latitude
        self.longitude = longitude

def make_geo(value):
    if value is None:
        return None
    elif isinstance(value, dict):
        return Geo(latitude=value.get('latitude'), longitude=value.get('longitude'))
    elif isinstance(value, tuple):
        return Geo(latitude=value[0], longitude=value[1])
    else:
        raise ValueError("Invalid type for geo value")