/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "includes/cppstl.h"
#include "includes/types.h"
#include "includes/util.h"
#include "includes/calendarentry.h"
#include "includes/datetime.h"
#include "includes/rruleiter.h"
#include "includes/vevent.h"
#include "includes/veventiter.h"

namespace uICAL {
    VEventIter::VEventIter(const VEvent_ptr ice, DateTime begin, DateTime end)
    : ice(ice)
    {
        this->range_begin = begin;
        this->rrule = new_ptr<RRuleIter>(this->ice->rrule, DateTime(), end);
    }

    bool VEventIter::next() {
        if (! this->range_begin.valid()) {
            return this->rrule->next();
        }
        DatePeriod span = this->ice->end - this->ice->start;
        while (this->rrule->next())
        {
            DateTime end = this->rrule->now() + span;

            if (end <= this->range_begin)
                continue;
            return true;
        }
        return false;
    }

    DateTime VEventIter::now() const {
        return this->rrule->now();
    }

    CalendarEntry_ptr VEventIter::entry() const {
        return new_ptr<CalendarEntry>(CalendarEntry::Type::EVENT,
                     this->ice->summary,
                     this->rrule->now(),
                     this->ice->end - this->ice->start);
    }

    bool operator < (const VEventIter_ptr& a, const VEventIter_ptr& b) {
        return a->rrule->now() < b->rrule->now();
    }

}
