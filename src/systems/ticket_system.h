#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_H
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_H
#include "database.h"
#include "order.h"
#include "seats.h"
#include "train_index.h"
namespace sjtu {
class TicketSystem {
 private:
  bpTree<TrainIndex, Seats> seats_;
  bpTree<TrainIndex, Order, int, OrderHash, std::less<TrainIndex>, std::greater<int>> train_orders_;
  bpTree<UserName, OrderIter, int, OrderIterHash, std::less<UserName>, std::greater<int>> user_orders_;

 public:
  TicketSystem();
  void ReleaseSeats(const TrainID& train_id, int total_seat, int sale_duration);
  Seats QuerySeats(const TrainID& train_id, int idx);
  bool BuyTicket(const UserName& username, const TrainID& train_id, int idx, int dep_idx, const Station& dep_station,
                 const Time& dep_time, int arr_idx, const Station& arr_station, const Time& arr_time, int price,
                 int num, bool accept_queue, int timestamp);
  void QueryOrders(const UserName& username);
  void RefundTicket(const UserName& username, int order_idx);
  ~TicketSystem();
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_H