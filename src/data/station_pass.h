#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_STATION_PASS_H
#define TRAINTICKETSYSTEM_2022_SRC_DATA_STATION_PASS_H
#include "str.hpp"
namespace sjtu {
class StationPass {
 public:
  TrainID train_id;
  int idx;  // index of station in train_id
  StationPass();
  StationPass(const TrainID& _train_id, int _idx);
  StationPass(const StationPass& other);
  ~StationPass();
};
class StationPassHash {
 public:
  TrainID operator()(const StationPass& obj) const;
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_STATION_PASS_H