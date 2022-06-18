#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_ORDER_H
#define TRAINTICKETSYSTEM_2022_SRC_DATA_ORDER_H
#include "str.hpp"
#include "times.h"
namespace sjtu {
class Order {
 public:
  enum OrderStatus { kSuccess, kPending, kRefunded };
  OrderStatus status;
  TrainID train_id;
  Station depart_station;
  Time depart_time;
  Station arrive_station;
  Time arrive_time;
  int price;  // price per ticket
  int num;
  int time_stamp;
  Order();
  Order(const TrainID& _train_id, const Station& _depart_station, const Time& _depart_time,
        const Station& _arrive_station, const Time& _arrive_time, int _price, int _num, int _time_stamp,OrderStatus _status);
  Order(const Order& other);
  Order& operator=(const Order& other);
  friend ostream& operator<<(ostream& out, const Order& order);
  ~Order();
};

class OrderHash {
 public:
  size_t operator()(const Order& order) const;
};
class OrderIter {
 public:
  int pos;
  int time_stamp;
  OrderIter();
  OrderIter(int _pos, int _time_stamp);
  OrderIter(const OrderIter& other);
  OrderIter& operator=(const OrderIter& other);
  ~OrderIter();
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_ORDER_H