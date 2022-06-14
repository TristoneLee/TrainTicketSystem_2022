#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_CPP
#define TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_CPP
#include "train.h"

#include "const_value.h"
#include "str.hpp"
#include "times.h"
#include "vector.hpp"
namespace sjtu {
Train::Train(const string& _train_id, int _station_num,
             const vector<string>& _station_list, int _seat_num,
             const vector<int>& _price_list, Time _start_time,
             const vector<int>& _travel_time_list,
             const vector<int>& _stopover_time_list,
             const vector<Date>& _sale_date, char _type, bool _released)
    : train_id(_train_id),
      station_num(_station_num),
      seat_num(_seat_num),
      sale_date(_sale_date[0]),
      sale_duration(_sale_date[1] - _sale_date[0] + 1),
      type(_type),
      released(_released) {
  for (int i = 0; i < station_num - 2; ++i) {
    station_list[i] = _station_list[i];
    price_list[i] = _price_list[i];
    travel_time_list[i] = _travel_time_list[i];
    stopover_time_list[i] = _stopover_time_list[i];
  }
  station_list[station_num - 2] = _station_list[station_num - 2];
  station_list[station_num - 1] = _station_list[station_num - 1];
  price_list[station_num - 2] = _price_list[station_num - 2];
  travel_time_list[station_num - 2] = _travel_time_list[station_num - 2];
  return;
}
};      // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_CPP