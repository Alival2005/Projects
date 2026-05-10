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
#include "Constants.h"
#include <vector>
#include <string>
#include <sstream>
#include <map>

class Dispatcher
{
    // AGGREGATION: Dispatcher manages these but doesn't create them itself
    vector<Responder*> responders;   // all units in the system (Ambulance/Police/FireTruck)
    vector<Emergency>  emergencies;  // all emergencies ever reported

    int nextEmergencyId;             // auto-increments so every emergency gets a unique ID

    // Assignment map: tracks exactly which responder is handling which emergency.
    // Key = emergencyId, Value = index into responders vector.
    // This fixes the resolve bug where any busy unit of the right type was freed
    // instead of the specific unit that was actually dispatched.
    map<int, int> assignmentMap;

    // COMPOSITION: Dispatcher owns these, they are created and destroyed with it
    Logger      logger;       // activity log (composition)
    FileHandler fileHandler;  // file save/load helper (composition)

    // Private helper: converts priority string to a number for sorting
    int priorityToNumber(const string& priority) const
    {
        if (priority == PRIORITY_HIGH)   return 3;
        if (priority == PRIORITY_MEDIUM) return 2;
        return 1; // low
    }

    // Private helper: maps an emergency category to the responder type it needs
    string emergencyToResponderType(const string& emergType) const
    {
        if (emergType == EMERG_MEDICAL) return TYPE_AMBULANCE;
        if (emergType == EMERG_CRIME)   return TYPE_POLICE;
        if (emergType == EMERG_FIRE)    return TYPE_FIRETRUCK;
        return "";
    }

