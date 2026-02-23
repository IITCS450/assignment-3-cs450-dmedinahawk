#include "types.h"
#include "stat.h"
#include "user.h"

static volatile int sink = 0;
static void burn(int n){
  for(int i = 0; i < n; i++)
    sink += i;
}

int
main(int argc, char **argv)
{
  int tickets[3] = {10, 20, 30};
  int pids[3];

  printf(1, "fairlotterytest: starting fairness experiment\n");

  // Fork 3 children with different ticket counts
  for(int i = 0; i < 3; i++){
    int child = fork();
    if(child < 0){
      printf(1, "fairlotterytest: fork failed\n");
      exit();
    }

    if(child == 0){
      // Child process
      if(settickets(tickets[i]) != 0){
        printf(1, "child %d: settickets(%d) failed\n", i, tickets[i]);
        exit();
      }

      int start = uptime();
      int count = 0;

      // Run for ~200 ticks
      while(uptime() - start < 1500){
        burn(50);
        count++;
      }

      printf(1, "child %d (tickets=%d): count=%d\n", i, tickets[i], count);
      exit();
    } else {
      pids[i] = child;
    }
  }

  // Parent waits for all children
  for(int i = 0; i < 3; i++)
    wait();

  printf(1, "fairlotterytest: done\n");
  exit();
}
