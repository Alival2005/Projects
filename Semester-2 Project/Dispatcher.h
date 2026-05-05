// Dispatcher.h
// The brain of the Emergency Response system
#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "Responder.h"
#include "Emergency.h"
#include "Ambulance.h"
#include "PoliceCar.h"
#include "FireTruck.h"
#include "Logger.h"
#include "Filehandler.h"
#include <vector>
#include <string>
#include <sstream>

class Dispatcher
{
    // AGGREGATION: Dispatcher manages these but doesn't create them itself
    vector<Responder*> responders;   // all units in the system (Ambulance/Police/FireTruck)
    vector<Emergency>  emergencies;  // all emergencies ever reported

    int nextEmergencyId;             // auto-increments so every emergency gets a unique ID

    // COMPOSITION: Dispatcher owns these,they are created and destroyed with it 
    Logger      logger;                          // activity log (composition)
    FileHandler fileHandler;                     // file save/load helper (composition)
    // Private helper: converts "high"/"medium"/"low" to a number
    int priorityToNumber(string priority)
    {
        if (priority == "high")   return 3;
        if (priority == "medium") return 2;
        return 1; // low
    }
    // sorted highest priority first (3 -> 2 -> 1)
    // We sort the indexes (not the vector) to keep IDs stable
    vector<int> getPendingSortedByPriority()
    {
        vector<int> pendingIndexes;  // Private helper: returns indexes of PENDING emergencies

        for (int i = 0; i < (int)emergencies.size(); i++)
        {
            if (emergencies[i].getStatus() == "pending")
                pendingIndexes.push_back(i);
        }

        // Bubble sort (descending): highest priority index bubbles to the front
        for (int i = 0; i < (int)pendingIndexes.size(); i++)
        {
            for (int j = i + 1; j < (int)pendingIndexes.size(); j++)
            {
                int priA = priorityToNumber(emergencies[pendingIndexes[i]].getPriority());
                int priB = priorityToNumber(emergencies[pendingIndexes[j]].getPriority());

                if (priB > priA)
                {
                    int temp          = pendingIndexes[i];
                    pendingIndexes[i] = pendingIndexes[j];
                    pendingIndexes[j] = temp;
                }
            }
        }

        return pendingIndexes;
    }

public:

