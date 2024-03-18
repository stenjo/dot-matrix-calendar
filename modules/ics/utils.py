from datetime import date, datetime, time, timedelta, timezone

'''Implements just enough of UUID so that uuid4().hex will produce
   something usable.
'''

import os


class UUID:

    def __init__(self, bytes=None):
        if bytes is None:
            raise TypeError('Must provide bytes argument')
        if len(bytes) != 16:
            raise ValueError('bytes must be a 16-char string')
        self._int = int.from_bytes(bytes, 'big')

    def __str__(self):
        hex = '%032x' % self._int
        return '%s-%s-%s-%s-%s' % (
               hex[:8], hex[8:12], hex[12:16], hex[16:20], hex[20:])

    @property
    def hex(self):
        return '%032x' % self._int


def uuid4():
    '''Generates a random UUID.'''
    return UUID(bytes=os.urandom(16))

datetime_tzutc = timezone.utc

MIDNIGHT = time()
TIMEDELTA_ZERO = timedelta()
TIMEDELTA_DAY = timedelta(days=1)
TIMEDELTA_SECOND = timedelta(seconds=1)
TIMEDELTA_CACHE = {0: TIMEDELTA_ZERO, "day": TIMEDELTA_DAY, "second": TIMEDELTA_SECOND}
MAX_TIMEDELTA_NEARLY_ZERO = timedelta(seconds=1) / 2

def ensure_datetime(value):
    if value is None:
        return None
    elif isinstance(value, datetime):
        return value
    elif isinstance(value, date):
        return datetime.combine(value, MIDNIGHT, tzinfo=None)
    elif isinstance(value, tuple):
        return datetime(*value)
    elif isinstance(value, dict):
        return datetime(**value)
    else:
        raise ValueError("can't construct datetime from {!r}".format(value))


def ensure_timedelta(value):
    if value is None:
        return None
    elif isinstance(value, timedelta):
        return value
    elif isinstance(value, (tuple, list)):
        return timedelta(*value)
    elif isinstance(value, dict):
        return timedelta(**value)
    else:
        raise ValueError("can't construct timedelta from {!r}".format(value))


def timedelta_nearly_zero(td):
    return -MAX_TIMEDELTA_NEARLY_ZERO <= td <= MAX_TIMEDELTA_NEARLY_ZERO

def floor_datetime_to_midnight(value):
    if value is None:
        return None
    if isinstance(value, date) and not isinstance(value, datetime):
        return value
    return datetime.combine(
        ensure_datetime(value).date(), MIDNIGHT, tzinfo=value.tzinfo
    )

def ceil_datetime_to_midnight(value):
    if value is None:
        return None
    if isinstance(value, date) and not isinstance(value, datetime):
        return value
    floored = floor_datetime_to_midnight(value)
    if floored != value:
        return floored + TIMEDELTA_DAY
    else:
        return floored

def floor_timedelta_to_days(value):
    return value - (value % TIMEDELTA_DAY)

def ceil_timedelta_to_days(value):
    mod = value % TIMEDELTA_DAY
    if mod == TIMEDELTA_ZERO:
        return value
    else:
        return value + TIMEDELTA_DAY - mod

def limit_str_length(val):
    # This is a placeholder. You may need to provide your own implementation to limit the string length.
    return str(val)

def next_after_str_escape(it, full_str):
    try:
        return next(it)
    except StopIteration as e:
        raise ValueError("value '{0}' may not end with an escape sequence".format(full_str))

def uid_gen():
    uid = str(uuid4())
    return "{0}@{1}.org".format(uid, uid[:4])

def validate_not_none(inst, attr, value):
    if value is None:
        raise ValueError("'{0}' may not be None".format(attr))

def validate_truthy(inst, attr, value):
    if not bool(value):
        raise ValueError("'{0}' must be truthy (got {1!r})".format(attr, value))

def check_is_instance(name, value, clazz):
    if not isinstance(value, clazz):
        raise TypeError(
            "'{name}' must be {type!r} (got {value!r} that is a {actual!r}).".format(
                name=name,
                type=clazz.__name__,
                actual=value.__class__.__name__,
                value=value,
            )
        )

def call_validate_on_inst(inst, attr, value):
    inst.validate(attr, value)

def one(iterable):
    it = iter(iterable)
    try:
        first = next(it)
    except StopIteration as e:
        raise ValueError("Too few items in iterable, expected one but got zero!") from e
    try:
        second = next(it)
    except StopIteration:
        return first
    raise ValueError("Expected exactly one item in iterable, but got {0!r}, {1!r}, and possibly more!".format(first, second))
