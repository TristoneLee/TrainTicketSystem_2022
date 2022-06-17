#ifndef TRAINTICKETSYSTEM_2022_SRC_TOOLS_TIMES_H
#define TRAINTICKETSYSTEM_2022_SRC_TOOLS_TIMES_H
#include <iostream>
using std::ostream;
using std::string;
namespace sjtu {
class Date {
 private:
  static const int kDaysInMonth[13];
  static const int kSumDaysInMonth[13];  // sum of days before each month

 public:
  int month, day;
  Date(int _month = 1, int _day = 1);
  Date(const string& input);
  Date(const Date& other);
  Date& operator=(const Date& other);
  bool operator==(const Date& other) const;
  bool operator!=(const Date& other) const;
  bool operator<(const Date& other) const;
  bool operator>(const Date& other) const;
  bool operator<=(const Date& other) const;
  bool operator>=(const Date& other) const;
  Date operator+(int days) const;
  Date operator-(int days) const;
  Date& operator+=(int days);
  Date& operator-=(int days);
  int operator-(const Date& other) const;
  string ToString() const;
  ~Date();
};

class Time {
 private:
  static const int kHoursInDay = 24;
  static const int kMinutesInHour = 60;

 public:
  Date date;
  int hour, minute;
  Time(int _hour = 0, int _minute = 0, const Date& _day = Date());
  Time(const string& input, const Date& _day = Date());
  Time(const Time& other);
  Time& operator=(const Time& other);
  bool operator==(const Time& other) const;
  bool operator!=(const Time& other) const;
  bool operator<(const Time& other) const;
  bool operator>(const Time& other) const;
  bool operator<=(const Time& other) const;
  bool operator>=(const Time& other) const;
  Time operator+(int minutes) const;
  Time operator-(int minutes) const;
  Time& operator+=(int minutes);
  Time& operator-=(int minutes);
  int operator-(const Time& other) const;
  string ToString() const;
  friend ostream& operator<<(ostream& os, const Time& time);
  ~Time();
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_TOOLS_TIMES_H