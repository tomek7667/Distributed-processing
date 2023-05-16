# Extend chat server

**Goal** - use all power of a hardware. **Single** thread is not good solution for todays computer - to utilize all power we must use clever approach using **many** threads.

TCP Timeout is ~2 minutes

```
C1      C2      C3
|       |       |
--------------------        <---- server
| t1    | t2    | t3        <---- threads
* f1    * f2    * f3        <---- fifo queues with messages
|
 \_______________________________ if message appears here,
                                  it puts in on other queue
```

on internet should be a **thread pool**.
---

Today assignment - use pipes [first google page](https://tldp.org/LDP/lpg/node11.html)

topic: **anoynymous and named pipes** *(potoki nazwane i anonimowe)*

> I would like you to modify your server at least in above way (1 thread per client, when new message arrives, put it on anonymous pipe of every other thread) - this solves problem: Code is using all the cores, if one client happens to have a bad connection the worst thing that may happen only one fifo will start to grow and it **doesn't** affect others. To do this, one piece of knowledge is needed: 
>
> We have a thread with inputs. We need to write a thread function which can monitor pipe and a socket at the same time. How to do it?
> 
> In unix pipe endpoint and socket is a file descriptor, so you have **file descriptior 1** and **file descriptor 2**. 
> 
> How to monitor **file descriptor**?
>
> **Select** - everywhere (max 1024) we need only 2. [Documentation for select](https://pubs.opengroup.org/onlinepubs/009695399/functions/select.html)
> 
> other polling - (maybe windows specific)
> **epoll**

---

the implementations `better_server.c` and `better_client.c` are to be used. The other code in lab8 is attached in order to show failure from trials before.
