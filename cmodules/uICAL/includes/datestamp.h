/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_datestamp_h
#define uical_datestamp_h

#include "includes/base.h"
#include "includes/datetime.h"
#include "includes/epochtime.h"

namespace uICAL {
    class DateStamp : public Base {
        public:
            DateStamp();
            DateStamp(const string& datestamp);
            DateStamp(unsigned year, unsigned month, unsigned day, unsigned hour, unsigned minute, unsigned second);

            void str(ostream& out) const;

            unsigned year;
            unsigned month;
            unsigned day;
            unsigned hour;
            unsigned minute;
            unsigned second;

            bool valid() const;

            DateTime::Day dayOfWeek() const;
            unsigned weekNo() const;
            unsigned dayOfYear() const;
            unsigned daysInMonth() const;
            unsigned daysInYear() const;

            void incYear(unsigned n);
            void incMonth(unsigned n);
            void incWeek(unsigned n, DateTime::Day wkst);
            void incDay(unsigned n);
            void incHour(unsigned n);
            void incMinute(unsigned n);
            void incSecond(unsigned n);

            void decDay(unsigned n);
            void decMonth(unsigned n);

            void setWeekNo(unsigned n);

            DateStamp(const DateStamp&) = default;
            DateStamp& operator = (const DateStamp& ds);

            bool operator > (const DateStamp& ds) const;
            bool operator < (const DateStamp& ds) const;
            bool operator <= (const DateStamp& ds) const;
            bool operator == (const DateStamp& ds) const;
            bool operator != (const DateStamp& ds) const;

        protected:
            seconds_t index() const;
            void validate() const;
            DateTime::Day getWeekDay(unsigned days) const;
    };
}
#endif
