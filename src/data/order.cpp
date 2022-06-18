#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_ORDER_CPP
#define TRAINTICKETSYSTEM_2022_SRC_DATA_ORDER_CPP
#include "order.h"
namespace sjtu {
Order::Order() = default;
Order::Order(const TrainID& _train_id, const Station& _depart_station, const Time& _depart_time,
             const Station& _arrive_station, const Time& _arrive_time, int _price, int _num, int _time_stamp,
             OrderStatus _status)
    : train_id(_train_id),
      depart_station(_depart_station),
      depart_time(_depart_time),
      arrive_station(_arrive_station),
      arrive_time(_arrive_time),
      price(_price),
      num(_num),
      time_stamp(_time_stamp),
      status(_status) {}
Order::Order(const Order& other) = default;
Order& Order::operator=(const Order& other) = default;
ostream& operator<<(ostream& os, const Order& order) {
  os << '[';
  if (order.status == Order::kSuccess) {
    os << "success";
  } else if (order.status == Order::kPending) {
    os << "pending";
  } else if (order.status == Order::kRefunded) {
    os << "refunded";
  }
  os << "] ";
  os << order.train_id << ' ';
  os << order.depart_station << ' ';
  os << order.depart_time << ' ';
  os << order.arrive_station << ' ';
  os << order.arrive_time << ' ';
  os << order.price << ' ';
  os << order.num;
  return os;
}
size_t OrderHash::operator()(const Order& order) const { return order.time_stamp; }
Order::~Order() = default;
OrderIter::OrderIter() = default;
OrderIter::OrderIter(int _pos, int _time_stamp) : pos(_pos), time_stamp(_time_stamp) {}
OrderIter::OrderIter(const OrderIter& other) = default;
OrderIter& OrderIter::operator=(const OrderIter& other) = default;
OrderIter::~OrderIter() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_ORDER_CPP