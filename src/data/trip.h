#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_H
#define TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_H
#include "str.hpp"
namespace sjtu {
class Trip {
 public:
  TrainID train_id;
  int depart_idx;
  int arrive_idx;
  int cost;
  int time;
  Trip();
  Trip(const TrainID& _train_id, int _depart_idx, int _arrive_idx, int _price, int _time);
  Trip(const Trip& other);
  Trip& operator=(const Trip& other);
  ~Trip();
};

class TripCmpByTime {
 public:
  bool operator()(const Trip& lhs, const Trip& rhs) const;
};

class TripCmpByCost {
 public:
  bool operator()(const Trip& lhs, const Trip& rhs) const;
};

}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_H