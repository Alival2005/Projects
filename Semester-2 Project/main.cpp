// Text-command based interface 
// Every command does a specific action; no numbered menus.
#include <iostream>
#include <sstream>
#include <string>
#include "Dispatcher.h"
using namespace std;
// Displays all available commands (called on startup and when help is typed)
void showHelp()
{
    cout << "\n-----EMERGENCY DISPATCH SYSTEM------" << endl;
    cout << "  add_responder ambulance <id> <name> <crew_count>" << endl;
    cout << "  add_responder police    <id> <name> <unit>"       << endl;
    cout << "  add_responder firetruck <id> <name> <water_L>"    << endl;
    cout << "  report   <type> <priority>   (type: medical/crime/fire)" << endl;
    cout << "  dispatch                     (sends units to pending emergencies)" << endl;
    cout << "  resolve  <emergency_id>      (marks a job done, frees the unit)" << endl;
    cout << "  show                         (display full system status)" << endl;
    cout << "  stats                        (quick summary numbers)" << endl;
    cout << "  log                          (full activity history)" << endl;
    cout << "  save                         (save state to file)" << endl;
    cout << "  load                         (load state from file)" << endl;
    cout << "  help                         (show this list)" << endl;
    cout << "  exit                         (shut down)" << endl;
    cout << "-----------------------------------\n" << endl;
}

int main()
{
    Dispatcher system;

    cout << "-------------------------------------------" << endl;
    cout << "   EMERGENCY RESPONSE DISPATCH SYSTEM     " << endl;
    cout << "-------------------------------------------" << endl;
    showHelp();
    string line;
    // Main loop — runs until the user types "exit"
    while (true)
    {
        cout << ">> ";
        getline(cin, line);

        if (line.empty()) continue;

        // Split input into command + arguments
        stringstream ss(line);
        string cmd;
        ss >> cmd;
        // add_responder adds ambulance, police, or firetruck
        if (cmd == "add_responder")
        {
            string type, name;
            int id;
            ss >> type >> id >> name;

            if (name.empty())
            {
                cout << "Usage: add_responder <type> <id> <name> [extra]" << endl;
                continue;
            }

            if (type == "ambulance")
            {
                int crew;
                if (!(ss >> crew))
                {
                    cout << "Usage: add_responder ambulance <id> <name> <crew_count>" << endl;
                    continue;
                }
                system.addResponder(new Ambulance(id, name, crew));
            }
            else if (type == "police")
            {
                string unit;
                if (!(ss >> unit))
                {
                    cout << "Usage: add_responder police <id> <name> <unit>" << endl;
                    continue;
                }
                system.addResponder(new PoliceCar(id, name, unit));
            }
            else if (type == "firetruck")
            {
                int capacity;
                if (!(ss >> capacity))
                {
                    cout << "Usage: add_responder firetruck <id> <name> <water_L>" << endl;
                    continue;
                }
                system.addResponder(new FireTruck(id, name, capacity));
            }
            else
            {
                cout << "Unknown type: \"" << type << "\". Use: ambulance | police | firetruck" << endl;
            }
        }
        // report  logs a new emergency as pending
        else if (cmd == "report")
        {
            string type, priority;
            ss >> type >> priority;

            if (type.empty() || priority.empty())
            {
                cout << "Usage: report <type> <priority>" << endl;
                continue;
            }
           // Validate type
            if (type != "medical" && type != "crime" && type != "fire")
            {
                cout << "Invalid type: \"" << type << "\". Use: medical | crime | fire" << endl;
                continue;
            }
           // Validate priority
            if (priority != "high" && priority != "medium" && priority != "low")
            {
                cout << "Invalid priority: \"" << priority << "\". Use: high | medium | low" << endl;
                continue;
            }

            system.reportEmergency(type, priority);
        }
        // dispatch: sends available units to pending jobs
        // Handles high priority emergencies first
        else if (cmd == "dispatch")
        {
            system.dispatch();
        }
        // resolve <id>: marks a job done, frees the unit
        else if (cmd == "resolve")
        {
            int emergencyId;
            if (!(ss >> emergencyId))
            {
                cout << "Usage: resolve <emergency_id>" << endl;
                continue;
            }
            system.resolveEmergency(emergencyId);
        }
        // show display all responders and emergencies
        else if (cmd == "show")
        {
            system.showAll();
        }
        // stats prints a quick summary of system numbers
        else if (cmd == "stats")
        {
            system.showStats();
        }
        // log prints the full activity history
        else if (cmd == "log")
        {
            system.showLog();
        }
        // save persists system state to dispatch_state.txt
        else if (cmd == "save")
        {
            system.saveState();
        }
        // load restores system state from dispatch_state.txt
        else if (cmd == "load")
        {
            system.loadState();
        }
        // help: reprints the command list
        else if (cmd == "help")
        {
            showHelp();
        }
        // exit: clean shutdown
        else if (cmd == "exit")
        {
            cout << "Dispatch system shutting down. Goodbye." << endl;
            break;
        }
        // Unknown command
        else
        {
            cout << "Unknown command: \"" << cmd << "\". Type 'help' to see all commands." << endl;
        }
    }

    return 0;
}
