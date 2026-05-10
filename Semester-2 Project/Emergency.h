//  Emergency.h
//  Represents a single emergency event reported by a citizen.
#ifndef EMERGENCY_H
#define EMERGENCY_H
#include <iostream>
#include <string>
#include "Constants.h"
using namespace std;

class Emergency 
{
    // private, they can only be accessible through getters/setters
    int id;
    string type; // it can be: fire, medical, or crime
    string priority; // "high", "medium", "low"
    string status; // "pending", "assigned", "resolved"

public:
    // Constructor 
    Emergency(int id, string type, string priority) 
    {
        this->id = id;
        this->type = type;
        this->priority = priority;
        this->status = STATUS_PENDING; //every new emergency starts as pending
    }

    //  Getters
    int getId() const      
    { 
        return id;       
    }
    string getType() const     
    { 
        return type;     
    }
    string getPriority() const 
    { 
        return priority; 
    }
    string getStatus() const   
    { 
        return status;   
    }

    // Setter for status only 
    void setStatus(string newStatus) 
    {
        status = newStatus; // status changes as emergency progresses (pending -> assigned -> resolved)
    }

    // Displaying all details of emergency
    void display() const
    {
        cout << "  Emergency  " << id << "\n | Type: " << type << "\n | Priority: " << priority << "\n | Status: " << status << endl;
    }
};

#endif
