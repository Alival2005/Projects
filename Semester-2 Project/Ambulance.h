//  Ambulance.h
#ifndef AMBULANCE_H
#define AMBULANCE_H
#include "Responder.h"

//child class of responder(inheritence)
class Ambulance : public Responder 
{
    int crewCount;   // number of paramedics on board
public:
  // Constructor
    Ambulance(int id, string name, int crew) : Responder(id, name) 
    {
        this->crewCount = crew;
    }
  // Override getType(), here polymorphism is in action
    string getType() const override 
    {
        return "Ambulance";
    }

    // Override display to also show crew size
    void display() const override 
    {
        cout << "  Ambulance #" << getId() << "\n | Name: " << getName() << "\n | Crew: " << crewCount << "\n | Status: " << (isAvailable() ? "Available" : "Busy") << endl;
    }
    int getCrewCount() const
    { 
	      return crewCount; 
    }
};

#endif
