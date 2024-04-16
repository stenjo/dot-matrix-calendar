/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_stream_h
#define uical_stream_h

#include <istream>
#include <vector>
namespace uICAL {
    class string;

    class ostream {
        public:
            ostream& operator <<(const ostream& stm);
            ostream& operator <<(const char* st);
            ostream& operator <<(const string& st);
            ostream& operator <<(char ch);
            ostream& operator <<(int i);
            ostream& operator <<(unsigned int i);
            ostream& operator <<(long long int i);

            operator string() const;

            bool empty() const;
            void clear();

            string str() const;
        protected:
            std::vector<string> strings;
    };

    class istream {
        public:
            virtual ~istream() = default;

            virtual char peek() const = 0;
            virtual char get() = 0;

            virtual bool readuntil(string& st, char delim) = 0;
        protected:
    };


    class istream_stl : public istream {
        public:
            istream_stl(std::istream& istm);

            char peek() const;
            char get();

            bool readuntil(string& st, char delim);

        protected:
            std::istream& istm;
    };

}
#endif