    // Constructor: initialise ID counter, pass default filename to FileHandler
    Dispatcher() : fileHandler("dispatch_state.txt")
    {
        nextEmergencyId = 1;
    }
    // addResponder: registers a new unit with the system
    void addResponder(Responder* r)   // Polymorphism via base class pointer
    {
        responders.push_back(r);

        string msg = "Responder added: " + r->getName() + " (" + r->getType() + ")";
        cout << msg << endl;
        logger.log(msg);
    }
    // reportEmergency logs a new emergency as "pending"
    void reportEmergency(string type, string priority)
    {
        Emergency newEmergency(nextEmergencyId, type, priority);
        emergencies.push_back(newEmergency);

        string msg = "Emergency #" + to_string(nextEmergencyId)
                   + " reported (" + type + ", " + priority + " priority)";
        cout << msg << endl;
        logger.log(msg);

        nextEmergencyId++;
    }
    // dispatch: matches PENDING emergencies to AVAILABLE units
    void dispatch()
    {
        cout << "\nInitiating Dispatch (Priority Order)...\n";// processesing HIGH priority emergencies first

        vector<int> sortedIndexes = getPendingSortedByPriority();

        if (sortedIndexes.empty())
        {
            cout << "  No pending emergencies to dispatch.\n";
            cout << "---------------------------" << endl;
            return;
        }

        for (int i = 0; i < (int)sortedIndexes.size(); i++)
        {
            int idx = sortedIndexes[i];

            // Map emergency type to the responder type it needs
            string requiredType = "";
            if      (emergencies[idx].getType() == "medical") requiredType = "Ambulance";
            else if (emergencies[idx].getType() == "crime")   requiredType = "Police";
            else if (emergencies[idx].getType() == "fire")    requiredType = "FireTruck";

            bool assigned = false;
          // Find an available responder of the correct type
            for (int j = 0; j < (int)responders.size(); j++)
            {
                if (responders[j]->isAvailable() && responders[j]->getType() == requiredType)
                {
                    responders[j]->setUnavailable();          // mark unit as busy
                    emergencies[idx].setStatus("assigned");   // mark emergency assigned

                    string msg = "Dispatched " + responders[j]->getName()
                               + " to Emergency #" + to_string(emergencies[idx].getId())
                               + " [" + emergencies[idx].getPriority() + " priority]";
                    cout << "  " << msg << endl;
                    logger.log(msg);

                    assigned = true;
                    break; // one unit per emergency is enough
                }
            }

            if (!assigned)
            {
                string msg = "No unit available for Emergency #"
                           + to_string(emergencies[idx].getId())
                           + " (" + emergencies[idx].getType() + ")";
                cout << "  WARNING: " << msg << endl;
                logger.log(msg);
            }
        }

        cout << "---------------------------" << endl;
    }
    // resolveEmergency: marks a job done and frees the unit
    void resolveEmergency(int emergencyId)   // The freed responder becomes available for new dispatches
    {
        for (int i = 0; i < (int)emergencies.size(); i++)
        {
            if (emergencies[i].getId() == emergencyId)
            {
                if (emergencies[i].getStatus() != "assigned")
                {
                    cout << "Emergency #" << emergencyId
                         << " is not currently assigned (status: "
                         << emergencies[i].getStatus() << ")." << endl;
                    return;
                }

                emergencies[i].setStatus("resolved");
              // Finding the corresponding busy responder and free it
                string requiredType = "";
                if      (emergencies[i].getType() == "medical") requiredType = "Ambulance";
                else if (emergencies[i].getType() == "crime")   requiredType = "Police";
                else if (emergencies[i].getType() == "fire")    requiredType = "FireTruck";

                for (int j = 0; j < (int)responders.size(); j++)
                {
                    if (!responders[j]->isAvailable() && responders[j]->getType() == requiredType)
                    {
                        responders[j]->setAvailable();

                        string msg = "Emergency #" + to_string(emergencyId)
                                   + " RESOLVED. " + responders[j]->getName()
                                   + " is now available.";
                        cout << msg << endl;
                        logger.log(msg);
                        return;
                    }
                }
                // Edge case: resolved but no matching busy unit found
                string msg = "Emergency #" + to_string(emergencyId) + " marked resolved.";
                cout << msg << endl;
                logger.log(msg);
                return;
            }
        }

        cout << "Emergency #" << emergencyId << " not found." << endl;
    }
    // saveState: uses FileHandler (Composition) to persist data
    void saveState()
    {
        vector<string> lines;  // Serialises each responder and emergency to a CSV-style line
        // Serialise each responder
        for (int i = 0; i < (int)responders.size(); i++)
        {
            string type = responders[i]->getType();
            string avail = responders[i]->isAvailable() ? "1" : "0";

            string line = "RESPONDER," + type + ","
                        + to_string(responders[i]->getId()) + ","
                        + responders[i]->getName() + "," + avail;
            // Add type-specific field using downcasting
            if (type == "Ambulance")
            {
                Ambulance* a = dynamic_cast<Ambulance*>(responders[i]);
                if (a) line += "," + to_string(a->getCrewCount());
            }
            else if (type == "Police")
            {
                PoliceCar* p = dynamic_cast<PoliceCar*>(responders[i]);
                if (p) line += "," + p->getUnit();
            }
            else if (type == "FireTruck")
            {
                FireTruck* f = dynamic_cast<FireTruck*>(responders[i]);
                if (f) line += "," + to_string(f->getWaterCapacity());
            }

            lines.push_back(line);
        }
        // Serialise each emergency
        for (int i = 0; i < (int)emergencies.size(); i++)
        {
            string line = "EMERGENCY,"
                        + to_string(emergencies[i].getId()) + ","
                        + emergencies[i].getType()          + ","
                        + emergencies[i].getPriority()      + ","
                        + emergencies[i].getStatus();
            lines.push_back(line);
        }
        lines.push_back("NEXT_ID," + to_string(nextEmergencyId));    // Also save the next ID counter so it doesn't reset on load

        fileHandler.saveState(lines);  // delegate to FileHandler (Composition)
    }
    // loadState: reads from file and rebuilds the system state
    void loadState()  // Parses each line and reconstructs responders + emergencies
    {
        // Clear current data before loading
        for (int i = 0; i < (int)responders.size(); i++)
            delete responders[i];
        responders.clear();
        emergencies.clear();

        vector<string> lines = fileHandler.loadState();  // FileHandler reads the file

        if (lines.empty())
        {
            cout << "No data to load." << endl;
            return;
        }

        for (int i = 0; i < (int)lines.size(); i++)
        {
            stringstream ss(lines[i]);// Parse each field by splitting on commas
            string token;
            vector<string> fields;

            while (getline(ss, token, ','))
                fields.push_back(token);

            if (fields.empty()) continue;
            // Rebuild a responder record
            if (fields[0] == "RESPONDER" && fields.size() >= 6)
            {
                string type  = fields[1];
                int    id    = stoi(fields[2]);
                string name  = fields[3];
                bool   avail = (fields[4] == "1");
                string extra = fields[5];

                Responder* r = nullptr;

                if (type == "Ambulance")
                    r = new Ambulance(id, name, stoi(extra));
                else if (type == "Police")
                    r = new PoliceCar(id, name, extra);
                else if (type == "FireTruck")
                    r = new FireTruck(id, name, stoi(extra));

                if (r != nullptr)
                {
                    if (!avail) r->setUnavailable(); // restore busy status
                    responders.push_back(r);
                    cout << "  Loaded: " << r->getName() << " (" << type << ")" << endl;
                }
            }

            // Rebuild an emergency record
            else if (fields[0] == "EMERGENCY" && fields.size() >= 5)
            {
                int    id       = stoi(fields[1]);
                string type     = fields[2];
                string priority = fields[3];
                string status   = fields[4];

                Emergency e(id, type, priority);
                e.setStatus(status);
                emergencies.push_back(e);
                cout << "  Loaded: Emergency #" << id << " (" << type << ", " << status << ")" << endl;
            }

            // Restore the ID counter
            else if (fields[0] == "NEXT_ID" && fields.size() >= 2)
            {
                nextEmergencyId = stoi(fields[1]);
            }
        }

        cout << "Load complete. " << responders.size() << " responders, "
             << emergencies.size() << " emergencies restored." << endl;
    }
     // Show all responders and emergencies with current status
    void showAll()
    {
        cout << "\n----- SYSTEM STATUS -----" << endl;

        cout << "--- Responders ---" << endl;
        if (responders.empty())
            cout << "  No responders in system." << endl;
        for (int i = 0; i < (int)responders.size(); i++)
            responders[i]->display(); // polymorphism: calls correct child display()

        cout << "\n--- Emergencies ---" << endl;
        if (emergencies.empty())
            cout << "  No emergencies reported." << endl;
        for (int i = 0; i < (int)emergencies.size(); i++)
            emergencies[i].display();

        cout << "--------------------\n" << endl;
    }
    void showStats()// Quick statistics summary
    {
        int availableCount = 0, busyCount = 0;
        int pendingCount   = 0, assignedCount = 0, resolvedCount = 0;

        for (int i = 0; i < (int)responders.size(); i++)
        {
            if (responders[i]->isAvailable()) availableCount++;
            else                              busyCount++;
        }

        for (int i = 0; i < (int)emergencies.size(); i++)
        {
            string s = emergencies[i].getStatus();
            if      (s == "pending")  pendingCount++;
            else if (s == "assigned") assignedCount++;
            else if (s == "resolved") resolvedCount++;
        }

        cout << "\n-------STATISTICS---------" << endl;
        cout << "Responders  : " << responders.size()
             << " total  (" << availableCount << " available, " << busyCount << " busy)" << endl;
        cout << "Emergencies : " << emergencies.size()
             << " total  (" << pendingCount << " pending, "
             << assignedCount << " assigned, " << resolvedCount << " resolved)" << endl;
        cout << "Log entries : " << logger.getCount() << endl;
        cout << "----------------\n" << endl;
    }
    // Print the full activity log
    void showLog()
    {
        logger.showHistory();
    }
    // Destructor:free all heap-allocated Responder objects (no memory leaks)
    ~Dispatcher()
    {
        for (int i = 0; i < (int)responders.size(); i++)
            delete responders[i];
    }
};

#endif
