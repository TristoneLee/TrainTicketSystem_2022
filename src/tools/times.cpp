#ifndef TRAINTICKETSYSTEM_2022_SRC_TOOLS_TIMES_CPP
#define TRAINTICKETSYSTEM_2022_SRC_TOOLS_TIMES_CPP
#include <asserts.h>
#include <times.h>

#include <iostream>
using std::string;
namespace sjtu {
const int Date::kDaysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int Date::kSumDaysInMonth[] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

Date::Date(int _month, int _day) : month(_month), day(_day) {}
Date::Date(const string& input)
    : month((input[0] - '0') * 10 + (input[1] - '0')), day((input[3] - '0') * 10 + (input[4] - '0')) {}
Date::Date(const Date& other) = default;
Date& Date::operator=(const Date& other) = default;
bool Date::operator==(const Date& other) const { return month == other.month && day == other.day; }
bool Date::operator!=(const Date& other) const { return !(*this == other); }
bool Date::operator<(const Date& other) const {
  if (month != other.month) return month < other.month;
  return day < other.day;
}
bool Date::operator>(const Date& other) const {
  if (month != other.month) return month > other.month;
  return day > other.day;
}
bool Date::operator<=(const Date& other) const { return *this < other || *this == other; }
bool Date::operator>=(const Date& other) const { return *this > other || *this == other; }
Date Date::operator+(int days) const {
  Date ret(*this);
  ret += days;
  return ret;
}
Date Date::operator-(int days) const {
  Date ret(*this);
  ret -= days;
  return ret;
}
Date& Date::operator+=(int days) {
  day += days;
  while (day > kDaysInMonth[month]) {
    day -= kDaysInMonth[month];
    month++;
    Assert(month <= 12);
  }
  return *this;
}
Date& Date::operator-=(int days) {
  day -= days;
  while (day < 1) {
    month--;
    Assert(month >= 1);
    day += kDaysInMonth[month];
  }
  return *this;
}
int Date::operator-(const Date& other) const {
  return kSumDaysInMonth[month] + day - (kSumDaysInMonth[other.month] + other.day);
}
string Date::ToString() const {  // format : mm-dd
  string ret;
  ret.resize(6);
  ret.push_back('0' + month / 10);
  ret.push_back('0' + month % 10);
  ret.push_back('-');
  ret.push_back('0' + day / 10);
  ret.push_back('0' + day % 10);
  return ret;
}
Date::~Date() = default;

Time::Time(int _hour, int _minute, const Date& _day) : date(_day), hour(_hour), minute(_minute) {}
Time::Time(const string& input, const Date& _day)
    : hour((input[0] - '0') * 10 + (input[1] - '0')), minute((input[3] - '0') * 10 + (input[4] - '0')), date(_day) {}
Time::Time(const Time& other) = default;
Time& Time::operator=(const Time& other) = default;
bool Time::operator==(const Time& other) const {
  return hour == other.hour && minute == other.minute && date == other.date;
}
bool Time::operator!=(const Time& other) const { return !(*this == other); }
bool Time::operator<(const Time& other) const {
  if (date != other.date) return date < other.date;
  if (hour != other.hour) return hour < other.hour;
  return minute < other.minute;
}
bool Time::operator>(const Time& other) const {
  if (date != other.date) return date > other.date;
  if (hour != other.hour) return hour > other.hour;
  return minute > other.minute;
}
bool Time::operator<=(const Time& other) const { return *this < other || *this == other; }
bool Time::operator>=(const Time& other) const { return *this > other || *this == other; }
Time Time::operator+(int minutes) const {
  Time ret(*this);
  ret += minutes;
  return ret;
}
Time Time::operator-(int minutes) const {
  Time ret(*this);
  ret -= minutes;
  return ret;
}
Time& Time::operator+=(int minutes) {
  minute += minutes;
  hour += minute / 60;
  minute %= 60;
  date += hour / 24;
  return *this;
}
Time& Time::operator-=(int minutes) {
  minute -= minutes;
  hour -= minute / 60;
  minute %= 60;
  if (minute < 0) {
    minute += 60;
    hour--;
  }
  date -= hour / 24;
  if (hour < 0) {
    hour += 24;
    date -= 1;
  }
  return *this;
}
int Time::operator-(const Time& other) const {
  return (date - other.date) * kHoursInDay * kMinutesInHour + (hour - other.hour) * kMinutesInHour + minute -
         other.minute;
}
string Time::ToString() const {  // only output the time hh:mm
  string ret;
  ret.resize(6);
  ret.push_back('0' + hour / 10);
  ret.push_back('0' + hour % 10);
  ret.push_back(':');
  ret.push_back('0' + minute / 10);
  ret.push_back('0' + minute % 10);
  return ret;
}
Time::~Time() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_TOOLS_TIMES_CPP