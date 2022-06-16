#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_TICKET_SYSTEM_H
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_TICKET_SYSTEM_H
#include "cmd_scanner.h"
#include "user_system.h"
namespace sjtu {
class TrainTicketSystem {
 private:
  UserSystem user_system;
  void JudgeOnline(const Command& cmd);  // dont check add_user for special reasons
  bool CheckFirst();

 public:
  TrainTicketSystem();
  ~TrainTicketSystem();
  void Work();
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_TICKET_SYSTEM_H