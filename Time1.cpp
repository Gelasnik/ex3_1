#include "Time1.h"
#include <iomanip>
#include <stdexcept>

using std::ostream;

namespace ParkingLotUtils {

    static const Time1::Hour HOURS_PER_DAY = 24;
    static const Time1::Minute MINUTES_PER_HOUR = 60;

    static Time1::Minute toMinutes(Time1::Day day, Time1::Hour hour, Time1::Minute minute) {
        return (day * HOURS_PER_DAY * MINUTES_PER_HOUR + hour * MINUTES_PER_HOUR + minute);
    }

    static Time1 fromMinutes(Time1::Minute minutes) {
        Time1::Hour hours = minutes / MINUTES_PER_HOUR;
        minutes = minutes % MINUTES_PER_HOUR;
        Time1::Day days = hours / HOURS_PER_DAY;
        hours = hours % HOURS_PER_DAY;
        return Time1(days, hours, minutes);
    }

    Time1::Time1(Time1::Day day, Time1::Hour hour, Time1::Minute minute) : day(day), hour(hour), minute(minute) {
        if (hour > HOURS_PER_DAY || minute > MINUTES_PER_HOUR) {
            throw std::invalid_argument("Hour must be <= 24 and Minute must be <= 60");
        }
    }

    Time1 Time1::operator-(const Time1 &time) const {
        Minute timeMinutes = toMinutes(time.day, time.hour, time.minute);
        Minute thisMinutes = toMinutes(this->day, this->hour, this->minute);
        if (thisMinutes < timeMinutes) {
            return fromMinutes(timeMinutes - thisMinutes);
        } else {
            return fromMinutes(thisMinutes - timeMinutes);
        }
    }

    Time1& Time1::operator+=(const Minute minutes) {
        Time1 newTime = fromMinutes(toMinutes(this->day, this->hour, this->minute) + minutes);
        (*this) = newTime;
        return (*this);
    }

    Time1::Hour Time1::toHours() const {
        return (this->day * HOURS_PER_DAY + this->hour + (this->minute > 0 ? 1 : 0));
    }

    ostream& operator<<(ostream &os, const Time1 &time) {
        return os << "Day " << time.day << ", " <<
                  std::setfill('0') << std::setw(2) << time.hour << ":" <<
                  std::setfill('0') << std::setw(2) << time.minute;
    }

}