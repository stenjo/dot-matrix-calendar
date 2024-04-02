/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "includes/cppstl.h"
#include "includes/types.h"
#include "includes/error.h"
#include "includes/util.h"
#include "includes/logging.h"
#include "includes/vline.h"
#include "includes/vobject.h"

namespace uICAL {
    VObject::VObject()
    {}

    const string& VObject::getName() const {
        return this->name;
    }

    VLine_ptr VObject::getPropertyByName(const string& name) const {
        for (auto line : this->lines) {
            if (line->name == name) {
                return line;
            }
        }
        return nullptr;
    }

    VObject::vector VObject::listObjects(const string& name) const {
        VObject::vector ret;
        for (auto child : this->children) {
            if (child->name == name) {
                ret.push_back(child);
            }
        }
        return ret;
    }

    void VObject::str(ostream& out) const {
        out << "BEGIN:" << this->name << uICAL::endl;
        for (auto line : this->lines) {
            line->str(out);
            out << uICAL::endl;
        }

        for (auto child : this->children) {
            child->str(out);
        }

        out << "END:" << this->name << uICAL::endl;
    }
}
