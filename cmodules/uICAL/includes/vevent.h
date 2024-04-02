/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_vevent_h
#define uical_vevent_h

#include "includes/base.h"
#include "includes/datetime.h"

namespace uICAL {
    class VObject;

    class VEvent : public Base {
        public:
            VEvent(const VObject_ptr& event, const TZMap_ptr& tzmap);

            void str(ostream& out) const;

            string summary;
            DateTime start;
            DateTime end;
            RRule_ptr rrule;

            friend class VEventIter;
    };
}
#endif
