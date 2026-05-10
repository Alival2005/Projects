// Constants.h
// Central place for all string constants used across the system.
// Using these instead of raw string literals prevents typos from causing
// silent bugs like Ambulence vs Ambulance would never be caught
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
using namespace std;

// Responder types 
// Must match getType() return values in Ambulance/PoliceCar/FireTruck
const string TYPE_AMBULANCE  = "Ambulance";
const string TYPE_POLICE     = "Police";
const string TYPE_FIRETRUCK  = "FireTruck";

// Emergency categories (used in report command and dispatch mapping) ---
const string EMERG_MEDICAL   = "medical";
const string EMERG_CRIME     = "crime";
const string EMERG_FIRE      = "fire";

// Priority levels 
const string PRIORITY_HIGH   = "high";
const string PRIORITY_MEDIUM = "medium";
const string PRIORITY_LOW    = "low";

// Emergency statuses 
const string STATUS_PENDING  = "pending";
const string STATUS_ASSIGNED = "assigned";
const string STATUS_RESOLVED = "resolved";

// File save/load tokens
// "1" means available, "0" means busy in dispatch_state.txt
const string AVAIL_TRUE      = "1";
const string AVAIL_FALSE     = "0";

#endif
