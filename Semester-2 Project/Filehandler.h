// FileHandler.h
// Handles saving and loading the system state to/from a plain text file.
#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
using namespace std;
// FileHandler uses Composition with Dispatcher  it is a helper tool
// owned and used exclusively by the Dispatcher class
class FileHandler
{
    string filename;  // the file this handler reads from / writes to
public:
    // Constructor to set the filename once reuse for save and load
    FileHandler(string fname)
    {
        this->filename = fname;
    }
    // saveState writes every responder and emergency to file
    bool saveState(vector<string>& lines)
    {
        ofstream outFile(filename);

        if (!outFile.is_open())
        {
            cout << "ERROR: Could not open file for writing: " << filename << endl;
            return false;
        }
      // Write each line provided by Dispatcher to the file
        for (int i = 0; i < (int)lines.size(); i++)
        {
            outFile << lines[i] << "\n";
        }
        outFile.close();
        cout << "State saved to: " << filename << endl;
        return true;
    }
    // loadState reads the file and returns each line as a string
    vector<string> loadState()
    {
        vector<string> lines; //  Dispatcher then parses those lines to rebuild its state
        ifstream inFile(filename);

        if (!inFile.is_open())
        {
            cout << "ERROR: Could not open file for reading: " << filename << endl;
            return lines; // return empty vector
        }
        string line;
        while (getline(inFile, line))
        {
            // Skip blank lines
            if (!line.empty())
            {
                lines.push_back(line);
            }
        }
        inFile.close();
        cout << "State loaded from: " << filename << endl;
        return lines;
    }
     // Returns the filename this handler is attached to
    string getFilename()
    {
        return filename;
    }
};

#endif
