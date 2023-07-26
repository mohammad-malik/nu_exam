#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <vector>
#include "Classes.h"
using namespace std;

// TODO: analytics

int main()
{

    
    // Creating logins
    srand(time(NULL));
    generateStudentLogins();
    generateTeacherLogins();

    // Loading teachers and students
    string TeacherDataFileName = "requiredFiles/Teacher_Details.csv";
    string StudentDataFileName = "requiredFiles/Student_Logins.csv";
    
    // Creating quiz object for if teacher wants to create a quiz
    Course loggedInTeacherCourse;
    Quiz createdQuiz;

    // Making program run until user wants to exit
    while(true)
    {
        clearScreen();
        
        // Logging in
        char isStudent;
        cout << "Welcome to FAST NU's Quiz Management System!\n" << endl;
        cout << "Are you a student? (y/n): ";
        cin >> isStudent;   
        if (isStudent == 'y'|| isStudent == 'Y')
            studentActions(loggedInTeacherCourse, createdQuiz);
        else if (isStudent == 'n' || isStudent == 'N') 
            teacherActions(loggedInTeacherCourse, createdQuiz);
        else
        {
            cout << "Invalid input!" << endl;
            continue;
        }
        clearScreen();

        // Asking user if they want to exit
        cout << "Do you want to exit? (y/n): ";
        char userExit;
        cin >> userExit;

        // If user wants to exit, then exit
        if(userExit == 'y' || userExit == 'Y')
        {
            clearScreen();
            cout << "Thank you for using FAST NU's Quiz Management System!" << endl;
            
            // If a quiz was created, deleting it
            if (createdQuiz.getTotalQuestions() != 0)
                delete &createdQuiz;
            break;
        }
    }   
    return 0;
}