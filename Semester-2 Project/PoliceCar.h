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
    string getType() override 
    {
        return "Police";
    }

    // Override display to also show unit info
    void display() override 
    {
        cout << "  PoliceCar #" << id << "\n | Name: "     << name << "\n | Unit: "     << unit << "\n | Status: "   << (available ? "Available" : "Busy") << endl;
    }
    string getUnit() 
    { 
        return unit; 
    }
};

#endif
