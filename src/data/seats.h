#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_SEATS_H
#define TRAINTICKETSYSTEM_2022_SRC_DATA_SEATS_H
#include "const_value.h"
namespace sjtu {
class Seats {
 private:
  void Modify(int depart_idx, int arrive_idx, int num);

 public:
  int remain_seats[kMaxStationNum];
  Seats(int seat_number = 0);
  Seats(const Seats& other);
  int QuerySeat(int depart_idx, int arrive_idx) const;
  void BuyTicket(int depart_idx, int arrive_idx, int num);
  void RefundTicket(int depart_idx, int arrive_idx, int num);
  ~Seats();
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_SEATS_H