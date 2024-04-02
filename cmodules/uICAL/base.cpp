/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "includes/cppstl.h"
#include "includes/types.h"
#include "includes/base.h"

namespace uICAL {

    string Base::as_str() const {
        ostream stm;
        this->str(stm);
        return stm;
    }

    ostream& operator << (ostream& out, const Base& b) {
        b.str(out);
        return out;
    }
}
