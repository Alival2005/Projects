//  Emergency.h
//  Represents a single emergency event reported by a citizen.
#ifndef EMERGENCY_H
#define EMERGENCY_H
#include <iostream>
#include <string>
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
        this->status = "pending"; //every new emergenecy starts as pending
    }

    //  Getters
    int getId()       
    { 
        return id;       
    }
    string getType()     
    { 
        return type;     
    }
    string getPriority() 
    { 
        return priority; 
    }
    string getStatus()   
    { 
        return status;   
    }

    // Setter for status only 
    void setStatus(string newStatus) 
    {
        status = newStatus; // status changes as emergency progresses (pending -> assigned -> resolved)
    }

    // Displaying all details of emergency
    void display() 
    {
        cout << "  Emergency  " << id << "\n | Type: "     << type << "\n | Priority: " << priority << "\n | Status: "   << status << endl;
    }
};

#endif
