#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_H
#define TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_H
#include "const_value.h"
#include "str.hpp"
#include "times.h"
#include "vector.hpp"
namespace sjtu {
class Train {
 public:
  static const int NIDX = -1;
  TrainID train_id;
  int station_num;
  Station station_list[kMaxStationNum];
  int seat_num;
  int price_prefix[kMaxStationNum];
  Time start_time;
  // arriving_time and leaving_time are based on start_time
  int arriving_time[kMaxStationNum];
  int leaving_time[kMaxStationNum];
  Date sale_date;
  int sale_duration;
  bool released;
  char type;
  Train();
  Train(const string& _train_id, int _station_num, const vector<string>& _station_list, int _seat_num,
        const vector<int>& _price_list, Time _start_time, const vector<int>& _travel_time_list,
        const vector<int>& _stopover_time_list, const vector<Date>& _sale_date, char _type, bool _released = false);
  Train(const Train& other);
  Train& operator=(const Train& other);
  int GetPrice(int depart_idx, int arrive_idx) const;
  int FindStation(const Station& station_name) const;  // return NIDX if not found
  Time LeavingTime(int station_idx, int idx = 0) const;
  Time ArrivingTime(int station_idx, int idx = 0) const;
  // return the index of the train leaving station_idx on date
  // return NIDX if not found
  int FindLeavingTrain(int station_idx, const Date& date) const;
  int FindLeavingTrainAfter(int station_idx, const Time& time) const;
  int GetTravelTime(int depart_idx, int arrive_idx) const;
  ~Train();
};
};      // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_H