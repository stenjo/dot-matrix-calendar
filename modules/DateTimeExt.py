from datetime import datetime, timedelta

class timedelta(timedelta):
    pass
    
class datetime(datetime):
    
    def strftime(self, format):
        """
        Format using strftime().

        Example: "%d/%m/%Y, %H:%M:%S"
        """
        return self._wrap_strftime(format, self.timetuple())

    
    def _strftime(self, format, timetuple):
        mapping = {
            '%d':f"{timetuple.tm_mday:02d}",
            '%m': f"{timetuple.tm_mon:02d}",
            '%H': f"{timetuple.tm_hour:02d}",
            '%M': f"{timetuple.tm_min:02d}",
            '%S': f"{timetuple.tm_sec:02d}",
            '%Y': f"{timetuple.tm_year}",
            '%y': f"{timetuple.tm_year}"[2:]
            }
        
        params = [format[i:i+2] for i, c in enumerate(format) if c == '%']
        
        while params:
            p = params.pop()
            format = format.replace(p, mapping[p])

        return format
                        
    def _wrap_strftime(self, format, timetuple):
        # Don't call utcoffset() or tzname() unless actually needed.
        fReplace = None  # the string to use for %f
        zReplace = None  # the string to use for %z
        colonZReplace = None  # the string to use for %:z
        zReplace = None  # the string to use for %Z

        # Scan format for %z, %:z and %Z escapes, replacing as needed.
        newFormat = []
        push = newFormat.append
        i, n = 0, len(format)
        while i < n:
            ch = format[i]
            i += 1
            if ch == '%':
                if i < n:
                    ch = format[i]
                    i += 1
                    if ch == 'f':
                        if fReplace is None:
                            fReplace = '%06d' % getattr(object,
                                                        'microsecond', 0)
                        newFormat.append(fReplace)
                    elif ch == 'z':
                        if zReplace is None:
                            if hasattr(object, "utcoffset"):
                                zReplace = self._format_offset(self.utcoffset(), sep="")
                            else:
                                zReplace = ""
                        assert '%' not in zReplace
                        newFormat.append(zReplace)
                    elif ch == ':':
                        if i < n:
                            ch2 = format[i]
                            i += 1
                            if ch2 == 'z':
                                if colonZReplace is None:
                                    if hasattr(object, "utcoffset"):
                                        colonZReplace = self._format_offset(self.utcoffset(), sep=":")
                                    else:
                                        colonZReplace = ""
                                assert '%' not in colonZReplace
                                newFormat.append(colonZReplace)
                            else:
                                push('%')
                                push(ch)
                                push(ch2)
                    elif ch == 'Z':
                        if zReplace is None:
                            zReplace = ""
                            if hasattr(object, "tzname"):
                                s = object.tzname()
                                if s is not None:
                                    # strftime is going to have at this: escape %
                                    zReplace = s.replace('%', '%%')
                        newFormat.append(zReplace)
                    else:
                        push('%')
                        push(ch)
                else:
                    push('%')
            else:
                push(ch)
        newFormat = "".join(newFormat)
        return self._strftime(newFormat, timetuple)
