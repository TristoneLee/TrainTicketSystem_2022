#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_SEATS_CPP
#define TRAINTICKETSYSTEM_2022_SRC_DATA_SEATS_CPP
#include "seats.h"

#include <iostream>

#include "asserts.h"
using std::min;
namespace sjtu {
Seats::Seats(int seat_number) {
  for (int i = 0; i < kMaxStationNum; i++) remain_seats[i] = seat_number;
}
Seats::Seats(const Seats& other) = default;
void Seats::Modify(int depart_idx, int arrive_idx, int num) {
  Assert(0 <= depart_idx && depart_idx < arrive_idx, "query_seat fail : range error");
  for (int i = depart_idx; i < arrive_idx; i++) remain_seats[i] += num;
  return;
}
int Seats::QuerySeat(int depart_idx, int arrive_idx) const {
  Assert(0 <= depart_idx && depart_idx < arrive_idx, "query_seat fail : range error");
  int ret = 100000;
  for (int i = depart_idx; i < arrive_idx; i++) ret = min(ret, remain_seats[i]);
  return ret;
}
void Seats::BuyTicket(int depart_idx, int arrive_idx, int num) { Modify(depart_idx, arrive_idx, -num); }
void Seats::RefundTicket(int depart_idx, int arrive_idx, int num) { Modify(depart_idx, arrive_idx, num); }
Seats::~Seats() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_SEATS_CPP