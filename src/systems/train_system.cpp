#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_SYSTEM_CPP
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_SYSTEM_CPP
#include "train_system.h"

#include "asserts.h"
#include "linked_hashmap.hpp"
#include "trip.h"
namespace sjtu {
TrainSystem::TrainSystem() : trains_("TrainSystem_train_"), station_passes_("TrainSystem_station_passes_") {}
void TrainSystem::AddTrain(const string& train_id, int station_num, const vector<string>& station_list, int _seat_num,
                           const vector<int>& price_list, Time start_time, const vector<int>& travel_time_list,
                           const vector<int>& stopover_time_list, const vector<Date>& sale_date, char type) {
  Assert(trains_.query(train_id).empty(), "add_train fail : train_id already exists");
  Train train(train_id, station_num, station_list, _seat_num, price_list, start_time, travel_time_list,
              stopover_time_list, sale_date, type);
  trains_.insert(train_id, train);
}
void TrainSystem::DeleteTrain(const string& train_id) {
  auto find_result = trains_.query(train_id);
  Assert(!find_result.empty(), "delete_train fail : train_id not exists");
  Train train = find_result.front();
  Assert(!train.released, "delete_train fail : train is already released");
  trains_.erase(train_id, train);
  return;
}
Train TrainSystem::ReleaseTrain(const string& train_id) {
  auto find_result = trains_.query(train_id);
  Assert(!find_result.empty(), "release_train fail : train does not exists");
  Train train = find_result.front();
  Assert(!train.released, "release_train fail : train is already released");
  trains_.erase(train_id, train);
  train.released = true;
  trains_.insert(train_id, train);
  // add the train to all the stations it passes
  for (int idx = 0; idx < train.station_num; ++idx) {
    const Station& now_station = train.station_list[idx];
    StationPass station_pass(train_id, idx);
    station_passes_.insert(now_station, station_pass);
  }
  return train;
}
Train TrainSystem::QueryTrain(const string& train_id) {
  auto find_result = trains_.query(train_id);
  Assert(!find_result.empty(), "query_train fail : train does not exists");
  return find_result.front();
}
vector<Trip> TrainSystem::QueryTicket(const string& depart_station, const string& arrive_station, Date date) {
  auto depart_station_pass = station_passes_.query(depart_station);
  auto arrive_station_pass = station_passes_.query(arrive_station);
  int idx_dep = 0, idx_arr = 0;
  int lst_dep = depart_station_pass.size(), lst_arr = arrive_station_pass.size();
  vector<Trip> ret;
  for (; idx_dep < lst_dep && idx_arr < lst_arr;) {
    if (depart_station_pass[idx_dep].train_id < arrive_station_pass[idx_arr].train_id) {
      ++idx_dep;
      continue;
    }
    if (depart_station_pass[idx_dep].train_id > arrive_station_pass[idx_arr].train_id) {
      ++idx_arr;
      continue;
    }
    // find a train that passes both stations
    if (depart_station_pass[idx_dep].idx < arrive_station_pass[idx_arr].idx) {
      // the train passes the depart station first
      Train train = trains_.query(depart_station_pass[idx_dep].train_id).front();
      const int& depart_station_idx = depart_station_pass[idx_dep].idx;
      const int& arrive_station_idx = arrive_station_pass[idx_arr].idx;
      int train_idx = train.FindLeavingTrain(depart_station_idx, date);
      if (train_idx != Train::NIDX) {
        // the train is on sale on date
        Time depart_time = train.LeavingTime(depart_station_idx, train_idx);
        Time arrive_time = train.ArrivingTime(arrive_station_idx, train_idx);
        int cost = train.GetPrice(depart_station_idx, arrive_station_idx);
        ret.push_back(
            Trip(train.train_id, depart_time, arrive_time, train_idx, depart_station_idx, arrive_station_idx, cost));
      }
    }
    ++idx_dep;
    ++idx_arr;
  }
  return ret;
}
TransferTrip TrainSystem::QueryTransfer(const string& depart_station, const string& arrive_station, Date date,
                                        bool SortByTime) {
  auto depart_station_pass = station_passes_.query(depart_station);
  auto arrive_station_pass = station_passes_.query(arrive_station);
  int idx_dep = 0, idx_arr = 0;
  int lst_dep = depart_station_pass.size(), lst_arr = arrive_station_pass.size();
  TransferTrip ret;
  bool found = false;
  static const TransferTripCmpByCost cmp0;
  static const TransferTripCmpByTime cmp1;
  auto update_ret = [&](const TransferTrip& now_ans) {
    if (!found) {
      ret = now_ans;
      found = 1;
      return;
    }
    if (SortByTime ? cmp1(now_ans, ret) : cmp0(now_ans, ret))  // now_ans < ret
      ret = now_ans;
    return;
  };
  for (; idx_dep < lst_dep; idx_dep++) {
    Train dep_train = trains_.query(depart_station_pass[idx_dep].train_id).front();
    const int& depart_station_idx = depart_station_pass[idx_dep].idx;
    // the depart station is the terminal station of dep_train
    if (depart_station_idx == dep_train.station_num - 1) continue;
    // can be used to find whether the station is in the train and where it is
    linked_hashmap<Station, int, StationHash> station_idx_map;
    for (int i = depart_station_idx + 1; i < dep_train.station_num; ++i) station_idx_map[dep_train.station_list[i]] = i;
    int dep_train_idx = dep_train.FindLeavingTrain(depart_station_idx, date);
    if (dep_train_idx == Train::NIDX) continue;  // train is not on sale on date
    Time dep_train_leaving_time = dep_train.LeavingTime(depart_station_idx, dep_train_idx);
    for (idx_arr = 0; idx_arr < lst_arr; idx_arr++) {
      // the arr_train is the same as the dep_train
      if (arrive_station_pass[idx_arr].train_id == dep_train.train_id) continue;
      Train arr_train = trains_.query(arrive_station_pass[idx_arr].train_id).front();
      const int& arrive_station_idx = arrive_station_pass[idx_arr].idx;
      for (int i = 0; i < arrive_station_idx; ++i)
        if (station_idx_map.count(arr_train.station_list[i])) {
          // dep_train and arr_train crosses
          const Station& transfer_station = arr_train.station_list[i];
          const int& transfer_station_idx_in_dep = station_idx_map[transfer_station];
          const int& transfer_station_idx_in_arr = i;
          Time dep_train_arriving_time = dep_train.ArrivingTime(transfer_station_idx_in_dep, dep_train_idx);
          int arr_train_idx = arr_train.FindLeavingTrainAfter(transfer_station_idx_in_arr, dep_train_arriving_time);
          if (arr_train_idx != Train::NIDX) {
            Time arr_train_leaving_time = arr_train.LeavingTime(transfer_station_idx_in_arr, arr_train_idx);
            Time arr_train_arriving_time = arr_train.ArrivingTime(arrive_station_idx, arr_train_idx);
            Trip dep_trip(dep_train.train_id, dep_train_leaving_time, dep_train_arriving_time, dep_train_idx,
                          depart_station_idx, transfer_station_idx_in_dep,
                          dep_train.GetPrice(depart_station_idx, transfer_station_idx_in_dep));
            Trip arr_trip(arr_train.train_id, arr_train_leaving_time, arr_train_arriving_time, arr_train_idx,
                          transfer_station_idx_in_arr, arrive_station_idx,
                          arr_train.GetPrice(transfer_station_idx_in_arr, arrive_station_idx));
            TransferTrip now_ans(dep_trip, arr_trip, transfer_station);
            update_ret(now_ans);
          }
        }
    }
  }
  return ret;
}
void TrainSystem::Clear() {
  trains_.clear();
  station_passes_.clear();
}
TrainSystem::~TrainSystem() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_SYSTEM_CPP