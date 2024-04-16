/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_string_h
#define uical_string_h

#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <includes/stream.h>

namespace uICAL {
    extern const char* endl;
    extern const char* fmt_04d;
    extern const char* fmt_02d;

    class string : public std::string {
        public:
            static const string& none();

            string() : std::string() {}
            string(const std::string& b) : std::string(b) {}
            string(const char* st) : std::string(st) {}
            string(const std::ostringstream& sst) : std::string(sst.str()) {}
            string(const string&) = default;

            string& operator = (const string& st) { std::string::operator =(st); return *this; }

            string substr(size_t from) const { return string(std::string::substr(from)); }
            string substr(size_t from, size_t len) const { return string(std::string::substr(from, len)); }

            template<typename... Args>
            static string fmt(const char* fmt, Args... args) {
                char buf[40];
                memset(buf, 0, 40);
                sprintf(buf, fmt, args...);
                if (buf[39] != 0) {
                    throw_implementationError("string::format buffer overflow");
                }
                return string(buf);
            }

            int as_int() const;
            void rtrim();
            bool readfrom(istream& is, char delim);
            void tokenize(char delim, std::function<void (string)> cb) const;

        protected:
            static void throw_implementationError(const char* msg);
    };

}
#endif
