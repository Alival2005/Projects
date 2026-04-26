#include <iostream>
#include <string>
#include "Dispatcher.h"
using namespace std;

// unction to keep the loop clean
void displayMenu() 
{
    cout << "\n=== EMERGENCY DISPATCH MENU ===" << endl;
    cout << " Commands available:" << endl;
    cout << "  > 1. Add Responder" << endl;
    cout << "  > 2. Report Emergency" << endl;
    cout << "  > 3. Dispatch" << endl;
    cout << "  > 4. Show" << endl;
    cout << "  > 5. Exit" << endl;
    cout << "-------------------------------" << endl;
    cout << "Enter command: ";
}

int main() 
{
    Dispatcher system;
    int command;

    cout << "Dispatch Center Online.\n";

    // Create the infinite loop
    while (true) 
    {
        displayMenu();
        cin >> command;

        // ADDING RESPONDER COMMAND
        if (command == 1) 
        {
            string name;
            int id, type;
            
            cout << "\n > 1. Ambulance \n > 2. Police \n > 3. FireTruck \nEnter type: " << endl;
            cin >> type;
            cout << "Enter ID (number): ";
            cin >> id;
            cout << "Enter Name (no spaces, e.g., Rescue-1): ";
            cin >> name;

            // Route to the correct child class constructor based on user input
            if (type == 1) 
            {
                int crew;
                cout << "Enter crew count: ";
                cin >> crew;
                system.addResponder(new Ambulance(id, name, crew));
            } 
            else if (type == 2) 
            {
                string unit;
                cout << "Enter unit name (e.g., Unit-7): ";
                cin >> unit;
                system.addResponder(new PoliceCar(id, name, unit));
            } 
            else if (type == 3) 
            {
                int capacity;
                cout << "Enter water capacity in L: ";
                cin >> capacity;
                system.addResponder(new FireTruck(id, name, capacity));
            } 
            else 
            {
                cout << "Invalid responder type! Must select from 1 - 3." << endl;
            }
        } 
        
        // REPORTING EMERGENCY COMMAND
        else if (command == 2) 
        {
            string type, priority;
            
            cout << "Enter emergency type (medical / crime / fire): ";
            cin >> type;
            
            // Quick validation to ensure they type the exact strings Day 2 expects
            if (type != "medical" && type != "crime" && type != "fire") 
            {
                cout << "Invalid emergency type! Must be medical, crime, or fire." << endl;
                continue; // Skips the rest of the loop and asks for a new command
            }

            cout << "Enter priority (high / medium / low): ";
            cin >> priority;
            
            system.reportEmergency(type, priority);
        } 
        
        // DISPATCHING COMMAND
        else if (command == 3) 
        {
            system.dispatch();
        } 
        
        // SHOWING COMMAND
        else if (command == 4) 
        {
            system.showAll();
        } 
        
        // EXITING COMMAND
        else if (command == 5) 
        {
            cout << "Shutting down dispatch system." << endl;
            break; // This breaks out of the while(true) loop and ends the program
        } 
        
        // HANDLING INVALID COMMANDS
        else 
        {
            cout << "Invalid command. Please choose exactly as shown in the menu." << endl;
        }
    }
}