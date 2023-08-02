#include "password_handling.h"

bool studentAuth(string rollNumber, string password)
{
    // Read login data from file
    ifstream inputFile("data/Student_Logins.csv");
    string line;
    while (getline(inputFile, line))
    {
        stringstream lineStream(line);
        string cell;
        getline(lineStream, cell, ',');
        if (cell == rollNumber)
        {
            getline(lineStream, cell, ',');
            getline(lineStream, cell, ',');
            if (cell == password)
                return true;
            else
                return false;
        }
    }
    return false;
}
string findStudentName(string rollNumber)
{
    // Reading file with student logins
    ifstream inputFile("data/Student_Logins.csv");
    string line;
    while (getline(inputFile, line))
    {
        stringstream lineStream(line);
        string cell;
        getline(lineStream, cell, ',');
        if (cell == rollNumber)
        {
            getline(lineStream, cell, ',');
            return cell;
        }
    }
    return "";
}
void generateStudentLogins()
{
    ifstream readFile;
    readFile.open("data/Course_Registration_Data_Sheet.csv");
    if (!readFile)
    {
        cout << "Error in opening readFile";
        return;
    }

    string line, cell;
    vector<string> fileContents;

    // Skip the first 2 lines
    getline(readFile, line);
    getline(readFile, line);
    
    while (getline(readFile, line))
    {
        stringstream lineStream(line);
        getline(lineStream, cell, ',');
        
        // Storing only the roll number and first name
        for (int i = 0; i < 2; i++)
        {
            getline(lineStream, cell, ',');
            fileContents.push_back(cell);
        }
    }
    
    ofstream writeFile;
    writeFile.open("data/Student_Logins.csv");
    writeFile << "Roll Number,Name,Password" << endl;
    for (int i = 0; i < fileContents.size(); i += 2)
        writeFile << fileContents[i] << ',' << fileContents[i+1] << ',' << passwordGenerator() << endl;
    
    readFile.close();
    writeFile.close();
}
vector<string> findStudentsInCourse(string courseToCheck)
{
    // Reading Course Registration File and storing into 2D vector
    ifstream inputFile("data/Course_Registration_Data_Sheet.csv");
    string line;
    vector<vector<string>> fileContents;
    while (getline(inputFile, line))
    {
        stringstream lineStream(line);
        string cell;
        vector<string> row;

        while (getline(lineStream, cell, ','))
            row.push_back(cell);

        fileContents.push_back(row);
    }
    inputFile.close();

    // Finding the course and storing the students in a string vector
    vector<string> studentRollNumbers;
    for (int i = 3; i < fileContents[0].size(); i++)
    {
        if (fileContents[0][i] == courseToCheck)
        {
            for (int j = 0; j < fileContents.size(); j++)
            {
                if (fileContents[j][i] == "1")
                    studentRollNumbers.push_back(fileContents[j][1]);
            }
        }
    }
    return studentRollNumbers;
}

bool teacherAuth(string username, string password)
{
    // Read login data from file
    ifstream inputFile("data/Teacher_Details.csv");
    if (!inputFile.is_open())
    {
        cout << "Failed to open file!" << endl;
        return false;
    }

    string line;
    while (getline(inputFile, line))
    {
        stringstream lineStream(line);
        string cell;
        getline(lineStream, cell, ',');
        if (cell == username)
        {
            getline(lineStream, cell, ',');
            getline(lineStream, cell, ',');
            getline(lineStream, cell, ',');
            if (cell == password)
                return true;
            else
                return false;
        }
    }
    return false;
}
string findTeacherName(string username)
{
    // Reading file with teacher logins
    ifstream inputFile("data/Teacher_Details.csv");
    string line;
    while (getline(inputFile, line))
    {
        stringstream lineStream(line);
        string cell;
        getline(lineStream, cell, ',');
        if (cell == username)
        {
            getline(lineStream, cell, ',');
            return cell;
        }
    }
    return "";
}
string findTeacherCourse(string username)
{
    // Reading file with teacher logins
    ifstream inputFile("data/Teacher_Details.csv");
    string line;
    while (getline(inputFile, line))
    {
        stringstream lineStream(line);
        string cell;
        getline(lineStream, cell, ',');
        if (cell == username)
        {
            getline(lineStream, cell, ',');
            getline(lineStream, cell, ',');
            return cell;
        }
    }
    return "";
}
void generateTeacherLogins()
{
    ifstream readFile;
    readFile.open("data/Teacher_Details.csv");
    if (!readFile)
    {
        cout << "Error in opening readFile";
        return;
    }

    string line, cell;
    vector<string> fileContents;

    // Skip the first line
    getline(readFile, line);
    
    while (getline(readFile, line))
    {
        stringstream lineStream(line);
        
        for (int i = 0; i < 3; i++)
        {
            getline(lineStream, cell, ',');
            fileContents.push_back(cell);
        }
    }
    
    ofstream writeFile;
    writeFile.open("data/Teacher_Details.csv");
    writeFile << "Username,Name,Course Taught,Password" << endl;
    for (int i = 0; i < fileContents.size() - 1; i += 3)
        writeFile << fileContents[i] << ',' << fileContents[i+1] << ',' << fileContents[i+2] << ',' << passwordGenerator() << endl;
    
    readFile.close();
    writeFile.close();
}
string findCourseTeacher(string courseName)
{
    ifstream inputFile("data/Teacher_Details.csv");
    string line;
    vector<vector<string>> fileContents;
    while (getline(inputFile, line))
    {
        stringstream lineStream(line);
        string cell;
        vector<string> row;

        while (getline(lineStream, cell, ','))
            row.push_back(cell);

        fileContents.push_back(row);
    }
    inputFile.close();

    for (int i = 0; i < fileContents.size(); i++)
    {
        if (fileContents[i][2] == courseName)
            return fileContents[i][1];
    }
    return "Not Found";
}
