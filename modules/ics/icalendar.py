from ics.component import Component


class Calendar(Component):
    """
    Represents a unique RFC 5545 iCalendar.
    
    Attributes:
    
        events: a list of `Event` contained in the Calendar
        todos: a list of `Todo` contained in the Calendar
    """
    
    NAME = "VCALENDAR"
    DEFAULT_VERSION = "2.0"
    DEFAULT_PRODID = "ics.py - https://github.com/ics-py/ics-py"
    
    def __init__(self, version=None, prodid=None, scale=None, method=None, events=None, todos=None, *args, **kwargs):
        self.version = version or self.DEFAULT_VERSION
        self.prodid = prodid or self.DEFAULT_PRODID
        self.scale = scale
        self.method = method
        self.events = events if events is not None else []
        self.todos = todos if todos is not None else []
        
        for key, val in kwargs.items():
            setattr(self, key, val)
        
        # Additional initialization tasks.
    
    @classmethod
    def parse_multiple(cls, string):
        """
        Parses an input string that may contain multiple calendars
        and returns a list of `Calendar` instances.
        """
        # Implement parsing logic to extract multiple calendars from the string.
        # This will depend on the implementation of string_to_containers.
        containers = string_to_containers(string)
        return [cls(imports=c) for c in containers]
    
    # Omit the 'normalize' method unless it is needed and a compatible 
    # version can be created without using the Normalization class.
    
    def __str__(self):
        return "<Calendar with {} event{} and {} todo{}>".format(
            len(self.events),
            "" if len(self.events) == 1 else "s",
            len(self.todos),
            "" if len(self.todos) == 1 else "s",
        )
    
    def __iter__(self):
        """
        Allows iteration over serialized Calendar lines.
        This can be used to write the Calendar to a file, etc.
        """
        return iter(self.serialize().splitlines(keepends=True))


# Note: The Component, Event, and Todo will need to be converted to MicroPython-compatible versions.
# Also, Normalization and the corresponding functions should be either removed or adapted.

# Usage example:
# cal = Calendar(events=[Event()], todos=[Todo()])
# print(cal)