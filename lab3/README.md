program that inputs `n` (> 0)
and it folds itself recursively (n levels)

---

example command
`PS`
# Lab 3
## 3.1
## 3.2
1.
Running processes for `codespace` user on `pts/1`
`ps -u codespace | grep pts/1`
2.
In order to run a process in the background
command must be ended with `&` symbol
3.
program running in the background can be found
using `jobs` command
4.
To attach to a process use `fg <job_number>` command
5.
To exit the job use `Ctrl + Z` combination
6.
To run it again in the background use `bg <job_number>` command
7.
running `ps -u codespace |grep pts/1` makes two processes
run concurrently.
8.
When we run a command that have errors and successes coming out like in command
we can filter out f.e. error out with `find / - name * mp3 2> /dev/null/mnt/multimedia/temp/pw.mp3`
9.
`top` command shows the processes and their **processor** time and **memory** occupation.
It's an interactive command.
## 3.3
10.
To create a process we use:
```c
#include <unistd.h>
pid_t fork(void);
```
It creates an **exact copy** of current process and will be
enabled after `fork()` function called. To check which process is
the **first** one and which the **second**, we check the return on fork.

In the **first** process the function will return the `pid` of the **second**
process. The **second** process is going to have `fork()` return 0 *(as it is the first one for itself.)*

Function `getpid()` returns id of the called process. `getppid()` gets the id of the previous process.
*To remember: get-parent-pid (getppid), and get-pid (getpid)*

To see descendant processes we run `ps` command with such argument:
`ps -u codespace --forest`

