//  Responder.h
#ifndef RESPONDER_H
#define RESPONDER_H
#include <iostream>
#include <string>
using namespace std;

//abstract base class
class Responder 
{
private:
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
    int getId() const
    { 
        return id;
    }
    string getName() const
    { 
        return name;      
    }
    bool isAvailable() const
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
    
    virtual string getType() const = 0; // pure virtual function

    // Virtual display (child classes can override for extra info)
    virtual void display() const
    {
        cout << "  Responder #" << getId() << "\n | Name: " << getName() << "\n | Type: " << getType() << "\n | Status: " << (isAvailable() ? "Available" : "Busy") << endl;
    }

    // Virtual destructor ( prevents memory leak )
    virtual ~Responder() 
    {
        
    }
};

#endif
