#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int hours_ = seconds/3600;
    int minutes_ = (seconds-(hours_*3600))/60;
    int seconds_ = seconds - (hours_*3600) - (minutes_*60);
    return (hours_ < 10 ? "0" + std::to_string(hours_) : std::to_string(hours_)) + ":" + 
           (minutes_ < 10 ? "0" + std::to_string(minutes_) : std::to_string(minutes_)) + ":" +
           (seconds_ < 10 ? "0" + std::to_string(seconds_) : std::to_string(seconds_));
}