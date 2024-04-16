/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "includes/cppstl.h"
#include "includes/stream.h"
#include "includes/string.h"

namespace uICAL {
    ostream& ostream::operator <<(const ostream& stm) {
        for (string st : stm.strings) {
            this->strings.push_back(st);
        }
        return *this;
    }

    ostream& ostream::operator <<(const char* st) {
        this->strings.push_back(st);
        return *this;
    }

    ostream& ostream::operator <<(const string& st) {
        this->strings.push_back(st);
        return *this;
    }

    ostream& ostream::operator <<(char ch) {
        this->strings.push_back(string::fmt("%c", ch));
        return *this;
    }

    ostream& ostream::operator <<(int i) {
        this->strings.push_back(string::fmt("%d", i));
        return *this;
    }

    ostream& ostream::operator <<(unsigned int i) {
        this->strings.push_back(string::fmt("%u", i));
        return *this;
    }

    ostream& ostream::operator <<(long long int i) {
        this->strings.push_back(string::fmt("%lld", i));
        return *this;
    }

    ostream::operator string() const {
        return this->str();
    }

    bool ostream::empty() const {
        return this->strings.size() == 0;
    }

    void ostream::clear() {
        this->strings.clear();
    }

    string ostream::str() const {
        string ret = "";
        for (string st : this->strings) {
            ret += st;
        }
        return ret;
    }

    istream_stl::istream_stl(std::istream& istm)
    : istm(istm)
    {}

    char istream_stl::peek() const {
        return this->istm.peek();
    }

    char istream_stl::get() {
        return this->istm.get();
    }

    bool istream_stl::readuntil(string& st, char delim) {
        if (std::getline(this->istm, st, delim)) {
            return true;
        }
        return false;
    }

}
