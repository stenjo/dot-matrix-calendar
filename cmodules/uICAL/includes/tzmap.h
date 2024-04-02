/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_tzinfo_h
#define uical_tzinfo_h

#include "includes/base.h"

namespace uICAL {
    class VComponent;

    class TZMap : public Base {
        public:
            TZMap();

            void add(const VObject_ptr& timezone);

            string findId(const string& nameOrId) const;

            int getOffset(const string& tzId);
            string getName(const string& tzId);

            void str(ostream& out) const;

        protected:
            void add(const string& id, const string& name, const string& tz);
            int parseOffset(const string& offset) const;

            typedef struct {
                int offset;
                string name;
            } attribs_t;

            std::map<string, attribs_t> id_attrib_map;
    };
}
#endif
