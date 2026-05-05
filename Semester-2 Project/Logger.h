// Logger.h
// Keeps a record of every important action taken in the system
#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Logger
{
    // Each log entry is just a plain string message
    vector<string> history;

public:
    // Called whenever something worth remembering happens
    void log(string message)
    {
        history.push_back(message);
    }
    // Print every log entry to the screen
    void showHistory()
    {
        cout << "\n------ DISPATCH LOG ------" << endl;

        if (history.empty())
        {
            cout << "  (No activity yet)" << endl;
        }
        else
        {
            // Print each entry with a line number so it's easy to read
            for (int i = 0; i < history.size(); i++)
            {
                cout << "  [" << i + 1 << "] " << history[i] << endl;
            }
        }

        cout << "------------------" << endl;
    }
     // Returns total number of log entries useful for stats
    int getCount()
    {
        return history.size();
    }
};
#endif
