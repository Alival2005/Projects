//  PoliceCar.h
#ifndef POLICECAR_H
#define POLICECAR_H
#include "Responder.h"

// child class of responder
class PoliceCar : public Responder 
{
    string unit; // police unit number

public:
    // Constructor
    PoliceCar(int id, string name, string unit) : Responder(id, name) 
    {
        this->unit = unit;
    }

    // Overriding getType()
    string getType() const override 
    {
        return "Police";
    }

    // Override display to also show unit info
    void display() const override 
    {
        cout << "  PoliceCar #" << getId() << "\n | Name: " << getName() << "\n | Unit: " << unit << "\n | Status: " << (isAvailable() ? "Available" : "Busy") << endl;
    }
    string getUnit() const
    { 
        return unit; 
    }
};

#endif
