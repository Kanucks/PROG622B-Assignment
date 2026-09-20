# NeoVerse: AI City Survival System

A console-based C++17 simulation built for the PROGRAMMING 622 assignment. It
models an AI-controlled smart city (engineer login, sensor/log storage, a
real-time event pipeline, four polymorphic city subsystems, STL-algorithm
driven reports, and file persistence).

## 1. How to run the simulation

**Requirements:** a C++17 compiler (g++ 7+ or clang++ 6+) and `make`. No
external libraries are used — only the standard library.

```bash
make        # builds ./neoverse from src/*.cpp into build/*.o
./neoverse  # run it (or: make run)
make clean  # remove build artifacts
```

Run it from the project's root folder (the one containing `data/`), since
the program reads/writes `data/engineers.dat`, `data/sensors.dat`,
`data/city_logs.dat`, `data/events.dat` and `data/config.txt` using
relative paths. If `data/engineers.dat` is missing or empty the program
seeds three default accounts automatically and saves them (see below).

On exit (menu option 14) the current engineer list, sensor readings, city
logs and processed-event history are all written back to `data/`, so state
survives between runs.

## 2. Sample login credentials

| Username | Password  | Clearance | Engineer ID |
|----------|-----------|-----------|-------------|
| admin    | admin123  | High      | ENG001      |
| jsmith   | pass123   | Medium    | ENG002      |
| tnandi   | city2035  | Low       | ENG003      |

These are seeded on first run. Passwords are never stored as plain text —
`Engineer` stores an XOR-then-hex-encoded cipher (see `Utils::encrypt` /
`Utils::decrypt`). This is a simple, reversible encoding chosen to satisfy
the "encrypted password" requirement in a way that is easy to mark and
explain; it is **not** presented as production-grade cryptography.

You get 3 login attempts before the program exits (`login()` in `main.cpp`).

## 3. Menu overview

```
 1. View city component status        7. Raise a new city event (enqueue)
 2. Add sensor reading                 8. Process next event (FIFO)
 3. Remove sensor reading by ID        9. Raise an emergency override
 4. Display all sensor readings       10. Resolve next emergency (LIFO)
 5. Add city log entry                11. View reports & analytics
 6. Display city logs                 12. Export event history to CSV
                                       13. Compare login search algorithms
                                       14. Save & Exit
```

Option 13 times both `loginLinear` (O(n)) and `loginBinary` (O(log n)) on
the current engineer list with `<chrono>` so the complexity difference in
section 1's brief can be seen directly rather than only argued about.

## 4. Project layout

```
include/      all class headers (.h)
src/          all implementations (.cpp), including main.cpp
data/         engineers.dat, sensors.dat, city_logs.dat, events.dat,
              config.txt, events_export.csv  — persisted state
Makefile      build script (g++ -std=c++17 -Wall -Wextra)
```

## 5. Containers used, and why

| Data                        | Container                | Why                                                                 |
|------------------------------|---------------------------|----------------------------------------------------------------------|
| Engineers                    | `std::vector<Engineer>`  | Small, mostly-read collection; contiguous memory for `find_if`/sort. |
| Today's sensor readings       | `std::vector<SensorReading>` | Needs fast random access, sorting and iteration for reports — a vector's contiguous, cache-friendly layout is built for exactly that. |
| Historical city logs          | hand-written `LinkedList<CityLogEntry>` (`include/LinkedList.h`) | The brief calls for "unlimited growth" history. A singly linked list with a tail pointer appends a new node in O(1) without ever reallocating or shifting existing entries, unlike a vector, which occasionally has to copy everything it holds when it outgrows its buffer. |
| Incoming city events          | `std::queue<Event>`      | Events must be processed in arrival order → FIFO is queue's whole purpose. |
| Emergency overrides           | `std::stack<EmergencyEvent>` | The newest emergency must be handled first → LIFO is stack's whole purpose. |
| Processed event / emergency history | `std::vector<Event>` / `std::vector<EmergencyEvent>` | Feeds the report generator, which needs `sort`/`min_element`/`max_element`/`count_if` — all vector-friendly algorithms. |
| Config key/value pairs        | `std::map<std::string,std::string>` | Small, unordered set of named settings; a map gives O(log n) lookup by name and there's no need to preserve insertion order. |

**Vector vs linked list, in one line:** a vector is fast to *read* (index
straight into contiguous memory) but can be expensive to *grow* past its
capacity; a linked list is cheap to *grow forever* (just allocate one more
node and link it) but can only be *read* by walking from the head. That's
exactly why sensor readings — read constantly for reports — sit in a
vector, and the ever-growing historical log sits in a linked list.

## 6. Big-O decisions

### 1. Login search (`EngineerManager`, section 1)
- `loginLinear` — `std::find_if` over an unsorted vector: **O(n)** worst
  case, no pre-processing needed. Fine for a small, frequently-changing
  engineer roster.
- `loginBinary` — sorts the vector by username (**O(n log n)**, done once)
  then `std::lower_bound` (**O(log n)**) to locate the entry. Only pays
  off once the roster is large *and* mostly static, because every insert
  potentially invalidates the sort order. Both are exercised, and timed
  against each other, from menu option 13.

