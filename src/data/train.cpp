#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_CPP
#define TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_CPP
#include "train.h"

#include "const_value.h"
#include "str.hpp"
#include "times.h"
#include "vector.hpp"
namespace sjtu {
Train::Train() = default;
Train::Train(const string& _train_id, int _station_num, const vector<string>& _station_list, int _seat_num,
             const vector<int>& _price_list, Time _start_time, const vector<int>& _travel_time_list,
             const vector<int>& _stopover_time_list, const vector<Date>& _sale_date, char _type, bool _released)
    : train_id(_train_id),
      station_num(_station_num),
      seat_num(_seat_num),
      start_time(_start_time),
      sale_date(_sale_date[0]),
      sale_duration(_sale_date[1] - _sale_date[0] + 1),
      type(_type),
      released(_released) {
  for (int i = 0; i < station_num; ++i) station_list[i] = _station_list[i];
  price_prefix[0] = 0;
  for (int i = 1; i < station_num; ++i) price_prefix[i] = price_prefix[i - 1] + _price_list[i - 1];
  arriving_time[0] = -1;  // meaningless
  leaving_time[0] = 0;    // the leaving time of the first station is start_time;
  for (int i = 1; i < station_num - 1; ++i) {
    arriving_time[i] = leaving_time[i - 1] + _travel_time_list[i - 1];
    leaving_time[i] = arriving_time[i] + _stopover_time_list[i - 1];
  }
  arriving_time[station_num - 1] = leaving_time[station_num - 2] + _travel_time_list[station_num - 2];
  return;
}
Train::Train(const Train& other) = default;
Train& Train::operator=(const Train& other) = default;
int Train::GetPrice(int depart_idx, int terminal_idx) const {
  return price_prefix[terminal_idx] - price_prefix[depart_idx];
}
int Train::FindStation(const Station& station_name) const {
  for (int idx; idx < station_num; ++idx)
    if (station_list[idx] == station_name) return idx;
  return NIDX;
}

Time Train::LeavingTime(int station_idx) const { return start_time + leaving_time[station_idx]; }
Time Train::ArrivingTime(int station_idx) const { return start_time + arriving_time[station_idx]; }
int Train::FindLeavingTrain(int station_idx, const Date& date) const {
  Time start_leaving_time = LeavingTime(station_idx);
  int ret = date - start_leaving_time.date;
  if (ret < 0 || ret > sale_duration) return NIDX;
  return ret;
}
int Train::GetTravelTime(int depart_idx, int arrive_idx) const {
  return arriving_time[arrive_idx] - leaving_time[depart_idx];
}
Train::~Train() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_CPP