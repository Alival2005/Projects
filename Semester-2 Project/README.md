# Emergency Response Dispatcher
## CS112 — Object Oriented Programming & Design
### Semester Project

---

## How to Compile & Run

### Requirements
- A C++ compiler (g++ or any C++11-compatible compiler)
- Terminal / Command Prompt

### Compile the interactive CLI
```
g++ -o main main.cpp
```

### Compile and run the automated test
```
g++ -o testMain testMain.cpp
./testMain
```

### Run the CLI
```
./main
```

---

## Available CLI Commands

| Command | Example | What it does |
|---|---|---|
| `add_responder ambulance <id> <name> <crew>` | `add_responder ambulance 101 Rescue-1 3` | Adds an ambulance with crew count |
| `add_responder police <id> <name> <unit>` | `add_responder police 102 Alpha-1 Unit-7` | Adds a police car |
| `add_responder firetruck <id> <name> <litres>` | `add_responder firetruck 103 Blaze-1 5000` | Adds a fire truck |
| `report <type> <priority>` | `report medical high` | Reports a new emergency (pending) |
| `dispatch` | `dispatch` | Sends units to pending emergencies (high priority first) |
| `resolve <id>` | `resolve 2` | Marks emergency done, frees the responder |
| `show` | `show` | Displays full system status |
| `stats` | `stats` | Quick summary numbers |
| `log` | `log` | Full activity history |
| `save` | `save` | Saves state to dispatch_state.txt |
| `load` | `load` | Loads state from dispatch_state.txt |
| `help` | `help` | Prints command list |
| `exit` | `exit` | Shuts down the system |

### Valid values
- Emergency types: `medical`, `crime`, `fire`
- Priority levels: `high`, `medium`, `low`

---

## Sample Session
```
>> add_responder ambulance 101 Ambulance-1 3
>> add_responder police 102 Police-1 Unit-7
>> add_responder firetruck 103 FireTruck-1 5000
>> report fire high
>> report medical low
>> dispatch
>> show
>> resolve 1
>> save
>> exit
```

---

## File Structure

| File | Purpose |
|---|---|
| `Emergency.h` | Represents a single emergency event |
| `Responder.h` | Abstract base class for all responder units |
| `Ambulance.h` | Ambulance (inherits Responder) |
| `PoliceCar.h` | Police car (inherits Responder) |
| `FireTruck.h` | Fire truck (inherits Responder) |
| `Logger.h` | Records all system activity as a history log |
| `FileHandler.h` | Handles saving and loading state to/from file |
| `Dispatcher.h` | Core system logic — manages all units and emergencies |
| `main.cpp` | CLI interface — parses and routes user commands |
| `testMain.cpp` | Automated test script |
| `dispatch_state.txt` | Auto-generated save file (created by `save` command) |

---

## OOP Concepts Applied

| Concept | Where |
|---|---|
| **Classes & Objects** | All .h files |
| **Encapsulation** | Private fields with getters/setters in Emergency.h, Responder.h |
| **Inheritance** | Ambulance, PoliceCar, FireTruck extend Responder |
| **Polymorphism** | `vector<Responder*>` — runtime dispatch via virtual functions |
| **Abstraction** | Responder is abstract (pure virtual `getType()`) |
| **Composition** | Dispatcher owns Logger and FileHandler |
| **Aggregation** | Dispatcher manages Responder* objects |
| **File Handling** | `save` / `load` commands via FileHandler.h |

---

## Group Members
| Name| Responsibility |
|---|---|
|Amna Aamer |Core logic ,logger,file handling |
|syed Ali Abbas Abidi | CLI engine & command parsing (main.cpp),testing |

