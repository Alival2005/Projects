//  FireTruck.h
#ifndef FIRETRUCK_H
#define FIRETRUCK_H
#include "Responder.h"

// Child class of Responder
class FireTruck : public Responder 
{
    int waterCapacity;   // water tank size in litres
public:
    // Constructor
    FireTruck(int id, string name, int capacity) : Responder(id, name) 
    {
        this->waterCapacity = capacity;
    }

    // Overriding getType()
    string getType() override 
    {
        return "FireTruck";
    }

    // Override display to also show tank capacity
    void display() override 
    {
        cout << "  FireTruck #"   << id << "\n | Name: "       << name << "\n | Water Tank: " << waterCapacity << "L" << "\n | Status: "     << (available ? "Available" : "Busy") << endl;
    }

    int getWaterCapacity() 
    { 
	    return waterCapacity; 
    }
};

#endif
