#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_STATION_PASS_CPP
#define TRAINTICKETSYSTEM_2022_SRC_DATA_STATION_PASS_CPP
#include "station_pass.h"
namespace sjtu {
StationPass::StationPass() = default;
StationPass::StationPass(const TrainID& _train_id, int _idx) : train_id(_train_id), idx(_idx) {}
StationPass::StationPass(const StationPass& other) = default;
StationPass::~StationPass() = default;
TrainID StationPassHash::operator()(const StationPass& obj) const { return obj.train_id; }
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_STATION_PASS_CPP