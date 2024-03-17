import re
import sys
from contextlib import contextmanager
from textwrap import TextWrapper

from ics.types import (
    ContainerItem,
    ExtraParams,
    RuntimeAttrValidation,
    copy_extra_params,
)
from ics.utils import limit_str_length, validate_truthy

DEFAULT_LINE_WRAP = TextWrapper(
    width=75,
    initial_indent="",
    subsequent_indent="",
    break_long_words=True,
    break_on_hyphens=True,
    expand_tabs=False,
    replace_whitespace=False,
    fix_sentence_endings=False,
    drop_whitespace=False,
)

@contextmanager
def contentline_set_wrap(width):
    oldwidth = DEFAULT_LINE_WRAP.width
    if not width or width <= 0:
        DEFAULT_LINE_WRAP.width = sys.maxsize
    else:
        DEFAULT_LINE_WRAP.width = width
    try:
        yield
    finally:
        DEFAULT_LINE_WRAP.width = oldwidth


class ParseError(Exception):
    def __init__(self, msg, line_nr=-1, col=-1, line=None, state=None):
        super().__init__(msg)
        self.msg = msg
        self.line_nr = line_nr
        self.col = col
        self.line = line
        self.state = state

    def __str__(self):
        strs = ["Line"]
        if self.line_nr != -1:
            strs.append(f" {self.line_nr}")
            if self.col != -1:
                if isinstance(self.col, int):
                    strs.append(f":{self.col}")
                else:
                    strs.append(":%s-%s" % self.col)
        strs.append(" ")
        strs.append(self.msg)
        if self.line:
            strs.append(": ")
            strs.append(self.line)
        if self.state:
            strs.append(" (")
            strs.append(self.state)
            strs.append(")")
        return "".join(strs)


class QuotedParamValue():
    @classmethod
    def maybe_unquote(cls, txt):
        if not txt:
            return txt
        if txt[0] == Patterns.DQUOTE:
            assert len(txt) >= 2
            assert txt[-1] == Patterns.DQUOTE
            return txt[1:-1]  # Unquoted
        else:
            return txt


def escape_param(string):
    if isinstance(string, str):
        return string.translate({ord('"'): "^'", ord("^"): "^^", ord("\n"): "^n", ord("\r"): ""})
    else:
        raise TypeError("Expected str instance, got {0}".format(type(string).__name__))


def unescape_param(string):
    def repl(match):
        g = match.group(1)
        if g == "n":
            return "\n"
        elif g == "^":
            return "^"
        elif g == "'":
            return '"'
        elif len(g) == 0:
            raise ParseError(
                "parameter value '%s' may not end with an escape sequence" % string
            )
        else:
            raise ParseError(
                "invalid escape sequence ^%s in parameter value '%s'" % (g, string)
            )

    return re.sub(r"\^(.?)", repl, string)


class Patterns:
    CONTROL = "\x00-\x08\x0A-\x1F\x7F"
    DQUOTE = '"'
    LINEBREAK = "\r?\n|\r"
    LINEFOLD = "(" + LINEBREAK + ")[ \t]"
    QSAFE_CHARS = "[^" + CONTROL + DQUOTE + "]*"
    SAFE_CHARS = "[^" + CONTROL + DQUOTE + ",:;]*"
    VALUE_CHARS = "[^" + CONTROL + "]*"
    IDENTIFIER = "[a-zA-Z0-9-]+"
    PVAL = "(?P<pval>" + DQUOTE + QSAFE_CHARS + DQUOTE + "|" + SAFE_CHARS + ")"
    PVALS = PVAL + "(," + PVAL + ")*"
    PARAM = "(?P<pname>" + IDENTIFIER + ")=(?P<pvals>" + PVALS + ")"
    LINE = (
        "(?P<name>" + IDENTIFIER + ")(;" + PARAM + ")*:(?P<value>" + VALUE_CHARS + ")"
    )


class ContentLine(RuntimeAttrValidation):
    def __init__(self, name, params=None, value="", line_nr=-1):
        self.name = str(name).upper()
        self.params = params or {}
        self.value = value
        self.line_nr = line_nr

    def serialize(self, newline=False, wrap=DEFAULT_LINE_WRAP):
        if wrap is None:
            return self._serialize_unwrapped(newline)
        return "\r\n".join(wrap.wrap(self._serialize_unwrapped(newline)))

    def serialize_iter(self, newline=False, wrap=DEFAULT_LINE_WRAP):
        if wrap is None:
            return self._serialize_iter_unwrapped(newline)
        lines = [
            elem
            for line in wrap.wrap(self._serialize_unwrapped(False))
            for elem in [line, "\r\n"]
        ]
        if not newline:
            lines.pop()
        return lines

    def _serialize_unwrapped(self, newline=False):
        return "".join(self._serialize_iter_unwrapped(newline))

    def _serialize_iter_unwrapped(self, newline=False):
        yield self.name
        for pname in self.params:
            yield ";"
            yield pname
            yield "="
            for nr, pval in enumerate(self.params[pname]):
                if nr > 0:
                    yield ","
                if isinstance(pval, QuotedParamValue) or re.search("[:;,]", pval):
                    yield f'"{escape_param(pval)}"'
                else:
                    yield escape_param(pval)
        yield ":"
        yield self.value
        if newline:
            yield "\r\n"


class Container():
    def __init__(self, name, data=None):
        self.name = str(name).upper()
        self.data = data or []
        self.check_items(self.data)

    def _check_item(self, item):
        # Custom implementation of validation that doesn't rely on 'attr' or 'functools'
        pass

    def check_items(self, *items):
        for item in items:
            self._check_item(item)

    def __getitem__(self, index):
        return self.data[index]

    def __setitem__(self, index, value):
        self._check_item(value)
        self.data[index] = value

    def __delitem__(self, index):
        del self.data[index]

    def __len__(self):
        return len(self.data)

    def insert(self, index, value):
        self._check_item(value)
        self.data.insert(index, value)


# You may need to manually implement other MutableSequence abstract methods.