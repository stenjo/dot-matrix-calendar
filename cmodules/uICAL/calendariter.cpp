/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "includes/cppstl.h"
#include "includes/types.h"
#include "includes/error.h"
#include "includes/util.h"
#include "includes/logging.h"
#include "includes/calendar.h"
#include "includes/calendarentry.h"
#include "includes/calendariter.h"
#include "includes/tzmap.h"
#include "includes/vevent.h"
#include "includes/veventiter.h"
#include "includes/vline.h"
#include "includes/vlinestream.h"
#include "includes/vobject.h"
#include "includes/vobjectstream.h"

namespace uICAL {
    CalendarIter::CalendarIter(const Calendar_ptr cal, const DateTime& begin, const DateTime& end)
    : cal(cal)
    {
        if (begin.valid() && end.valid() && end < begin) {
            log_error("Begin and end describe a negative range: %s -> %s", begin.as_str().c_str(), end.as_str().c_str());
            throw ValueError("Begin and end describe a negative range");
        }

        for (auto ev : this->cal->events) {
            VEventIter_ptr evIt = new_ptr<VEventIter>(ev, begin, end);

            if (evIt->next()) {  // Initialise and filter
                this->events.push_back(evIt);
            }
        }
    }

    bool CalendarIter::next() {
        if (this->events.size() == 0) {
            return false;
        }

        auto minIt = std::min_element(this->events.begin(), this->events.end());

        this->currentEntry = (*minIt)->entry();

        if (! (*minIt)->next()) {
            this->events.erase(minIt);
        }
        return true;
    }

    CalendarEntry_ptr CalendarIter::current() const {
        if (! this->currentEntry) {
            log_warning("%s", "No more entries");
            throw RecurrenceError("No more entries");
        }
        return this->currentEntry;
    }
}
