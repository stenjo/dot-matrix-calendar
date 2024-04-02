/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_bycounterset_h
#define uical_bycounterset_h

#include "includes/counter.h"

namespace uICAL {
    class ByAndCounter : public Counter {
        public:
            static Counter_ptr init(std::vector<Counter_ptr> counters);

            virtual bool next();
            virtual bool reset(const DateStamp& base);

            virtual DateStamp value() const;

            virtual void str(ostream& out) const;

            virtual bool syncLock(const DateStamp& from, const DateStamp& now) const;

        protected:
            ByAndCounter(std::vector<Counter_ptr> counters);
            virtual ~ByAndCounter() = default;

            virtual const string name() const { return "ByAndCounter"; };
            virtual void wrap();

            bool findNextCommon();

            std::vector<Counter_ptr> counters;
            Counter_ptr current;
    };
}
#endif
