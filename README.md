*This project has been created as part of the 42 curriculum by gcabecas.*

---

## Description

Codexion is a concurrency simulation inspired by the classic Dining Philosophers
problem. A group of coders share a circular workspace, each needing two USB dongles
to compile their quantum code. Coders alternate between three states — compiling,
debugging, and refactoring — and must never burn out from lack of compilation time.

The program models concurrent resource contention using POSIX threads and mutexes,
implementing two arbitration policies (FIFO and EDF) to control how dongles are
granted to waiting coders. A separate monitor thread enforces the burnout deadline
and stops the simulation within 10 ms of the actual timeout.

## Instructions

**Compile:**
```
make
```

**Run:**
```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All times are in milliseconds. `scheduler` must be `fifo` or `edf`.

**Clean:**
```
make clean    # remove object files
make fclean   # remove objects and binary
make re       # full rebuild
```

## Blocking cases handled

**Deadlock prevention**

Coffman's four conditions for deadlock are: mutual exclusion, hold and wait,
no preemption, and circular wait. The first three are inherent to the problem
(dongles must be held exclusively while compiling). Circular wait is broken by
always acquiring dongles in ascending index order — coder N takes
`dongle[N-1]` then `dongle[N % nb_coders]`, swapping if necessary so the
lower index is always taken first. This total ordering eliminates cycles.

**Starvation prevention**

Under FIFO scheduling, every request is served in strict arrival order, so no
coder can be skipped indefinitely. Under EDF scheduling, the coder whose burnout
deadline is nearest is served first; when two deadlines are equal the lower
coder ID is used as a tiebreaker, making the result deterministic. Together
these policies guarantee that every coder eventually gets the dongle.

**Dongle cooldown**

After a dongle is released, it is locked for `dongle_cooldown` milliseconds.
The waiting coder re-checks availability on every condition variable wakeup
using `gettimeofday` to measure elapsed time since `last_release`. A 1 ms
timeout on `pthread_cond_timedwait` ensures the coder wakes up and re-checks
even when no broadcast arrives, so the cooldown expiry is never missed.

**Precise burnout detection**

A dedicated monitor thread loops every 500 µs checking each coder's elapsed
time since their last compile start. The timestamp is taken individually per
coder inside the loop so late coders in the list are not measured against a
stale snapshot. Burnout is logged and the stop flag set atomically, guaranteeing
the log appears within 10 ms of the actual deadline.

**Log serialization**

All output goes through `print_log`, which locks `print_mutex` around the
`printf` call. This prevents two threads from interleaving characters on the
same line.

## Thread synchronization mechanisms

**Primitives used**

| Primitive | Count | Role |
|---|---|---|
| `pthread_mutex_t` | 1 per dongle + 1 per coder + 3 global | Protect dongle state, coder state, stop flag, print output, start signal |
| `pthread_cond_t` | 1 per dongle + 1 global | Wake waiting coders on dongle release; synchronize simulation start |

**Dongle access**

Each `t_dongle` owns a `mutex` and a `cond`. To acquire a dongle, a coder:

1. Locks `d->mutex`
2. Pushes a priority request onto the dongle's min-heap queue
3. Loops on `pthread_cond_timedwait` until `can_take` returns true
4. Sets `d->held = 1`, pops from the queue, unlocks

`can_take` checks three conditions under the mutex: the dongle is not held,
the cooldown has elapsed, and this coder is at the top of the priority queue.
Only one coder can pass all three at a time, preventing races on the `held` flag.

On release, `d->held = 0` and `d->last_release` are written under `d->mutex`,
then `pthread_cond_broadcast` wakes all waiters. Each waiter re-evaluates
`can_take` independently — only the head of the queue will succeed.

**Start synchronization**

All coder threads and the monitor thread call `wait_sim_start` before entering
their main loop. They block on `start_cond` until the main thread calls
`start_sim` after all threads are created. This ensures every thread starts
from the same reference point, so relative timestamps are consistent.

**Stop flag**

`sim->stop` is read and written exclusively through `set_stop` and `is_stopped`,
which both lock `stop_mutex`. This prevents torn reads across threads. When the
monitor sets the stop flag, all coder threads see it on their next iteration or
when their `pthread_cond_timedwait` times out (at most 1 ms later).

**Race condition example — compile count**

`coder->compile_count` is incremented by the coder thread and read by the
monitor thread. Both operations go through `inc_compile_count` and
`get_compile_count`, which lock `coder->mutex`. Without this, the monitor could
read a partially-written integer and incorrectly decide the simulation is over.

## Resources

- `man pthread_create`, `man pthread_mutex_init`, `man pthread_cond_wait`,
  `man pthread_cond_timedwait`, `man pthread_cond_broadcast` — POSIX thread
  primitives reference

**AI usage**

AI  was used to understand POSIX threading concepts (mutex ownership,
condition variable spurious wakeups, memory visibility between threads) and to
clarify the subject requirements (EDF deadline definition, cooldown semantics,
the 10 ms burnout precision constraint) and for the readme.