### 2. City data (`CityDataManager`, section 2)
- Vector `push_back` — **amortised O(1)**; occasional O(n) reallocation
  when capacity is exceeded.
- Vector removal (`removeSensorReadingById`, erase-remove idiom) —
  **O(n)**: every element after the removed one must shift left to stay
  contiguous.
- Vector traversal / display — **O(n)**.
- LinkedList `append` — **O(1)** (tail pointer, no shifting).
- LinkedList `removeIf` — **O(n)** (must walk from head to find a match).
- LinkedList `forEach` traversal — **O(n)**.
- **Insertion comparison:** the vector's amortised O(1) push_back and the
  list's guaranteed O(1) append look similar, but the vector's is only
  "amortised" — any push that exceeds capacity costs O(n) to reallocate
  and copy every existing element, while the list's O(1) append is O(1)
  *every single time*, which is what makes it a better fit for a log that
  must never stop growing.

### 3. Event processing (`EventManager`, section 3)
- `queue::push` / `queue::pop` — **O(1)** each; keeps FIFO ordering.
- `stack::push` / `stack::pop` — **O(1)** each; keeps LIFO ordering.
- **Why stack vs queue:** ordinary events (traffic, weather, etc.) are
  independent and should simply be worked through in the order they
  arrived — a queue. An emergency override is different: a *newer* critical
  failure is, by definition, more urgent than one already waiting, so the
  most recently raised emergency should pre-empt older ones — a stack.

### 4. OOP dispatch (`CityComponent` hierarchy, section 4)
- Calling `processEvent()` through a `CityComponent*` is **O(1)** (one
  vtable lookup) regardless of which of the four subsystems the pointer
  actually refers to — this is what makes runtime polymorphism cheap.

### 5. STL algorithms (`ReportGenerator`, section 5)

| Algorithm | Complexity | Why it was chosen |
|-----------|-----------|---------------------|
| `std::sort` | O(n log n) | Needed a full ascending ordering of sensor readings by value; introsort is the standard general-purpose choice. |
| `std::find_if` | O(n) worst case | Looking up one event by ID in an unordered history vector — no faster option without first sorting/indexing it. |
| `std::min_element` / `std::max_element` | O(n) each | Single linear scan is optimal when the data isn't already sorted and you only need one extreme value. |
| `std::count_if` | O(n) | Must inspect every event once to tally how many are "critical" (severity ≥ 4). |

### 6. Reports (section 6)
Built from a single O(n) pass (or a couple of O(n) passes) over the
processed-event vector using iterators / range-based `for` and the
algorithms above — no report requires more than linear work.

### 7. File I/O (section 7)
Reading/writing each `.dat` file is **O(n)** in the number of records —
unavoidable, since persisting state means touching every record at least
once. Serialization uses a simple pipe-delimited (`|`) text format so the
files stay human-readable for testing and marking.

## 7. OOP concepts demonstrated

- **Encapsulation:** every class keeps its data `private`/`protected` and
  exposes it only through named getters/setters (e.g. `Engineer`,
  `CityComponent` and its four subclasses).
- **Inheritance:** `PowerSystem`, `TransportSystem`, `HealthSystem` and
  `SecuritySystem` all derive from the abstract `CityComponent`;
  `EmergencyEvent` derives from `Event`.
- **Polymorphism:** `CityComponent::processEvent()` is pure virtual;
  `EventManager` calls it through a `CityComponent*`, and the override
  that actually runs depends on the real object type at run time (see the
  differing console output per subsystem when an event is processed).
- **Constructors & destructors:** each `CityComponent` prints a line when
  constructed and destroyed (visible at startup/shutdown in the console),
  demonstrating object lifetime and that `delete` via a base pointer is
  safe because `~CityComponent()` is virtual.

## 8. File handling & persistence (section 7)

| File | Contents |
|------|----------|
| `data/engineers.dat` | one engineer per line: `ID\|username\|encryptedPasswordHex\|clearance` |
| `data/sensors.dat`   | one sensor reading per line: `id\|type\|value\|timestamp` |
| `data/city_logs.dat` | one historical log entry per line: `id\|message\|timestamp` |
| `data/events.dat`    | one processed event per line: `id\|type(int)\|description\|severity\|timestamp` |
| `data/config.txt`    | `key=value` settings (city name, queue size cap, etc.) |
| `data/events_export.csv` | CSV export of the processed-event history (menu option 12), for opening in a spreadsheet |

The four data files shipped in `data/` already contain sample records from
a real run of the program (login, sensor readings, log entries, a normal
event and an emergency override, a report, and a CSV export) so the
persistence and reporting features can be inspected without first running
a session.

## 9. Design notes / assumptions

- Each event type is routed to one primary subsystem for its `processEvent()`
  reaction: Power Failure → PowerSystem, Traffic Accident → TransportSystem,
  Weather Alert → HealthSystem (hospitals go on standby), Network Overload →
  SecuritySystem. This mapping lives in `EventManager.cpp` (`routeComponent`).
- "Average response time" (section 6) is approximated by average event
  severity, since the prototype doesn't yet model wall-clock dispatch
  latency — flagged here rather than silently faked.
- Compiled with `-Wall -Wextra` and currently builds warning-free.