    // Private helper: returns indexes of PENDING emergencies sorted highest priority first.
    // Sorts indexes (not the vector itself) so emergency IDs stay stable.
    // Uses bubble sort (descending): highest priority bubbles to the front.
    vector<int> getPendingSortedByPriority()
    {
        vector<int> pendingIndexes;

        for (int i = 0; i < (int)emergencies.size(); i++)
        {
            if (emergencies[i].getStatus() == STATUS_PENDING)
                pendingIndexes.push_back(i);
        }

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

    // addResponder: registers a new unit with the system.
    // Rejects duplicates — two responders cannot share the same ID.
    void addResponder(Responder* r)
    {
        // ID duplicate check
        for (int i = 0; i < (int)responders.size(); i++)
        {
            if (responders[i]->getId() == r->getId())
            {
                cout << "ERROR: Responder with ID " << r->getId()
                     << " already exists. Responder not added." << endl;
                delete r; // avoid memory leak since we own the pointer now
                return;
            }
        }

        responders.push_back(r);
        string msg = "Responder added: " + r->getName() + " (" + r->getType() + ")";
        cout << msg << endl;
        logger.log(msg);
    }

    // removeResponder: removes a responder by ID.
    // Only available (not currently busy) responders can be removed.
    void removeResponder(int responderId)
    {
        for (int i = 0; i < (int)responders.size(); i++)
        {
            if (responders[i]->getId() == responderId)
            {
                if (!responders[i]->isAvailable())
                {
                    cout << "ERROR: Responder #" << responderId
                         << " is currently assigned to an emergency and cannot be removed." << endl;
                    return;
                }

                string msg = "Responder removed: " + responders[i]->getName()
                           + " (" + responders[i]->getType() + ")";
                cout << msg << endl;
                logger.log(msg);

                delete responders[i];
                responders.erase(responders.begin() + i);
                return;
            }
        }

        cout << "ERROR: Responder #" << responderId << " not found." << endl;
    }

    // reportEmergency: logs a new emergency as "pending"
    void reportEmergency(const string& type, const string& priority)
    {
        Emergency newEmergency(nextEmergencyId, type, priority);
        emergencies.push_back(newEmergency);

        string msg = "Emergency #" + to_string(nextEmergencyId)
                   + " reported (" + type + ", " + priority + " priority)";
        cout << msg << endl;
        logger.log(msg);

        nextEmergencyId++;
    }

    // dispatch: matches PENDING emergencies to AVAILABLE units, high priority first.
    // Records the assignment in assignmentMap so resolveEmergency knows exactly
    // which responder to free (fixes the wrong-unit-freed bug).
    void dispatch()
    {
        cout << "\nInitiating Dispatch (Priority Order)...\n";

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
            string requiredType = emergencyToResponderType(emergencies[idx].getType());

            bool assigned = false;

            for (int j = 0; j < (int)responders.size(); j++)
            {
                if (responders[j]->isAvailable() && responders[j]->getType() == requiredType)
                {
                    responders[j]->setUnavailable();
                    emergencies[idx].setStatus(STATUS_ASSIGNED);

                    // Record exactly which responder index is handling this emergency
                    assignmentMap[emergencies[idx].getId()] = j;

                    string msg = "Dispatched " + responders[j]->getName()
                               + " to Emergency #" + to_string(emergencies[idx].getId())
                               + " [" + emergencies[idx].getPriority() + " priority]";
                    cout << "  " << msg << endl;
                    logger.log(msg);

                    assigned = true;
                    break;
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

    // resolveEmergency: marks a job done and frees the exact unit that was dispatched.
    // Uses assignmentMap to look up the correct responder index directly,
    // instead of guessing by type (which could free the wrong unit).
    void resolveEmergency(int emergencyId)
    {
        for (int i = 0; i < (int)emergencies.size(); i++)
        {
            if (emergencies[i].getId() == emergencyId)
            {
                if (emergencies[i].getStatus() != STATUS_ASSIGNED)
                {
                    cout << "Emergency #" << emergencyId
                         << " is not currently assigned (status: "
                         << emergencies[i].getStatus() << ")." << endl;
                    return;
                }

                emergencies[i].setStatus(STATUS_RESOLVED);

                // Look up the exact responder assigned to this emergency
                if (assignmentMap.count(emergencyId))
                {
                    int responderIdx = assignmentMap[emergencyId];
                    responders[responderIdx]->setAvailable();

                    string msg = "Emergency #" + to_string(emergencyId)
                               + " RESOLVED. " + responders[responderIdx]->getName()
                               + " is now available.";
                    cout << msg << endl;
                    logger.log(msg);

                    assignmentMap.erase(emergencyId); // clean up the map entry
                }
                else
                {
                    // Edge case: assigned status but no map entry (e.g. loaded from file)
                    string msg = "Emergency #" + to_string(emergencyId) + " marked resolved.";
                    cout << msg << endl;
                    logger.log(msg);
                }
                return;
            }
        }

        cout << "Emergency #" << emergencyId << " not found." << endl;
    }

    // saveState: uses FileHandler (Composition) to persist data
    void saveState()
    {
        vector<string> lines;

        // Serialise each responder
        for (int i = 0; i < (int)responders.size(); i++)
        {
            string type = responders[i]->getType();

            // AVAIL_TRUE ("1") = available, AVAIL_FALSE ("0") = busy
            string avail = responders[i]->isAvailable() ? AVAIL_TRUE : AVAIL_FALSE;

            string line = "RESPONDER," + type + ","
                        + to_string(responders[i]->getId()) + ","
                        + responders[i]->getName() + "," + avail;

            // Add type-specific field using downcasting
            if (type == TYPE_AMBULANCE)
            {
                Ambulance* a = dynamic_cast<Ambulance*>(responders[i]);
                if (a) line += "," + to_string(a->getCrewCount());
            }
            else if (type == TYPE_POLICE)
            {
                PoliceCar* p = dynamic_cast<PoliceCar*>(responders[i]);
                if (p) line += "," + p->getUnit();
            }
            else if (type == TYPE_FIRETRUCK)
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

        // Save next ID counter so it doesn't reset on reload
        lines.push_back("NEXT_ID," + to_string(nextEmergencyId));

        fileHandler.saveState(lines);
    }

    // loadState: reads from file and rebuilds the system state.
    // Note: assignmentMap is NOT restored from file. Any emergencies that were
    // "assigned" when saved will still show as assigned after load, but resolving
    // them will fall through to the edge-case path in resolveEmergency.
    void loadState()
    {
        // Clear current data before loading
        for (int i = 0; i < (int)responders.size(); i++)
            delete responders[i];
        responders.clear();
        emergencies.clear();
        assignmentMap.clear();

        vector<string> lines = fileHandler.loadState();

        if (lines.empty())
        {
            cout << "No data to load." << endl;
            return;
        }

        for (int i = 0; i < (int)lines.size(); i++)
        {
            stringstream ss(lines[i]);
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
                // AVAIL_TRUE ("1") means available, anything else means busy
                bool   avail = (fields[4] == AVAIL_TRUE);
                string extra = fields[5];

                Responder* r = nullptr;

                if (type == TYPE_AMBULANCE)
                    r = new Ambulance(id, name, stoi(extra));
                else if (type == TYPE_POLICE)
                    r = new PoliceCar(id, name, extra);
                else if (type == TYPE_FIRETRUCK)
                    r = new FireTruck(id, name, stoi(extra));

                if (r != nullptr)
                {
                    if (!avail) r->setUnavailable();
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

    // showAll: display all responders and emergencies with current status
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

    // showStats: quick statistics summary
    void showStats()
    {
        int availableCount = 0, busyCount = 0;
        int pendingCount = 0, assignedCount = 0, resolvedCount = 0;

        for (int i = 0; i < (int)responders.size(); i++)
        {
            if (responders[i]->isAvailable()) availableCount++;
            else busyCount++;
        }

        for (int i = 0; i < (int)emergencies.size(); i++)
        {
            string s = emergencies[i].getStatus();
            if      (s == STATUS_PENDING)  pendingCount++;
            else if (s == STATUS_ASSIGNED) assignedCount++;
            else if (s == STATUS_RESOLVED) resolvedCount++;
        }

        cout << "\n-------STATISTICS---------" << endl;
        cout << "Responders  : " << responders.size() << " total  (" << availableCount << " available, " << busyCount << " busy)" << endl;
        cout << "Emergencies : " << emergencies.size() << " total  (" << pendingCount << " pending, " << assignedCount << " assigned, " << resolvedCount << " resolved)" << endl;
        cout << "Log entries : " << logger.getCount() << endl;
        cout << "----------------\n" << endl;
    }

    // showLog: print the full activity log
    void showLog()
    {
        logger.showHistory();
    }

    // Destructor: free all heap-allocated Responder objects (no memory leaks)
    ~Dispatcher()
    {
        for (int i = 0; i < (int)responders.size(); i++)
            delete responders[i];
    }
};

#endif
