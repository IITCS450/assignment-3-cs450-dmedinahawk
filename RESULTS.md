Destiny Medina
CS450- Assignment 3 Results report
Setup (children	count,	ticket	counts)
I created a fairness experiment using 3 child processes.
each child was assigned a different number of lottery tickets:
[experiment below]

$ fairlotterytest
fairlotterytest: starting fairness experiment
child 0 (tickets=10): count=50179
child 1 (tickets=20): count=14878
child 2 (tickets=30): count=22106
fairlotterytest: done

All children were forked from the same parent and ran concurrently.

Test lottery was used to check correctness not fairness and consequentially didnt measure CPU share. However it was able to confirm my syscall, ensure the kernel accepts the ticket values and helped debug earlier mistakes. it ran fast and didnt need long runtime or scheduling behavior.

Fairlotterytest experimented with scheduler fairness. It was created to fork multiple cildren, assign differnt ticket counts, and run CPU-bound work for a fixed amount of WCT. it measured how much cpu each child received, and showed semi-proportional cpu distribution. I say semi-proportional because through different tests it was clear that there was weighted randomness. 

Workload (CPU-bound	loop)

I decided not to combine testing because the syscall test would take too long, and instead of fairness checking there would be sys checking.
Each child went through a CPU-bound loop consisting of repeated calls to a small burn() function. the loop ran for a fixed amount of WCT that was measured using uptime(). This ensured that all children ran for the same duration.
A yield() inside the loop made scheduler interaction frequent.
By designing this workload it allowed the lottery scheduler to determine how many iteration each child completes based on ticket count.

in an earlier version of testing i set the interations to 20 million and 3 children. Unknowlingly I set 3 billion operations total across all children. I was not able to complete a fairness test because it took too long to process. This is where uptime() came in handy. 

Observed relative shares over a sufficiently long run	
At first i thought each child would produce proportional shares relative to their tickets. NUmbers varied but consistently the highest-ticket process consistently recieves the most CPU time, and lower ticket processes receive proportionally less.

Notes on variance and why longer runs converge	
lottery scheduling is probablistic, not deterministic. individual runs may show noise or temporary inversions, especially over short intervals.
However, as the runtime increases and more lotteries occur, the average CPU share converges toward the ticket ratios. Longer runs reduce variance and make the proportional relationship more apparent.