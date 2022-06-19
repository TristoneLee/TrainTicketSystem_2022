#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_TICKET_SYSTEM_H
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_TICKET_SYSTEM_H
#include "cmd_scanner.h"
#include "ticket_system.h"
#include "train_system.h"
#include "user_system.h"
namespace sjtu {
class TrainTicketSystem {
 private:
  UserSystem user_system;
  TrainSystem train_system;
  TicketSystem ticket_system;
  void JudgeOnline(const Command& cmd);  // dont check add_user for special reasons
  bool CheckFirst();
  void PrintTrip(const Trip& trip, const string& depart_station, const string& arrive_station);

 public:
  TrainTicketSystem();
  ~TrainTicketSystem();
  void Work();
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_TICKET_SYSTEM_H