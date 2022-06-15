#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_H
#define TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_H
#include "const_value.h"
#include "str.hpp"
#include "times.h"
#include "vector.hpp"
namespace sjtu {
class Train {
 public:
  TrainID train_id;
  int station_num;
  Station station_list[kMaxStationNum];
  int seat_num;
  int remain_seat_num[kMaxStationNum];
  int price_list[kMaxStationNum];
  Time start_time;
  int travel_time_list[kMaxStationNum];
  int stopover_time_list[kMaxStationNum];
  Date sale_date;
  int sale_duration;
  bool released;
  char type;
  Train();
  Train(const string& _train_id, int _station_num, const vector<string>& _station_list, int _seat_num,
        const vector<int>& _price_list, Time _start_time, const vector<int>& _travel_time_list,
        const vector<int>& _stopover_time_list, const vector<Date>& _sale_date, char _type, bool _released);
  Train& operator=(const Train& other);
  ~Train();
};
};      // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_H