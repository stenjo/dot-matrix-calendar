
from ics.contentline.container import (
    Container,
    ContentLine,
    ParseError,
    QuotedParamValue,
)
from ics.contentline.parser import ParserClass
from ics.utils import one

# Assume the modules Container, ContentLine, ParseError, ParserClass, and one() 
# function, are all available in MicroPython and work as expected.

Parser = ParserClass()
string_to_containers = Parser.string_to_containers
lines_to_containers = Parser.lines_to_containers

def string_to_container(txt):
    return one(string_to_containers(txt))

def lines_to_container(lines):
    return one(lines_to_containers(lines))

__all__ = [
    "ParseError",
    "QuotedParamValue",
    "ContentLine",
    "Container",
    "Parser",
    "string_to_containers",
    "lines_to_containers",
    "string_to_container",
    "lines_to_container",
]
