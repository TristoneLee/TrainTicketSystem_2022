#include "train_ticket_system.h"
int main(int argc, char *argv[]) {
#ifndef ONLINE_JUDGE
  if (argc > 1 && !strcmp(argv[1], "debug")) {
    freopen("stdin.txt", "r", stdin);
    freopen("tmp.out", "w", stdout);
  }
#endif
  sjtu::TrainTicketSystem().Work();
}