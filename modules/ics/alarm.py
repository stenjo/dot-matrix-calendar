
from ics.component import Component
from ics.utils import ensure_timedelta

__all__ = [
    "BaseAlarm",
    "AudioAlarm",
    "CustomAlarm",
    "DisplayAlarm",
    "EmailAlarm",
    "NoneAlarm",
    "get_type_from_action",
]

class BaseAlarm(Component):
    """
    A calendar event VALARM base class
    """

    NAME = "VALARM"

    def __init__(self, trigger=None, repeat=None, duration=None):
        self.trigger = trigger
        self.repeat = repeat
        self.duration = ensure_timedelta(duration) if duration is not None else None

    @property
    def action(self):
        raise NotImplementedError("Subclasses must implement this method")


class AudioAlarm(BaseAlarm):
    """
    A calendar event VALARM with AUDIO option.
    """

    def __init__(self, trigger=None, repeat=None, duration=None, attach=None):
        super().__init__(trigger, repeat, duration)
        self.attach = attach

    @property
    def action(self):
        return "AUDIO"


class CustomAlarm(BaseAlarm):
    """
    A calendar event VALARM with custom ACTION.
    """

    def __init__(self, trigger=None, repeat=None, duration=None, action=None):
        super().__init__(trigger, repeat, duration)
        self._action = action

    @property
    def action(self):
        return self._action


class DisplayAlarm(BaseAlarm):
    """
    A calendar event VALARM with DISPLAY option.
    """

    def __init__(self, trigger=None, repeat=None, duration=None, description=None):
        super().__init__(trigger, repeat, duration)
        self.description = description

    @property
    def action(self):
        return "DISPLAY"


class EmailAlarm(BaseAlarm):
    """
    A calendar event VALARM with Email option.
    """

    def __init__(self, trigger=None, repeat=None, duration=None, summary=None,
                 description=None, attendees=None, attach=None):
        super().__init__(trigger, repeat, duration)
        self.summary = summary  # message subject
        self.description = description  # message body
        self.attendees = attendees if attendees else []  # message recipients
        self.attach = attach if attach else []  # e-mail attachments

    def add_attendee(self, attendee):
        # Assuming Attendee is defined somewhere
        self.attendees.append(attendee)

    @property
    def action(self):
        return "EMAIL"


class NoneAlarm(BaseAlarm):
    """
    A calendar event VALARM with NONE option.
    """

    @property
    def action(self):
        return "NONE"


def get_type_from_action(action_type):
    mapping = {
        "DISPLAY": DisplayAlarm,
        "AUDIO": AudioAlarm,
        "NONE": NoneAlarm,
        "EMAIL": EmailAlarm,
    }
    return mapping.get(action_type, CustomAlarm)