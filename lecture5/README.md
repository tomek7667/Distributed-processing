Test&Set instruction - CPU instruction:

waiting on a loop until a condition happens, when it happens it sets and loops again (?)

- busy waiting
- test might never change 
- cpu wasting

if implementing semaphors (who knows what it is) then such instructions might be very very helpful

---

# Semaphors and monitors (and mutexes) - procedrual programming (C, pascal)

These are designed to give synchronisation mechanisms on a **single** computer.

## Difference between a **mutex** and a **binary semaphor**

understand the `ownerships`, if the thread enter a critical section it becomes the `owner`. It allows re-entrancy for the thread. If it was a binary sempahor it would stop in a recursive function, mutex allow multiple. *(Also they have a counter of enter function)*.

Mutext forget the owner when the function entry counter is `0`

## Monitor

Synchronization mechanism design to protect one resource. 

# Side notes

## tuple spaces
allow to introduce synchronisation to an application *(Linda - in last lectures, - it's an environment that is orthogonal to programming languages. It can be used **from** programming languages)*

## Streams
usually used in a single computer, can be **anonymous** or **main**. Professor is not educated enough to know if it exists in windows systems *(an doesn't care to learn as he himself admitted)*.

## Channels, messages

extended idea of steams. It's the same, just different underlying communication is being used. Can be created having 2 streams in opposing directions. **Subchannel** might be introduced for prioritizing. TCP allow such prioritize to transfer **out of band** packages.

When a receiver is down we can ofc non-stop transmit data, but it's wasteful. **Control information** might be used.

Subchannels idea might be implemented in different ways. not excaly priority mechanisms.

## Remote - function | service | method execturion

Used in distributed processing environments.

## Mailboxes, generic links (like sockets), file reading or pipe reading, signals

also can be used for synchronization purposes.

### signals

usually interrupts software in a small packets. Used when need to notify asynchronous about some event.

# Semaphors

- we can enter semaphor
- we can leave semaphor

it's a protected variable. If it's non-zero the process can pass, if it's zero, process is put to waiting set.

*it works on a queue provided by OS so we assume it works correctly*

Semaphor can be either `0` or `1`. We can raise it many times but it will be still `1`.

**Binary semaphor doesn't remember how many times it was raised.**

**General semaphor can be in/decremented by any arbitrary number**

Sometimes we will encounter operations that will operate jointly on a set of semaphors.

If try to enter at multiple semaphors at once, or sometimes when **at least** one semaphor permits
- PAND - if all
- POR - if any of Semaphors 

sum

- any semaphor is integer
- we can't do arythmetic
- we can't ask semaphor for n of proccesses that wait
- sufficient for patterns: mutual exclusion, producer consumer, 5 philosophers and more