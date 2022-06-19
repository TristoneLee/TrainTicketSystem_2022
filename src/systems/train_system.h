#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_SYSTEM_H
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_SYSTEM_H
#include "database.h"
#include "station_pass.h"
#include "str.hpp"
#include "train.h"
#include "trip.h"
namespace sjtu {
class TrainSystem {
 private:
  bpTree<TrainID, Train> trains_;
  bpTree<Station, StationPass, TrainID, StationPassHash> station_passes_;

 public:
  TrainSystem();
  void AddTrain(const string& train_id, int station_num, const vector<string>& station_list, int _seat_num,
                const vector<int>& price_list, Time start_time, const vector<int>& travel_time_list,
                const vector<int>& stopover_time_list, const vector<Date>& sale_date, char type);
  void DeleteTrain(const string& train_id);
  Train ReleaseTrain(const string& train_id);
  Train QueryTrain(const string& train_id);
  vector<Trip> QueryTicket(const string& depart_station, const string& arrive_station, Date date);
  TransferTrip QueryTransfer(const string& depart_station, const string& arrive_station, Date date, bool SortByTime);
  void Clear();
  ~TrainSystem();
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_SYSTEM_H