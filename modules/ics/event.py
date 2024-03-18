from datetime import datetime, timedelta, timezone
from uuid import uuid4

UTC = timezone.utc

STATUS_VALUES = (None, "TENTATIVE", "CONFIRMED", "CANCELLED")

def uid_gen():
    return str(uuid4())

def now_in_utc():
    return datetime.now(UTC)


class CalendarEntryAttrs:
    _TIMESPAN_TYPE = None  # Should be overridden by subclass

    def __init__(self, timespan, summary=None, uid=None, description=None,
                 location=None, url=None, status=None, created=None,
                 last_modified=None, dtstamp=None, alarms=None, attach=None, *args, **kwargs):
        
        if uid is None:
            uid = uid_gen()
        if dtstamp is None:
            dtstamp = now_in_utc()

        self.timespan = timespan
        self.summary = summary
        self.uid = uid
        self.description = description
        self.location = location
        self.url = url
        self.status = status and status.upper()
        self.created = created and ensure_utc(created)
        self.last_modified = last_modified and ensure_utc(last_modified)
        self.dtstamp = dtstamp and ensure_utc(dtstamp)
        self.alarms = alarms if alarms is not None else []
        self.attach = attach if attach is not None else []
        # Initialize additional attributes from args or kwargs as needed
        
        self.validate_timespan(self.timespan)

    def validate_timespan(self, value):
        if not isinstance(value, self._TIMESPAN_TYPE):
            raise ValueError("timespan must be an instance of {}".format(self._TIMESPAN_TYPE))
        # Add additional validation logic if necessary

    def add_attendee(self, attendee):
        if not isinstance(attendee, Attendee):
            raise ValueError("attendee must be an instance of Attendee")
        self.attendees.append(attendee)


class Event(CalendarEntryAttrs):
    NAME = "VEVENT"

    def __init__(self, summary=None, begin=None, end=None, duration=None, *args, **kwargs):
        super().__init__(kwargs.get("timespan"), summary, *args, **kwargs)
        # If necessary, replace or remove any references to methods, classes, etc.,
        # that are not available or compatible with MicroPython.
        
        # Initialize timespan using begin, end, and duration if provided
        # ...

    # Replace or remove any other methods that are not compatible with MicroPython

# Replace Organizer, Attendee, BaseAlarm, Geo, and their associated methods
# with MicroPython-compatible versions.

# Note: This code leaves out several parts of the original implementation for simplicity.
# Depending on the complexity and requirements of the target MicroPython environment,
# you may need to implement additional features or simplifications.