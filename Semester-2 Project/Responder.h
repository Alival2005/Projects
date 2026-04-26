//  Responder.h
#ifndef RESPONDER_H
#define RESPONDER_H
#include <iostream>
#include <string>
using namespace std;

//abstract base class
class Responder 
{
protected:
    int id;
    string name; // like Ambulance 1, Police 2
    bool available; // true = free to take a job
public:
   // Constructor
    Responder(int id, string name) 
    {
        this->id = id;
        this->name = name;
        this->available = true; // all responders start as available
    }
    //  Getters
    int getId() 
    { 
        return id;
    }
    string getName()     
    { 
        return name;      
    }
    bool isAvailable() 
    { 
        return available; 
    }

    //Marking responder as busy when dispatched to an emergency
    void setUnavailable() 
    { 
        available = false; 
    }
    
    //Marking responder as free again when emergency is resolved
    void setAvailable()   
    { 
        available = true;  
    }
    
	virtual string getType() = 0;//pure virtual function

    // Virtual display (child classes can override for extra info)
    virtual void display() 
    {
        cout << "  Responder #" << id << "\n | Name: "     << name << "\n | Type: "     << getType() << "\n | Status: "   << (available ? "Available" : "Busy") << endl;
    }

    // Virtual destructor ( prevents memory leak )
    virtual ~Responder() 
    {
        
    }
};

#endif
