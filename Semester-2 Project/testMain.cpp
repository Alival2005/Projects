#include <iostream>
using namespace std;
#include "Dispatcher.h"

int main() 
{
    Dispatcher system;

    // Adding Responders
    system.addResponder(new Ambulance(101, "Ambulance-1", 3));
    system.addResponder(new Ambulance(104, "Ambulance-2", 2));
    system.addResponder(new PoliceCar(102, "Police-1", "Unit-7"));
    system.addResponder(new FireTruck(103, "FireTruck-1", 5000));

    cout << endl;

    // Reporting Emergencies with its type and its priority
    system.reportEmergency("fire", "high");
    system.reportEmergency("medical", "medium");
    system.reportEmergency("crime", "high");
    
    // This second medical emergency will fail to dispatch because Rescue-1 is busy
    system.reportEmergency("medical", "low"); 

    // Dispatching
    system.dispatch();

    // Show Status to verify statuses changed to Busy and assigned
    system.showAll();
}