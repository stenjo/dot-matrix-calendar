class ValueConverter:  # Simplified ValueConverter base class
    @staticmethod
    def serialize(value):
        return str(value)

    @staticmethod
    def parse(value):
        return value


class RawTextConverter(ValueConverter):
    pass


class BooleanConverter(ValueConverter):
    @staticmethod
    def parse(value):
        return value.lower() in ['true', 't', 'yes', 'y', '1']

    @staticmethod
    def serialize(value):
        return 'TRUE' if value else 'FALSE'


class URIConverter(ValueConverter):
    pass


class PersonProperty:
    def __init__(self, name, converter=RawTextConverter, default=None):
        self.name = name
        self.converter = converter
        self.default = default

    def __get__(self, instance, owner):
        if self.name not in instance.extra:
            return self.default
        value = instance.extra[self.name]
        if len(value) == 0:
            return self.default
        elif len(value) == 1:
            return self.converter.parse(value[0])
        else:
            raise ValueError("Expected at most one value for property {}, got {!r}!".format(self.name, value))

    def __set__(self, instance, value):
        instance.extra[self.name] = [self.converter.serialize(value)]

    def __delete__(self, instance):
        instance.extra.pop(self.name, None)


class PersonMultiProperty:
    def __init__(self, name, converter=RawTextConverter, default=None):
        self.name = name
        self.converter = converter
        self.default = default

    def __get__(self, instance, owner):
        if self.name not in instance.extra:
            return self.default
        return [self.converter.parse(v) for v in instance.extra[self.name]]

    def __set__(self, instance, value):
        instance.extra[self.name] = [self.converter.serialize(v) for v in value]

    def __delete__(self, instance):
        instance.extra.pop(self.name, None)


class Person:
    # Abstract class for Attendee and Organizer.
    NAME = "ABSTRACT-PERSON"

    sent_by = PersonProperty("SENT-BY", URIConverter)
    common_name = PersonProperty("CN", RawTextConverter)
    directory = PersonProperty("DIR", URIConverter)

    def __init__(self, email, extra=None, **kwargs):
        self.email = email
        self.extra = extra if extra is not None else {}
        for key, val in kwargs.items():
            setattr(self, key, val)


class Organizer(Person):
    # Organizer of an event or todo.
    NAME = "ORGANIZER"


class Attendee(Person):
    # Attendee of an event or todo.
    NAME = "ATTENDEE"

    user_type = PersonProperty("CUTYPE", default="INDIVIDUAL")
    member = PersonMultiProperty("MEMBER", URIConverter)
    role = PersonProperty("ROLE", default="REQ-PARTICIPANT")
    status = PersonProperty("PARTSTAT", default="NEEDS-ACTION")
    rsvp = PersonProperty("RSVP", BooleanConverter, default=False)
    delegated_to = PersonMultiProperty("DELEGATED-TO", URIConverter)
    delegated_from = PersonMultiProperty("DELEGATED-FROM", URIConverter)