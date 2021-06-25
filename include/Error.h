#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <system_error>
#include "tsqueue.h"

namespace be
{
    class Error
    {
        // A thread safe queue of error messages
        static tsqueue<std::string> error_messages;
    public:

        static void write_error(std::string msg)
        {
            if(msg != "")
                error_messages.push_back(msg);
        }

        static std::string get_error()
        {
            if(error_messages.size() > 0)
                return error_messages.pop_front();
            return "";
        }


        class exception : std::exception
        {
            const char* msg;
        public:

            exception(const char* msg) : msg(msg)
            {
            }

            const char* what() const _GLIBCXX_USE_NOEXCEPT
            {
                return msg;
            }
        };
    };

}
#endif // ERROR_H_INCLUDED
