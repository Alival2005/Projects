// testMain.cpp
// Full automated test — verifies all features from Day 1 through Day 5
#include <iostream>
#include "Dispatcher.h"
using namespace std;

int main()
{
    cout << "========== TEST 1: Adding Responders ==========" << endl;
    Dispatcher system;
    system.addResponder(new Ambulance(101, "Ambulance-1", 3));
    system.addResponder(new Ambulance(104, "Ambulance-2", 2));
    system.addResponder(new PoliceCar(102, "Police-1", "Unit-7"));
    system.addResponder(new FireTruck(103, "FireTruck-1", 5000));

    cout << "\n========== TEST 2: Reporting Emergencies (mixed priority) ==========" << endl;
    system.reportEmergency("fire",    "low");     // ID 1 - should dispatch LAST
    system.reportEmergency("medical", "high");    // ID 2 - should dispatch FIRST
    system.reportEmergency("crime",   "medium");  // ID 3 - should dispatch THIRD
    system.reportEmergency("medical", "high");    // ID 4 - should dispatch SECOND

    cout << "\n========== TEST 3: Priority Dispatch ==========" << endl;
    // Expected: ID 2 (high), ID 4 (high), ID 3 (medium), ID 1 (low)
    system.dispatch();

    cout << "\n========== TEST 4: Show Status ==========" << endl;
    system.showAll();

    cout << "\n========== TEST 5: Stats ==========" << endl;
    system.showStats();

    cout << "\n========== TEST 6: Resolve Emergency #2 ==========" << endl;
    system.resolveEmergency(2); // Ambulance-1 should become available again

    cout << "\n========== TEST 7: Report + Re-Dispatch ==========" << endl;
    system.reportEmergency("medical", "high"); // ID 5 -> Ambulance-1 should take it
    system.dispatch();

    cout << "\n========== TEST 8: Save State to File ==========" << endl;
    system.saveState(); // writes to dispatch_state.txt

    cout << "\n========== TEST 9: Load State from File ==========" << endl;
    Dispatcher system2;  // fresh dispatcher
    system2.loadState(); // should restore same state
    system2.showAll();

    cout << "\n========== TEST 10: Full Activity Log ==========" << endl;
    system.showLog();

    cout << "\n========== All Tests Complete ==========" << endl;
    return 0;
}
