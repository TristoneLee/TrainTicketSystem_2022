#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_CPP
#define TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_CPP
#include "trip.h"
namespace sjtu {
Trip::Trip() = default;
Trip::Trip(const TrainID& _train_id, const Time& _depart_time, const Time& _arrive_time, int _train_idx,
           int _depart_idx, int _arrive_idx, int _cost)
    : train_id(_train_id),
      depart_time(_depart_time),
      arrive_time(_arrive_time),
      train_idx(_train_idx),
      depart_idx(_depart_idx),
      arrive_idx(_arrive_idx),
      cost(_cost),
      time(_arrive_time - _depart_time) {}
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
TransferTrip::TransferTrip() = default;
TransferTrip::TransferTrip(const Trip& _trip1, const Trip& _trip2, const Station& _transfer_station)
    : trips{_trip1, _trip2},
      transfer_station(_transfer_station),
      total_time(trips[1].arrive_time - trips[0].depart_time),
      total_cost(trips[0].cost + trips[1].cost) {}
TransferTrip::TransferTrip(const TransferTrip& other) = default;
TransferTrip& TransferTrip::operator=(const TransferTrip& other) = default;
TransferTrip::~TransferTrip() = default;
bool TransferTripCmpByTime::operator()(const TransferTrip& lhs, const TransferTrip& rhs) const {
  if (lhs.total_time != rhs.total_time) return lhs.total_time < rhs.total_time;
  if (lhs.total_cost != rhs.total_cost) return lhs.total_cost < rhs.total_cost;
  if (lhs.trips[0].train_id != rhs.trips[0].train_id) return lhs.trips[0].train_id < rhs.trips[0].train_id;
  return lhs.trips[1].train_id < rhs.trips[1].train_id;
}
bool TransferTripCmpByCost::operator()(const TransferTrip& lhs, const TransferTrip& rhs) const {
  if (lhs.total_cost != rhs.total_cost) return lhs.total_cost < rhs.total_cost;
  if (lhs.total_time != rhs.total_time) return lhs.total_time < rhs.total_time;
  if (lhs.trips[0].train_id != rhs.trips[0].train_id) return lhs.trips[0].train_id < rhs.trips[0].train_id;
  return lhs.trips[1].train_id < rhs.trips[1].train_id;
}
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_TRIP_CPP