import re
import sys
from contextlib import contextmanager


# You may need to manually implement other MutableSequence abstract methods.

class CustomTextWrapper:
    def __init__(self, width=75, initial_indent="", subsequent_indent=" ",
                 break_long_words=True, break_on_hyphens=True):
        self.width = width
        self.initial_indent = initial_indent
        self.subsequent_indent = subsequent_indent
        self.break_long_words = break_long_words
        self.break_on_hyphens = break_on_hyphens

    def _wrap_long_word(self, word, current_line):
        wrapped_lines = []
        space_left = self.width - len(current_line.rstrip())
        
        # If the word is already longer than the width of a line, it will
        # be placed on a new line (or multiple lines as needed).
        if self.break_long_words and len(word) > self.width:
            if current_line.strip():  # If there's any content already, start a new line
                wrapped_lines.append(current_line.rstrip())
                current_line = self.subsequent_indent

            while len(word) > self.width:
                wrapped_lines.append(word[:self.width])
                word = word[self.width:]
            current_line += word
        else:
            # The word fits in the space left, or is allowed to exceed line length
            current_line += word[:space_left]
            word = word[space_left:]  # Remainder is considered a "new word" and processed again

        return wrapped_lines, current_line

    def wrap(self, text):
        wrapped_lines = []  # This will be the list of wrapped lines.
        current_line = self.initial_indent  # Start the first line with initial_indent

        for word in text.split():
            hyphenated_parts = [word]  # Default: no hyphenation
            if self.break_on_hyphens and '-' in word:
                # Split the word by hyphens into parts
                hyphenated_parts = word.split('-')

            for i, part in enumerate(hyphenated_parts):
                # Add back the hyphen if it's not the last part
                if i < len(hyphenated_parts) - 1:
                    part += '-'

                # Check if the current part plus a space can fit on the current line
                if len(current_line) + len(part) + (1 if current_line.strip() else 0) > self.width:
                    # Wrap to next line if no space is left
                    wrapped_lines.append(current_line.rstrip())
                    current_line = self.subsequent_indent + part
                else:
                    # If the current line has other content on it, add a space before the word
                    if current_line.strip():
                        current_line += ' '
                    current_line += part

                # Check if the wrapped text needs to wrap again
                while len(current_line) > self.width:
                    additional_lines, current_line = self._wrap_long_word(current_line, '')
                    wrapped_lines.extend(additional_lines)
                    
        # After the loop, add the remaining content to the list of wrapped lines
        if current_line.strip():
            wrapped_lines.append(current_line)

        return wrapped_lines
    
DEFAULT_LINE_WRAP = CustomTextWrapper(
    # width=75,
    # initial_indent="",
    # subsequent_indent=" ",
    # break_long_words=True,
    # break_on_hyphens=False,
    # # # expand_tabs=False,
    # # replace_whitespace=False,
    # # fix_sentence_endings=False,
    # # drop_whitespace=False,
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


class ContentLine:
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



# Example usage of CustomTextWrapper:
# wrapper = CustomTextWrapper(width=30, initial_indent="* ", subsequent_indent="- ")
# text = "The quick brown fox jumps over-the-lazy dog. The quick brown-fox jumps over the-lazy dog."
# wrapped_lines = wrapper.wrap(text)

# for line in wrapped_lines:
#     print(repr(line))