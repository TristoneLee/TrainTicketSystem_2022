#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_TICKET_SYSTEM_CPP
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_TICKET_SYSTEM_CPP
#include "train_ticket_system.h"

#include <iostream>

#include "sort.hpp"
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
#include "asserts.h"
#include "cmd_scanner.h"
#include "split.h"

namespace sjtu {
void TrainTicketSystem::JudgeOnline(const Command& cmd) {
  if (cmd.op == "query_profile" || cmd.op == "modify_profile")
    Assert(user_system.IsOnline(cmd.args['c']), "user is not online");
  if (cmd.op == "buy_ticket" || cmd.op == "query_order" || cmd.op == "refund_ticket")
    Assert(user_system.IsOnline(cmd.args['u']), "user is not online");
}
bool TrainTicketSystem::CheckFirst() {
  fstream temp_file(".isfirst", fstream::in | fstream::out | fstream::binary);
  if (!temp_file) {
    // create file
    fstream temp_create_file(".isfirst", fstream::out | fstream::binary);
    temp_file.close();
    return true;
  }
  temp_file.close();
  return false;
}
void TrainTicketSystem::PrintTrip(const Trip& trip, const string& depart_station, const string& arrive_station) {
  cout << trip.train_id << ' ';
  cout << depart_station << ' ';
  cout << trip.depart_time << ' ';
  cout << "-> ";
  cout << arrive_station << ' ';
  cout << trip.arrive_time << ' ';
  cout << trip.cost << ' ';
  Seats seat = ticket_system.QuerySeats(trip.train_id, trip.train_idx);
  cout << seat.QuerySeat(trip.depart_idx, trip.arrive_idx) << endl;
}
TrainTicketSystem::TrainTicketSystem() {}
void TrainTicketSystem::Work() {
  while (true) {
    try {
      // judge privilege inside the TrainTicketSystem
      // judge other availablity outsides
      Command now_cmd(std::cin);
      cout << '[' << now_cmd.timestamp << ']' << ' ';
      if (now_cmd.op == "exit") {
        cout << "bye" << endl;
        break;
      }
      JudgeOnline(now_cmd);
      if (now_cmd.op == "add_user") {
        // check privilege
        if (CheckFirst()) {
          now_cmd.args['g'] = "10";
        } else {
          Assert(user_system.IsOnline(now_cmd.args['c']) &&
                     user_system.QueryProfile(now_cmd.args['c']).privilege > ToInt(now_cmd.args['g']),
                 "add_user fail: privilege not enough");
        }
        const string& username = now_cmd.args['u'];
        const string& password = now_cmd.args['p'];
        const string& name = now_cmd.args['n'];
        const string& mail_addr = now_cmd.args['m'];
        const int privilege = ToInt(now_cmd.args['g']);
        user_system.AddUser(username, name, password, mail_addr, privilege);
        cout << '0' << endl;
      }
      if (now_cmd.op == "login") {
        const string& username = now_cmd.args['u'];
        const string& password = now_cmd.args['p'];
        user_system.Login(username, password);
        cout << '0' << endl;
      }
      if (now_cmd.op == "logout") {
        const string& username = now_cmd.args['u'];
        user_system.Logout(username);
        cout << '0' << endl;
      }
      if (now_cmd.op == "query_profile") {
        User user = user_system.QueryProfile(now_cmd.args['u']);
        User cur_user = user_system.QueryProfile(now_cmd.args['c']);
        Assert(cur_user.privilege > user.privilege || cur_user.username == user.username,
               "query_profile fail: privilege not enough");
        cout << user << endl;
      }
      if (now_cmd.op == "modify_profile") {
        User ori_user = user_system.QueryProfile(now_cmd.args['u']);
        User cur_user = user_system.QueryProfile(now_cmd.args['c']);
        Assert(cur_user.privilege > ori_user.privilege || cur_user.username == ori_user.username,
               "modify_profile fail: privilege not enough");
        User new_user(ori_user);
        if (now_cmd.args.count('p')) new_user.password = now_cmd.args['p'];
        if (now_cmd.args.count('n')) new_user.name = now_cmd.args['n'];
        if (now_cmd.args.count('m')) new_user.mail_addr = now_cmd.args['m'];
        if (now_cmd.args.count('g')) {
          Assert(cur_user.privilege > ToInt(now_cmd.args['g']), "modify_profile fail: target privilege too high");
          new_user.privilege = ToInt(now_cmd.args['g']);
        }
        user_system.ModifyProfile(ori_user, new_user);
        cout << new_user << endl;
      }
      if (now_cmd.op == "add_train") {
        const string& train_id = now_cmd.args['i'];
        const int station_num = ToInt(now_cmd.args['n']);
        vector<string> station_list = SplitIntoStrings(now_cmd.args['s']);
        const int seat_num = ToInt(now_cmd.args['m']);
        vector<int> price_list = SplitIntoInts(now_cmd.args['p']);
        vector<int> travel_time_list = SplitIntoInts(now_cmd.args['t']);
        vector<int> stopover_time_list = SplitIntoInts(now_cmd.args['o']);
        vector<Date> sale_date = SplitIntoDates(now_cmd.args['d']);
        Time start_time(now_cmd.args['x'], sale_date[0]);
        const char type = now_cmd.args['y'][0];
        train_system.AddTrain(train_id, station_num, station_list, seat_num, price_list, start_time, travel_time_list,
                              stopover_time_list, sale_date, type);
        cout << '0' << endl;
      }
      if (now_cmd.op == "delete_train") {
        const string& train_id = now_cmd.args['i'];
        train_system.DeleteTrain(train_id);
        cout << '0' << endl;
      }
      if (now_cmd.op == "release_train") {
        const string& train_id = now_cmd.args['i'];
        Train train = train_system.ReleaseTrain(train_id);
        ticket_system.ReleaseSeats(train_id, train.seat_num, train.sale_duration);
        cout << '0' << endl;
      }
      if (now_cmd.op == "query_train") {
        const string& train_id = now_cmd.args['i'];
        Date date(now_cmd.args['d']);
        Train train = train_system.QueryTrain(train_id);
        int idx = train.sale_date - date;
        Assert(idx >= 0 && idx < train.sale_duration, "query_train fail: date not in sale_date");
        Seats seat = ticket_system.QuerySeats(train_id, idx);
        cout << train_id << train.type << endl;
        cout << "xx-xx xx:xx"
             << " -> " << train.LeavingTime(0, idx) << ' ';
        cout << 0 << ' ';
        cout << seat.QuerySeat(0, 1) << endl;
        for (int station_idx = 1; station_idx < train.station_num - 1; ++station_idx) {
          cout << train.ArrivingTime(station_idx, idx) << " -> " << train.LeavingTime(station_idx, idx) << ' ';
          cout << train.price_prefix[station_idx] << ' ';
          cout << seat.QuerySeat(station_idx, station_idx + 1) << endl;
        }
        // output the terminal station
        cout << train.ArrivingTime(train.station_num - 1, idx) << " -> "
             << "xx-xx xx:xx" << ' ';
        cout << train.price_prefix[train.station_num - 1] << ' ';
        cout << 'x' << endl;
      }
      if (now_cmd.op == "query_ticket") {
        const string& depart_station = now_cmd.args['s'];
        const string& arrive_station = now_cmd.args['t'];
        Date date(now_cmd.args['d']);
        bool sort_by_time = true;
        if (now_cmd.args.count('p') && now_cmd.args['p'] == "cost") sort_by_time = false;
        auto trip_list = train_system.QueryTicket(depart_station, arrive_station, date);
        if (sort_by_time)
          Sort<Trip, TripCmpByTime>(trip_list);
        else
          Sort<Trip, TripCmpByCost>(trip_list);
        cout << trip_list.size() << endl;
        for (const auto& trip : trip_list) PrintTrip(trip, depart_station, arrive_station);
      }
      if (now_cmd.op == "query_transfer") {
        const string& depart_station = now_cmd.args['s'];
        const string& arrive_station = now_cmd.args['t'];
        Date date(now_cmd.args['d']);
        bool sort_by_time = true;
        if (now_cmd.args.count('p') && now_cmd.args['p'] == "cost") sort_by_time = false;
        auto transfer_trip = train_system.QueryTransfer(depart_station, arrive_station, date, sort_by_time);
        PrintTrip(transfer_trip.trips[0], depart_station, transfer_trip.transfer_station);
        PrintTrip(transfer_trip.trips[1], transfer_trip.transfer_station, arrive_station);
      }
      if (now_cmd.op == "buy_ticket") {
        const string& train_id = now_cmd.args['i'];
        const string& user_name = now_cmd.args['u'];
        Train train = train_system.QueryTrain(train_id);
        Assert(train.released, "buy_ticket fail: train not released");
        Date date(now_cmd.args['d']);
        const string& dep_station = now_cmd.args['f'];
        const string& arr_station = now_cmd.args['t'];
        int dep_idx = train.FindStation(dep_station);
        int arr_idx = train.FindStation(arr_station);
        int buy_num = ToInt(now_cmd.args['n']);
        Assert(buy_num < train.seat_num, "buy_ticket fail: not enough seats");
        int train_idx = train.FindLeavingTrain(dep_idx, date);
        Assert(train_idx != Train::NIDX, "buy_ticket fail: no train leaving on this date");
        Time dep_time = train.LeavingTime(dep_idx, train_idx);
        Time arr_time = train.ArrivingTime(arr_idx, train_idx);
        int price = train.GetPrice(dep_idx, arr_idx);
        bool accept_queue = false;
        if (now_cmd.args.count('q') && now_cmd.args['q'] == "true") accept_queue = true;
        bool stat = ticket_system.BuyTicket(user_name, train_id, train_idx, dep_idx, dep_station, dep_time, arr_idx,
                                            arr_station, arr_time, price, buy_num, accept_queue, now_cmd.timestamp);
        if (stat)
          cout << price * buy_num << endl;
        else {
          if (accept_queue)
            cout << "queue" << endl;
          else
            cout << "-1" << endl;
        }
      }
      if (now_cmd.op == "query_order") {
        const string& username = now_cmd.args['u'];
        ticket_system.QueryOrders(username);
      }
      if (now_cmd.op == "refund_ticket") {
        const string& username = now_cmd.args['u'];
        int idx = 1;
        if (now_cmd.args.count('n')) idx = ToInt(now_cmd.args['n']);
        ticket_system.RefundTicket(username, idx);
        cout << '0' << endl;
      }
      if (now_cmd.op == "rollback") {
        // TODO
      }
      if (now_cmd.op == "clean") {
        // TODO
      }
    } catch (const char* msg) {
      cout << "-1" << endl;
      cerr << msg << endl;
    } catch (...) {
      cout << "-1" << endl;
      cerr << "unknown error" << endl;
    }
    // cout << now_cmd.op << endl;
    // cout << "arglist:" << endl;
    // for (const auto& x : now_cmd.args) {
    //   cout << x.first << " " << x.second << endl;
    // }
    // cout << endl;
  }
}
TrainTicketSystem::~TrainTicketSystem() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TRAIN_TICKET_SYSTEM_CPP