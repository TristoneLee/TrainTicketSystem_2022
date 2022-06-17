#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_H
#define TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_H
#include "str.hpp"
#include "times.h"
namespace sjtu {
class Trip {
 public:
  TrainID train_id;
  Time depart_time;
  Time arrive_time;
  int depart_idx;
  int arrive_idx;
  int cost;
  int time;
  Trip();
  Trip(const TrainID& _train_id, Time _depart_time, Time _arrive_time, int _depart_idx, int _arrive_idx, int _cost);
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

class TransferTrip {
 public:
  Trip trips[2];
  Station transfer_station;
  int total_time;
  int total_cost;
  TransferTrip();
  TransferTrip(const Trip& _trip1, const Trip& _trip2, const Station& _transfer_station);
  TransferTrip(const TransferTrip& other);
  TransferTrip& operator=(const TransferTrip& other);
  ~TransferTrip();
};
class TransferTripCmpByTime {
 public:
  bool operator()(const TransferTrip& lhs, const TransferTrip& rhs) const;
};
class TransferTripCmpByCost {
 public:
  bool operator()(const TransferTrip& lhs, const TransferTrip& rhs) const;
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_H