/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "includes/cppstl.h"
#include "includes/types.h"
#include "includes/error.h"
#include "includes/util.h"
#include "includes/logging.h"
#include "includes/vevent.h"
#include "includes/vline.h"
#include "includes/vlinestream.h"
#include "includes/vobject.h"
#include "includes/vobjectstream.h"

namespace uICAL {
    VObjectStream::VObjectStream(VLineStream& stm)
    : stm(stm)
    , useLine([](const string parent, const string line) {
            std::ignore = parent;
            std::ignore = line;
            return true;
        })
    {}

    VObjectStream::VObjectStream(VLineStream& stm, lineP_t useLine)
    : stm(stm)
    , useLine(useLine)
    {}

    string VObjectStream::nextObjectName() {
        VLine_ptr line = this->stm.next();

        if (!line) {
            log_error("Parse error: %s", "Empty stream");
            throw ParseError(string("Parse error, empty stream"));
        }
        if (line->name == "END") {
            log_trace("Final component: %s", line->as_str().c_str());
            return string::none();
        }
        if (line->name != "BEGIN") {
            log_error("Parse error, expected BEGIN: %s", line->as_str().c_str());
            throw ParseError(string("Parse error, expected BEGIN: ") + line->as_str());
        }

        log_trace("Component BEGIN: %s", line->value.c_str());
        return line->value;
    }

    void VObjectStream::loadObject(string objName, VObject_ptr& obj, bool recurse) {
        while(true) {
            VLine_ptr line = this->stm.next();
            if (!line) {
                log_error("%s", "Parse error, unexpected end of ICAL");
                throw ParseError("Parse error, unexpected end of ICAL");
            }
            if (line->name == "BEGIN") {
                stm.repeatLine();
                if (recurse || obj == nullptr) {
                    string childName = this->nextObjectName();
                    VObject_ptr child = nullptr;
                    if (this->useLine(childName, string::none())) {
                        child = new_ptr<VObject>();
                        child->name = childName;
                        log_trace("Use component: %s", childName.c_str());
                    }
                    else {
                        log_trace("Ignore component: %s", childName.c_str());
                    }
                    loadObject(childName, child, true);
                    if (obj && child) {
                        obj->children.push_back(child);
                    }
                }
                else {
                    return;
                }
            }
            else if (line->name == "END") {
                if (objName == line->value) {
                    log_trace("Component END: %s", line->value.c_str());
                    return;
                }
                log_error("Mismatch \"%s\": %s", objName.c_str(), line->as_str().c_str());
                throw ParseError(string("Mismatch for: \"") + objName.c_str() + string("\": ") + line->as_str());
            }
            else {
                if (obj && this->useLine(obj->getName(), line->name)) {
                    obj->lines.push_back(line);
                    log_trace("Use line: %s", line->as_str().c_str());
                }
                else {
                    log_trace("Ignore line: %s", line->as_str().c_str());
                }
            }
        }
    }

    VObject_ptr VObjectStream::nextObject(bool recurse) {
        while(true) {
            string objName = this->nextObjectName();
            if (objName.empty()) {
                return nullptr;
            }
            VObject_ptr obj = nullptr;
            if (this->useLine(objName, string())) {
                obj = new_ptr<VObject>();
                obj->name = objName;
            }
            loadObject(objName, obj, recurse);
            if (obj) {
                return obj;
            }
        }
    }
}
