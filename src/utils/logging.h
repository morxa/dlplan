#ifndef SRC_UTILS_LOGGING_H
#define SRC_UTILS_LOGGING_H

/**
 * Taken from fast-downward.org
 */

#include "system.h"
#include "timer.h"

#include <iostream>
#include <ostream>
#include <string>
#include <vector>


namespace dlp {
namespace utils {
/*
  Simple logger that prepends time and peak memory info to messages.
  Logs are written to stdout.
  Usage:
        utils::g_log << "States: " << num_states << endl;
*/
class Log {
private:
    bool line_has_started = false;

public:
    template<typename T>
    Log &operator<<(const T &elem) {
        if (!line_has_started) {
            line_has_started = true;
            std::cout << "[t=" << g_timer << ", "
                      << get_peak_memory_in_kb() << " KB] ";
        }

        std::cout << elem;
        return *this;
    }

    using manip_function = std::ostream &(*)(std::ostream &);
    Log &operator<<(manip_function f) {
        if (f == static_cast<manip_function>(&std::endl)) {
            line_has_started = false;
        }

        std::cout << f;
        return *this;
    }
};

extern Log g_log;

}
}

#endif