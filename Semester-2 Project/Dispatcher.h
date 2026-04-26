//  Dispatcher.h
//  This is the brain of the system, it manages all responders and emergencies

#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "Responder.h"
#include "Emergency.h"
#include "Ambulance.h"
#include "PoliceCar.h"
#include "FireTruck.h"
#include <vector>
#include <string>

class Dispatcher 
{
    vector<Responder*> responders;
    vector<Emergency>  emergencies;

    int nextEmergencyId;   // auto increment ID for new emergencies

public:

    // Constructor
    Dispatcher() 
    {
        nextEmergencyId = 1;
    }

    // Add a new responder to the system; ambulance, police, firetruck
    void addResponder(Responder* r) 
    {
        responders.push_back(r);
        cout << "Responder added: ID " << r->getId() << " (" << r->getType() << ")" << endl;
    }

    // Citizen reports a new emergency
    void reportEmergency(string type, string priority) 
    {
        // Create emergency using the auto incrementing ID
        Emergency newEmergency(nextEmergencyId, type, priority);
        emergencies.push_back(newEmergency);
        
        cout << "Emergency reported: ID " << nextEmergencyId << " (" << type << ", " << priority << " priority)" << endl;
        
        nextEmergencyId++; // Increment for the next one
    }

    // Match pending emergencies with available responders
    void dispatch() 
    {
        cout << "\nInitiating Dispatch...\n";
        
        // Loop through emergencies by reference so we can modify their status
        for (int i = 0; i < emergencies.size(); i++) 
        {
            if (emergencies[i].getStatus() == "pending") 
            {
                bool assigned = false;
                
                // Map the emergency type to the required responder type
                string requiredType = "";
                if (emergencies[i].getType() == "medical") requiredType = "Ambulance";
                else if (emergencies[i].getType() == "crime") requiredType = "Police";
                else if (emergencies[i].getType() == "fire") requiredType = "FireTruck";
                
                // Find an available responder of the correct type
                for (int j = 0; j < responders.size(); j++) 
                {
                    if (responders[j]->isAvailable() && responders[j]->getType() == requiredType) 
                    {
                        // Update statuses using the methods defined in your headers
                        responders[j]->setUnavailable();
                        emergencies[i].setStatus("assigned");
                        
                        cout << "Dispatched " << responders[j]->getName() << " (ID: " << responders[j]->getId() << ") to Emergency ID: " << emergencies[i].getId() << endl;
                        
                        assigned = true;
                        break; // Stop searching once assigned
                    }
                }
                
                if (!assigned) 
                {
                    cout << "No available responder for Emergency ID: " << emergencies[i].getId() << " (" << emergencies[i].getType() << ")" << endl;
                }
            }
        }
        cout << "---------------------------" << endl;
    }

    // Show all emergencies and their current status
    void showAll() 
    {
        cout << "\n=== SYSTEM STATUS ===\n";
        cout << "--- Responders ---\n";
        if (responders.empty()) cout << "  No responders in system.\n";
        for (int i = 0; i < responders.size(); i++) 
        {
            responders[i]->display(); // Polymorphism
        }
        
        cout << "\n--- Emergencies ---\n";
        if (emergencies.empty()) cout << "  No emergencies reported.\n";
        for (int i = 0; i < emergencies.size(); i++) 
        {
            emergencies[i].display();
        }
        cout << "=====================\n\n";
    }

    ~Dispatcher()
    {
        for (Responder* r : responders)
        {
            delete r;
        }
    }
};

#endif
