#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_CPP
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_CPP
#include "ticket_system.h"

#include <iostream>
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
#include "asserts.h"
#include "cmd_scanner.h"
#include "split.h"

namespace sjtu {
void TicketSystem::JudgeOnline(const Command& cmd) {
  if (cmd.op == "query_profile" || cmd.op == "modify_profile")
    Assert(user_system.IsOnline(cmd.args['c']), "user is not online");
  if (cmd.op == "buy_ticket" || cmd.op == "query_order" || cmd.op == "refund_ticket")
    Assert(user_system.IsOnline(cmd.args['u']), "user is not online");
}
bool TicketSystem::CheckFirst() {
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
TicketSystem::TicketSystem() {}
void TicketSystem::Work() {
  while (true) {
    try {
      // judge privilege inside the TicketSystem
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
        // cout << user.username << ' ' << user.name << ' ' << user.mail_addr << ' ' << user.privilege << endl;
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
        // TODO
      }
      if (now_cmd.op == "release_train") {
        // TODO
      }
      if (now_cmd.op == "query_train") {
        // TODO
      }
      if (now_cmd.op == "query_ticket") {
        // TODO
      }
      if (now_cmd.op == "query_transfer") {
        // TODO
      }
      if (now_cmd.op == "buy_ticket") {
        // TODO
      }
      if (now_cmd.op == "query_order") {
        // TODO
      }
      if (now_cmd.op == "refund_ticket") {
        // TODO
      }
      if (now_cmd.op == "rollback") {
        // TODO
      }
      if (now_cmd.op == "clean") {
        // TODO
      }
    } /*  catch (const char* msg) {
       cout << "-1" << endl;
       cerr << msg << endl;
     }  */
    catch (...) {
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
TicketSystem::~TicketSystem() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_TICKET_SYSTEM_CPP