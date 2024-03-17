# parser.py

class ContentLine:
    def __init__(self, name, params=None, value=None):
        self.name = name
        self.params = params if params is not None else {}
        self.value = value

    def __repr__(self):
        params_str = "".join([";{}={}".format(k, ",".join(v))
                             for k, v in self.params.items()])
        return "{}{}:{}".format(self.name,
                                params_str if params_str else '',
                                ",".join(self.value) if self.value is not None else '')

def unfold(lines):
    buffer = ""
    for line in lines:
        if line.startswith((' ', '\t')):
            buffer += line[1:]
            continue
        if buffer:
            yield buffer
        buffer = line.rstrip()
    if buffer:
        yield buffer

def parse_params(params_str):
    params = {}
    for pair in params_str.split(";"):
        if '=' in pair:
            key, val = pair.split('=', 1)
            params[key] = val.split(",")
        else:
            params[pair] = []
    return params

def parse_line_to_contentline(line):
    parts = line.split(':', 1)
    if len(parts) < 2:
        raise ValueError("Line is not a valid content line (no colon found): {}".format(line))
    name_params, value = parts
    name, *param_parts = name_params.split(';')
    params = parse_params(';'.join(param_parts))
    value = value.split(',')  # Simplified handling of values
    return ContentLine(name, params, value)

class Container(list):  # Inherits from list to store ContentLines
    def __init__(self, name):
        super().__init__()
        self.name = name

    def __repr__(self):
        return "<Container: {} with {} elements>".format(self.name, len(self))

class ParserClass:
    # The ParserClass should provide methods to parse strings and lines into Containers

    def string_to_containers(self, string):
        lines = string.splitlines()
        return self.lines_to_containers(lines)

    def lines_to_containers(self, lines):
        containers = []
        current_container = None

        for line in unfold(lines):
            content_line = parse_line_to_contentline(line)
            if content_line.name == 'BEGIN':
                container_name = content_line.value[0]
                current_container = Container(container_name)
            elif content_line.name == 'END':
                containers.append(current_container)
                current_container = None
            else:
                if current_container is not None:
                    current_container.append(content_line)
                else:
                    raise ValueError('Content line outside of a BEGIN/END block.')

        return containers

# Usage example:
ics_string = """BEGIN:VCALENDAR
VERSION:2.0
BEGIN:VEVENT
SUMMARY:MicroPython Meeting
DTSTART;VALUE=DATE-TIME:20230401T120000Z
DTEND;VALUE=DATE-TIME:20230401T130000Z
END:VEVENT
END:VCALENDAR
"""

# parser = ParserClass()
# ics_containers = parser.string_to_containers(ics_string)
# for container in ics_containers:
#     print(container)
#     for line in container:
#         print(line)