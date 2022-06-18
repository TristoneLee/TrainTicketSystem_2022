#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_INDEX_CPP
#define TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_INDEX_CPP
#include "train_index.h"
namespace sjtu {
TrainIndex::TrainIndex() = default;
TrainIndex::TrainIndex(const TrainID& _train_id, int _idx) : train_id(_train_id), idx(_idx) {}
TrainIndex::TrainIndex(const TrainIndex& other) = default;
TrainIndex& TrainIndex::operator=(const TrainIndex& other) = default;
bool TrainIndex::operator<(const TrainIndex& other) const {
  if (train_id != other.train_id) return train_id < other.train_id;
  return idx < other.idx;
}
bool TrainIndex::operator==(const TrainIndex& other) const { return train_id == other.train_id && idx == other.idx; }
TrainIndex::~TrainIndex() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_INDEX_CPP