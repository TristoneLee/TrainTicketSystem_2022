#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_CPP
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_CPP
#include "ticket_system.h"

#include "asserts.h"
using std::endl;
namespace sjtu {
TicketSystem::TicketSystem()
    : seats_("TicketSystem_seats_"),
      train_orders_("TicketSystem_train_orders_"),
      user_orders_("TicketSystem_user_orders_") {}
void TicketSystem::ReleaseSeats(const TrainID& train_id, int total_seat, int sale_duration) {
  TrainIndex train_idx(train_id, 0);
  Seats seat(total_seat);
  for (int idx = 0; idx < sale_duration; ++idx) {
    train_idx.idx = idx;
    seats_.insert(train_idx, seat, timestamp_);
  }
  return;
}
Seats TicketSystem::QuerySeats(const TrainID& train_id, int idx) {
  TrainIndex train_idx(train_id, idx);
  return seats_.query(train_idx).front();
}
bool TicketSystem::BuyTicket(const UserName& username, const TrainID& train_id, int idx, int dep_idx,
                             const Station& dep_station, const Time& dep_time, int arr_idx, const Station& arr_station,
                             const Time& arr_time, int price, int num, bool accept_queue, int timestamp) {
  TrainIndex train_idx(train_id, idx);
  auto seats_iter = seats_.find(train_idx);
  Seats seats = (*seats_iter).valueOf();
  Order order(train_id, idx, dep_station, dep_idx, dep_time, arr_station, arr_idx, arr_time, price, num, timestamp);
  bool ret;
  if (seats.QuerySeat(dep_idx, arr_idx) >= num) {
    seats.BuyTicket(dep_idx, arr_idx, num);
    seats_.valueUpdate(seats_iter, seats, timestamp_);
    ret = true;
    // if (train_id.ToString() == "LeavesofGrass" && idx == 30) {
    //   std::cerr << "detected!" << endl;
    //   seats_iter = seats_.find(train_idx);
    //   seats_iter.fuck();
    //   std::cerr << '[' << (*seats_iter).keyOf().train_id << ',' << (*seats_iter).keyOf().idx << ']' << endl;
    //   for (int i = 0; i <= 16; ++i) std::cerr << seats.remain_seats[i] << '\t';
    //   std::cerr << endl;
    // }
  } else {
    if (!accept_queue) return false;
    order.status = Order::kPending;
    ret = false;
  }
  int pos = train_orders_.insert(train_idx, order, timestamp_);
  OrderIter order_iter(pos, timestamp);
  user_orders_.insert(username, order_iter, timestamp_);
  return ret;
}
void TicketSystem::QueryOrders(const UserName& username) {
  auto order_iters = user_orders_.query(username);
  cout << order_iters.size() << endl;
  for (int i = 0; i < order_iters.size(); ++i) {
    OrderIter order_iter = order_iters[i];
    cout << train_orders_.dirRead(order_iter.pos) << endl;
  }
  return;
}
Order TicketSystem::RefundTicket(const UserName& username, int order_idx) {
  auto user_orders = user_orders_.query(username);
  Assert(user_orders.size() >= order_idx, "refund fail: order index out of range");
  OrderIter refunded_order_pos = user_orders[order_idx - 1];
  Order refunded_order = train_orders_.dirRead(refunded_order_pos.pos);
  // if (refunded_order.train_id.ToString() == "LeavesofGrass" && refunded_order.train_idx == 30) {
  //   std::cerr << "ddetected!" << endl;
  //   // std::cerr << endl;
  // }
  Assert(refunded_order.status != Order::kRefunded, "refund fail: order has already been refunded");
  bool refund_order_success = (refunded_order.status == Order::kSuccess);
  refunded_order.status = Order::kRefunded;
  const TrainID& train_id = refunded_order.train_id;
  int train_idx = refunded_order.train_idx;
  TrainIndex train_index(train_id, train_idx);
  auto refunded_order_iter = train_orders_.pairFind(train_index, refunded_order);
  train_orders_.valueUpdate(refunded_order_iter, refunded_order, timestamp_);
  if (!refund_order_success) return Order();
  auto seats_iter = seats_.find(train_index);
  Seats seats = (*seats_iter).valueOf();
  seats.RefundTicket(refunded_order.dep_idx, refunded_order.arr_idx, refunded_order.num);
  for (auto influenced_order_iter = train_orders_.find(train_index);
       !influenced_order_iter.ifEnd() && (*influenced_order_iter).keyOf() == train_index; ++influenced_order_iter) {
    Order influenced_order = (*influenced_order_iter).valueOf();
    // std::cerr << "judging: " << influenced_order << std::endl;
    if (influenced_order.status != Order::kPending) continue;
    int now_dep_idx = influenced_order.dep_idx;
    int now_arr_idx = influenced_order.arr_idx;
    int now_num = influenced_order.num;
    if (seats.QuerySeat(now_dep_idx, now_arr_idx) < now_num) continue;
    seats.BuyTicket(now_dep_idx, now_arr_idx, now_num);
    influenced_order.status = Order::kSuccess;
    train_orders_.valueUpdate(influenced_order_iter, influenced_order, timestamp_);
    // std::cerr << "Refunded: " << influenced_order << std::endl;
    // std::cerr << "now seats: " << seats.QuerySeat(now_dep_idx, now_arr_idx) << std::endl;
  }
  seats_.valueUpdate(seats_iter, seats, timestamp_);
  return refunded_order;
}
void TicketSystem::Clear() {
  seats_.clear();
  train_orders_.clear();
  user_orders_.clear();
}
void TicketSystem::ModifyTimestamp(int timestamp) { timestamp_ = timestamp; }
void TicketSystem::Rollback(int timestamp){
  seats_.roll(timestamp);
  train_orders_.roll(timestamp);
  user_orders_.roll(timestamp);
}
TicketSystem::~TicketSystem() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_CPP