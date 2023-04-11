not a good solution because:
 - user of a list must remember to create a lock (this is bad you shouldn't be required to do it) - but noone specified it is for user
 - imagine a user of your list has a need not for one, but for many list - still, noone specified it's for users, we thought it's just assignment for labs

proper solutions is:
When having a function when creating a list: create.list(...)

it should return a `list descriptor` and inside of it we should have initialized lock for this specific instance.

