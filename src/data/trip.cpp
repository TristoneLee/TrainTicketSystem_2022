#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_CPP
#define TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_CPP
#include "trip.h"
namespace sjtu {
Trip::Trip() = default;
Trip::Trip(const TrainID& _train_id, int _depart_idx, int _arrive_idx, int _price, int _time)
    : train_id(_train_id), depart_idx(_depart_idx), arrive_idx(_arrive_idx), cost(_price), time(_time) {}
Trip::Trip(const Trip& other) = default;
Trip& Trip::operator=(const Trip& other) = default;
Trip::~Trip() = default;
bool TripCmpByTime::operator()(const Trip& lhs, const Trip& rhs) const {
  if (lhs.time != rhs.time) return lhs.time < rhs.time;
  return lhs.train_id < rhs.train_id;
}
bool TripCmpByCost::operator()(const Trip& lhs, const Trip& rhs) const {
  if (lhs.cost != rhs.cost) return lhs.cost < rhs.cost;
  return lhs.train_id < rhs.train_id;
}
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_CPP