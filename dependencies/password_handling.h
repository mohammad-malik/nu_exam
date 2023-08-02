#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <future>
#define WINDOWS     // used for clearScreen()
using namespace std;

void deleteCharacters(string &stringToModify, int indexToStart, int indexesToDelete)
{
    string newString = "";
    for (int i = 0; stringToModify[i] != '\0'; i++)
    {
        if (i < indexToStart || i >= indexToStart + indexesToDelete)
            newString += stringToModify[i];
    }
    stringToModify = newString;
}
bool validatePassword(string password)
{
    // Declaring variables for checks
    bool hasUppercase = false, hasLowercase = false, hasDigit = false;
    int characterCount;

    // Counting characters nd checking for conditions
    for (characterCount = 0; password[characterCount] != '\0'; characterCount++)
    {
        if (password[characterCount] >= 65 && password[characterCount] <= 91)
            hasUppercase = true;
        if (password[characterCount] >= 97 && password[characterCount] <= 123)
            hasLowercase = true;
        if (password[characterCount] >= 48 && password[characterCount] <= 57)
            hasDigit = true;
    }

    // Printing why password is invalid (if it is)
    if ((characterCount < 6) || (characterCount > 50) || (hasDigit == 0) || (hasLowercase == 0) || (hasUppercase == 0))
        return false;

    return true;
}
string passwordGenerator()
{
    // Creating strings of all possible characters
    string caps = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string smalls = "abcdefghijklmnopqrstuvwxyz";
    string numbers = "1234567890";
    string symbols = "!@#$%^&*()_+-=[]{};':\"./<>?`~";

    string password = "";

    // Randomizing the length of the password
    int length = rand() % 9 + 8;

    // Looping till the password is 'length' characters long
    for (int i = 0; i < length; i++)
    {
        // Getting a random number between 0 and 3
        int type = rand() % 4;
        int randomIndex = 0;

        // Ensuring that the password has atleast one of each type
        if (i < 4)
            type = i;
        
        // Adding a random character of the type to the password
        if (type == 0)
        {
            randomIndex = rand() % 26;
            password += caps[randomIndex];
        }  
        else if (type == 1)
        {
            randomIndex = rand() % 26;
            password += smalls[randomIndex];
        }  
        else if (type == 2)
        {
            randomIndex = rand() % 10;
            password += numbers[randomIndex];
        }  
        else if (type == 3)
        {
            randomIndex = rand() % 29;
            password += symbols[randomIndex];
        }  
    }
    return password;
}
string findCourseCode(string courseName)
{
    // Reading file with teacher logins
    ifstream inputFile("data/Course_Registration_Data_Sheet.csv");
    string line;
    
    // Storing into 2d vector
    vector<vector<string>> data;
    while (getline(inputFile, line))
    {
        stringstream lineStream(line);
        string cell;
        vector<string> row;
        while (getline(lineStream, cell, ','))
            row.push_back(cell);
        data.push_back(row);
    }

    // Searching for course name
    for (int i = 0; i < data.size(); i++)
    {
        if (data[0][i] == courseName)
            return data[1][i];
    }
    return "";
}

void clearScreen()
{
  #ifdef WINDOWS
    system("cls");
    cout << endl;
  #else
    system("clear");
    cout << endl;
  #endif
}
