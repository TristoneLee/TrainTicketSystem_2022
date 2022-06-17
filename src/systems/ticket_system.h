#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_H
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_H
#include "BPTree.h"
#include "seats.h"
#include "train_index.h"
namespace sjtu {
class TicketSystem {
 private:
  FileMap<TrainIndex, Seats> seats_;

 public:
  TicketSystem();
  void ReleaseSeats(const TrainID& train_id, int total_seat, int sale_duration);
  Seats QuerySeats(const TrainID& train_id, int idx);
  ~TicketSystem();
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_H