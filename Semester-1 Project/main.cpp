// Karachi Metro System
// A simple console application to manage metro ticketing and card system
// By =  2025079 AHMED AMAN ULLAH KHAN
//       2025421 MOHID SALMAN
//       2025806 SYED ALI ABBAS ABIDI

// card_num.dat will store card numbers and PINs
// transaction.dat will store ticket purchase transactions

#include <iostream> // include iostream for input and output
#include <string> // include string for using string data type
#include <fstream> // include fstream for file handling
#include <cstdlib> // include cstdlib for general utilities
using namespace std; // use standard namespace

// Global Variables: Declaring stations and other necessary variables
string stations[5] = {"DHA Phase 8", "Clifton", "Civil Lines", "Bahadurabad", "Scheme 33"};
int total_stations = 5, last = 1000;

// Function to display available stations
void show_stations()
{
    cout << "STATIONS";

    // Loop through and display each station
    for (int i = 0; i < total_stations; i++)
    {
        cout << "\n" << (i + 1) << ". " << stations[i];
    }
}

// Function to generate a new card number
int generate_card_num()
{
    // Read the last card number from the file
    ifstream fin("card_num.dat");
    int pin;
    fin.close();

    // Increment and return the new card number
    return last = last + 1;
}

// Function to create a new card
void create_card()
{
    // Open file in append mode to store new card details
    ofstream fout("card_num.dat", ios::app);
    int card_num = generate_card_num();
    string pin;

    // Taking user input for PIN
    cout << "Enter a 4-digit PIN for your card: ";
    cin >> pin;
    fout << card_num << " " << pin << endl;
    fout.close();
    cout << "Card created successfully! Your card number is: " << card_num << endl;
}

// Function to login using card number and PIN
bool login(int &card)
{
    // Taking user input for card number and PIN
    int card_num, pin;
    cout << "Enter your card number: ";
    cin >> card_num;
    cout << "Enter your PIN: ";
    cin >> pin;

    // Validating card number and PIN from the file
    ifstream fin("card_num.dat");
    int stored_card, stored_pin;

    // Reading through the file to find matching credentials
    while (fin >> stored_card >> stored_pin)
    {
        if (stored_card == card_num && stored_pin == pin)
        {
            card = card_num;
            cout << "Login successful!" << endl;
            fin.close();
            return true;
        }
    }
    fin.close();
    cout << "Invalid card number or PIN." << endl;
    return false;
}

// Function to buy a ticket
void buy_ticket(bool is_logged_in, int card_num)
{
    // Calling show_stations function to display stations
    show_stations();
    int from, to;

    // Taking user input for departure and destination stations
    cout << "\nEnter your departure station number: ";
    cin >> from;
    cout << "\nEnter your destination station number: ";
    cin >> to;

    // Calculate ticket price based on distance
    int hop = abs(to - from);
    int price = hop * 50;
    
    // Apply discount if logged in
    if (is_logged_in)
    {
        price = price - (price * 0.15);
    }
    cout << "Ticket price: " << price << " PKR\n";

    // Record the transaction
    ofstream fout("transaction.dat", ios::app);
    if (is_logged_in)
    {
        fout << card_num << " " << from << " " << to << " " << price << endl;
    }
    else
    {
        fout << " - " << from << " " << to << " " << price << endl;
    }
    fout.close();
    cout << "Ticket purchased successfully!" << endl;
}

int main()
{
    // Variable Declarations
    int choice;
    bool logged_in = false;
    int card_num = 0;

    // Main Menu Loop
    while (true)
    {
        cout << "\nKARACHI METRO SYSTEM\n";
        cout << "1. Buy Ticket\n";
        cout << "2. Create Card\n";
        cout << "3. Login\n";
        cout << "4. Exit\n";
        cout << "\nEnter choice: ";
        cin >> choice;
        switch (choice)
        {
            case 1:
            {
                buy_ticket(logged_in, card_num);
                break;
            }
            case 2:
            {
                create_card();
                break;
            }
            case 3:
            {
                logged_in = login(card_num);
                break;
            }
            case 4:
            {
                return false;
                break;
            }
            default:
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }
    return 0;
}