/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "includes/cppstl.h"
#include "includes/stream.h"
#include "includes/string.h"
#include "includes/error.h"

namespace uICAL {
    const char* endl = "\n";
    const char* fmt_04d = "%04d";
    const char* fmt_02d = "%02d";

    static string EMPTY = string();

    const string& string::none() {
        return EMPTY;
    }

    void string::tokenize(char delim, std::function<void (string)> cb) const {
        if (this->empty()) {
            return;
        }

        char pattern[2];
        pattern[0] = delim;
        pattern[1] = 0;
        size_t cursor = 0;
        while(true) {
            size_t index = this->find(pattern, cursor);
            if (index == npos) {
                cb(this->substr(cursor));
                return;
            }
            cb(this->substr(cursor, index - cursor));
            cursor = index + 1;
        }
    }

    bool string::readfrom(istream& istm, char delim) {
        return istm.readuntil(*this, delim);
    }

    void string::throw_implementationError(const char* msg) {
        throw ImplementationError(msg);
    }

    #ifdef ARDUINO

        int string::as_int() const {
            return this->toInt();
        }

        void string::rtrim() {
            this->trim();
        }

    #else

        int string::as_int() const
        {
            return std::atoi(this->c_str());
        }

        void string::rtrim() {
            this->erase(std::find_if(this->rbegin(), this->rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), this->end());
        }

    #endif
}
