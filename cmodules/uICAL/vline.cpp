/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "includes/cppstl.h"
#include "includes/types.h"
#include "includes/error.h"
#include "includes/util.h"
#include "includes/logging.h"
#include "includes/vline.h"

namespace uICAL {
    VLine::VLine(const string& line) {
        if(line.empty()) {
            log_error("%s", "VLINE is empty");
            throw ParseError("VLINE is empty");
        }

        size_t colon = line.find(":");
        size_t semicolon = line.find(";");

        if (colon == string::npos) {
            log_error("VLINE does not have a ':' \"%s\"", line.c_str());
            throw ParseError(string("VLINE does not have a ':' \"") + line + "\"");
        }

        if (semicolon != string::npos && semicolon < colon) {
            this->name = line.substr(0, semicolon);
            this->readParams(line.substr(semicolon + 1, colon - semicolon - 1));
        }
        else {
            this->name = line.substr(0, colon);
        }
        this->value = line.substr(colon + 1, line.length() - colon - 1);
    }

    string VLine::getParam(const string& key) {
        for (auto param : this->params) {
            if (param.first == key) {
                return param.second;
            }
        }
        return string("");
    }

    void VLine::readParams(const string& str) {
        str.tokenize(';', [&](const string token){
            size_t equals = token.find("=");
            if (equals == string::npos) {
                log_error("Bad param: \"%s\"", token.c_str());
                throw ParseError(string("Bad param: ") + token);
            }
            const string name = token.substr(0, equals);
            const string value = token.substr(equals + 1, token.length());
            this->params.insert(std::pair<string, string>(name, value));
        });
    }

    void VLine::str(ostream& out) const {
        out << this->name;
        if (this->params.size()) {
            for (auto kv : this->params) {
                out << ";";
                out << kv.first << "=" << kv.second ;
            }
        }
        out << ":" << this->value;
    }
}
