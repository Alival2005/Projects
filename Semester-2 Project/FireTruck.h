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
    string getType() const override 
    {
        return "FireTruck";
    }

    // Override display to also show tank capacity
    void display() const override 
    {
        cout << "  FireTruck #" << getId() << "\n | Name: " << getName() << "\n | Water Tank: " << waterCapacity << "L" << "\n | Status: " << (isAvailable() ? "Available" : "Busy") << endl;
    }

    int getWaterCapacity() const
    { 
	    return waterCapacity; 
    }
};

#endif
