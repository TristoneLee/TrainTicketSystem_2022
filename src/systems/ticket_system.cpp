#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_CPP
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_CPP
#include "ticket_system.h"
namespace sjtu {
TicketSystem::TicketSystem() : seats_("TicketSystem_seats_") {}
void TicketSystem::ReleaseSeats(const TrainID& train_id, int total_seat, int sale_duration) {
  TrainIndex train_idx(train_id, 0);
  Seats seat(total_seat);
  for (int idx = 0; idx < sale_duration; ++idx) {
    train_idx.idx = idx;
    seats_.Insert(train_idx, seat);
  }
  return;
}
Seats TicketSystem::QuerySeats(const TrainID& train_id, int idx) {
  TrainIndex train_idx(train_id, idx);
  return seats_.Find(train_idx).front();
}
TicketSystem::~TicketSystem() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_CPP