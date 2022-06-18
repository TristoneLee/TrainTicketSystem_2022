#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_INDEX_H
#define TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_INDEX_H
#include "str.hpp"
namespace sjtu {
class TrainIndex {
 public:
  TrainID train_id;
  int idx;
  TrainIndex();
  TrainIndex(const TrainID& _train_id, int _idx);
  TrainIndex(const TrainIndex& other);
  TrainIndex& operator=(const TrainIndex& other);
  bool operator<(const TrainIndex& other) const;
  bool operator==(const TrainIndex& other) const;
  ~TrainIndex();
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_TRAIN_INDEX_H