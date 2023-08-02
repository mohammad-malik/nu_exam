#include "student_teacher_dependencies.h"

// classes and functions using them
class PasswordField
{
private:
    const string placeholder;       // the placeholder to be shown when user is not typing
    const int precedings;           // the number of characters to be erased before the placeholder (always the number of characters in the placeholder except the last one)
    const int trailers;             // the number of characters to be erased after the placeholder (always 1)

    // Reprinting the placeholder
    void refreshPlaceholder()
    {
        // Erasing the placeholder and the input before it by moving the cursor
        for (int i = 0; i != precedings; ++i)
            cout << '\b';

        cout << placeholder;
        
        // Erasing the input after the placeholder by moving the cursor
        for (int i = 0; i != trailers; ++i)
            cout << '\b';

        // Flushing the output buffer to make the changes visible to the user
        cout << flush;
    }

public:
    // Constructor set to default values as this is always the same
    PasswordField(): placeholder("* Press the enter to confirm "), precedings(placeholder.size() - 1), trailers(1) {}

    // Overloading the getline function for the PasswordField class
    string getline()
    {
        // Creating a 'future' typecast to string to listen to the user input 'asynchronously' meaning that the program will not wait for the user to enter the input and will continue executing the code
        future<string> listener = 
        async([]()
            {
                string answer;
                std::getline(cin, answer);
                return answer; 
            });

        // Printing the placeholder
        for (int i = 0; i != precedings; ++i)
            cout << placeholder[i];

        // Flushing the output buffer to make the changes visible to the user
        cout << flush;

        // Waiting for the user to enter the input and then refreshing the placeholder to hide the input with asterisks
        while (listener.wait_for(chrono::milliseconds(10)) != future_status::ready)
            refreshPlaceholder();

        // Returning the user input after the placeholder is refreshed
        return listener.get();
    }
};

class Time
{
private:
    int hour;
    int minute;

public:
    Time()
    {
        this->hour = 0;
        this->minute = 0;
    }
    Time(int hour, int minute) 
    {
        // Validation checks
        if (hour < 0 || hour > 23)
        {
            cout << "Invalid hour!" << endl;
            return;
        }
        if (minute < 0)
        {
            cout << "Invalid minute!" << endl;
            return;
        }

        // Adjusting and setting the time
        this->hour = hour;

        if (minute > 59)
        {
            this->hour++;
            this->minute -= 60;
        }
        this->minute = minute;
    }
    void setHour(int hour)
    {
        // Validation check
        if (hour < 0 || hour > 23)
        {
            cout << "Invalid hour!" << endl;
            return;
        }
        this->hour = hour;
    }
    void setMinute(int minute)
    {
        // Validation check
        if (minute < 0)
        {
            cout << "Invalid minute!" << endl;
            return;
        }

        // Adjusting the time for correctness
        if (minute > 59)
        {
            this->hour++;
            minute -= 60;
        }
        this->minute = minute;
    }

    int getHour()
    {
        return this->hour;
    }
    int getMinute()
    {
        return this->minute;
    }
    int getTime()
    {
        return this->hour * 60 + this->minute;
    }

    int operator-(Time time)
    {
        int timeDifference = (this->getTime() - time.getTime());
        if (timeDifference < 0)
        {
            cout << "Invalid time difference!" << endl;
            return -1;
        }
        return timeDifference;
    }

    friend ostream &operator<<(ostream &output, const Time &time);
    friend istream &operator>>(istream &input, Time &time);

    bool operator>(Time time)
    {
        if (this->getTime() > time.getTime())
        {
            return true;
        }
        return false;
    }
};

ostream &operator<<(ostream &output, const Time &timeToOutput)
{
    output << timeToOutput.hour << ":";
    if (timeToOutput.minute < 10)
        output << "0";

    output << timeToOutput.minute;
    return output;
}
istream &operator>>(istream &input, Time &timeToInput)
{
    cout << "Enter hours: ";
    input >> timeToInput.hour;
    cout << "Enter minutes: ";
    input >> timeToInput.minute;

    // Input validation
    while (timeToInput.hour < 0 || timeToInput.hour > 23)
    {
        cout << "Invalid hour!" << endl;
        input >> timeToInput.hour;
    }
    while (timeToInput.minute < 0)
    {
        cout << "Invalid minute!" << endl;
        input >> timeToInput.minute;
    }

    // Adjusting the time for correctness
    if (timeToInput.minute > 59)
    {
        timeToInput.hour++;
        timeToInput.minute -= 60;
    }

    return input;
}

class User
{
protected:
    string name;
    string password;

public:
    User();
    User(string name, string password)
    {
        this->name = name;
        this->password = password;
    }

    string getName() const
    {
        return name;
    }
    string getPassword() const
    {
        return password;
    }

    void setName(string name)
    {
        this->name = name;
    }
    void inputPassword()
    {
        PasswordField passwordField;
        cin.ignore();
        cout << "Enter your password: ";
        this->password = passwordField.getline();

        if (validatePassword(password))
        {
            this->password = password;
            overwritePassword(this->password);
        }
        else
        {
            cout << "Invalid password. Please try again." << endl;
            this->password = passwordField.getline();
        }
    }

    void setPassword(string password)
    {
        if (validatePassword(password))
            this->password = password;
        else
        {
            cout << "Invalid password. Please try again." << endl;
            inputPassword();
        }
    }

    virtual void overwritePassword(string password) = 0;
};

class Question
{
protected:
    string courseOfQuestion;
    string questionText;
    string subtopic;
    bool studentIsCorrect;

public:
    Question() {}
    Question(string courseOfQuestion, string questionText, string subtopic, bool isCorrect) : courseOfQuestion(courseOfQuestion)
    {
        this->questionText = questionText;
        this->subtopic = subtopic;
        this->studentIsCorrect = false;
    }

    string getCourseOfQuestion() const
    {
        return courseOfQuestion;
    }
    string getQuestionText() const
    {
        return questionText;
    }
    string getSubtopic() const
    {
        return subtopic;
    }
    bool getStudentIsCorrect() const
    {
        return studentIsCorrect;
    }

    void setCourseOfQuestion(string courseOfQuestion)
    {
        this->courseOfQuestion = courseOfQuestion;
    }
    void setQuestionText(string questionText)
    {
        this->questionText = questionText;
    }
    void setSubtopic(string subtopic)
    {
        this->subtopic = subtopic;
    }

    virtual void print() const = 0;
    virtual bool askQuestion() = 0;
    virtual void writeQuestion(string fileName) = 0;
};

class MCQ : public Question
{
private:
    string option1;
    string option2;
    string option3;
    string option4;
    string correctAnswer;

public:
    MCQ()
    {
        option1 = "";
        option2 = "";
        option3 = "";
        option4 = "";
        correctAnswer = "";
    }
    MCQ(string courseOfQuestion, string questionText, string subtopic, string option1, string option2, string option3, string option4, string correctAnswer) : Question(courseOfQuestion, questionText, subtopic, false)
    {
        this->option1 = option1;
        this->option2 = option2;
        this->option3 = option3;
        this->option4 = option4;
        this->correctAnswer = correctAnswer;
    }

    void setAllOptions(string option1, string option2, string option3, string option4, string correctAnswer)
    {
        this->option1 = option1;
        this->option2 = option2;
        this->option3 = option3;
        this->option4 = option4;
        this->correctAnswer = correctAnswer;
    }

    void print() const override
    {
        cout << getQuestionText() << endl;
        cout << "1) " << option1 << endl;
        cout << "2) " << option2 << endl;
        cout << "3) " << option3 << endl;
        cout << "4) " << option4 << endl;
    }

    string getOption1() const
    {
        return option1;
    }
    string getOption2() const
    {
        return option2;
    }
    string getOption3() const
    {
        return option3;
    }
    string getOption4() const
    {
        return option4;
    }
    string getCorrectAnswer() const
    {
        return correctAnswer;
    }

    void setOption1(string option1)
    {
        this->option1 = option1;
    }
    void setOption2(string option2)
    {
        this->option2 = option2;
    }
    void setOption3(string option3)
    {
        this->option3 = option3;
    }
    void setOption4(string option4)
    {
        this->option4 = option4;
    }
    void setCorrectAnswer(string correctAnswer)
    {
        this->correctAnswer = correctAnswer;
    }

    bool checkAnswer(string studentAnswer)
    {
        if (studentAnswer == correctAnswer)
            this->studentIsCorrect = 1;
        else
            this->studentIsCorrect = 0;

        return this->studentIsCorrect;
    }
    bool askQuestion() override
    {
        this->print();

        int choice;
        string studentAnswer;

        cout << "\nEnter your choice for the answer you pick: ";
        cin >> choice;      
        switch (choice)
        {
        case 1:
            studentAnswer = getOption1();
            break;      
        case 2:
            studentAnswer = getOption2();
            break;      
        case 3:
            studentAnswer = getOption3();
            break;      
        case 4:
            studentAnswer = getOption4();
            break;      
        default:
            cout << "Invalid choice! Your answer is being marked incorrect." << endl;
            studentAnswer = "INCORRECTANSWER";
        }       
        
        this->studentIsCorrect = checkAnswer(studentAnswer);
        return this->studentIsCorrect;
    }
    void writeQuestion(string fileName) override
    {
        // Writing to file
        ofstream writeFile;
        writeFile.open(fileName, ios::app);

        writeFile << getCourseOfQuestion() << endl;
        writeFile << getQuestionText() << endl;
        writeFile << getOption1() << endl;
        writeFile << getOption2() << endl;
        writeFile << getOption3() << endl;
        writeFile << getOption4() << endl;
        writeFile << getCorrectAnswer() << endl;
    }

    bool operator==(const MCQ &question) const
    {
        return ((this->courseOfQuestion == question.courseOfQuestion) && (this->questionText == question.questionText) && (this->subtopic == question.subtopic) && (this->studentIsCorrect == question.studentIsCorrect) && (this->option1 == question.option1) && (this->option2 == question.option2) && (this->option3 == question.option3) && (this->option4 == question.option4) && (this->correctAnswer == question.correctAnswer));
    }
    MCQ &operator=(const MCQ &question)
    {
        this->courseOfQuestion = question.courseOfQuestion;
        this->questionText = question.questionText;
        this->subtopic = question.subtopic;
        this->studentIsCorrect = question.studentIsCorrect;
        this->option1 = question.option1;
        this->option2 = question.option2;
        this->option3 = question.option3;
        this->option4 = question.option4;
        this->correctAnswer = question.correctAnswer;
        
        return *this;
    }
};

class TrueFalse : public Question
{
private:
    bool correctAnswer;

public:
    TrueFalse() : Question() {}
    TrueFalse(string courseOfQuestion, string questionText, string subtopic, bool isCorrect, bool correctAnswer) : Question(courseOfQuestion, questionText, subtopic, isCorrect)
    {
        this->correctAnswer = correctAnswer;
    }

    bool getCorrectAnswer() const
    {
        return correctAnswer;
    }
    void setCorrectAnswer(bool correctAnswer)
    {
        this->correctAnswer = correctAnswer;
    }

    void print() const override
    {
        cout << "True or False?" << endl;
        cout << getQuestionText() << endl;
    }
    bool checkAnswer(string studentAnswer)
    {
        if (studentAnswer == "True" && correctAnswer == 1)
            this->studentIsCorrect = 1;
        else if (studentAnswer == "False" && correctAnswer == 0)
            this->studentIsCorrect = 1;
        else
            this->studentIsCorrect = 0;

        return this->studentIsCorrect;
    }
    bool askQuestion() override
    {
        this->print();
        cout << "\nEnter your choice for the answer (1 for True, 0 for False): ";
        
        int choice;
        string studentAnswer;
        cin >> choice;
        
        if (choice == 1)
            studentAnswer = "True";
        else if (choice == 0)
            studentAnswer = "False";
        else
        {
            cout << "Invalid choice! Your answer is being marked incorrect." << endl;
            studentAnswer = "INCORRECTANSWER";
        }
        
        this->studentIsCorrect = checkAnswer(studentAnswer);
        return this->studentIsCorrect;
    }
    void writeQuestion(string fileName) override
    {
        // Writing to file
        ofstream writeFile;
        writeFile.open(fileName, ios::app);

        writeFile << getCourseOfQuestion() << endl;
        writeFile << getQuestionText() << endl;
        if (getCorrectAnswer())
            writeFile << "True" << endl;
        else
            writeFile << "False" << endl;
    }

    bool operator==(const TrueFalse &question) const
    {
        return ((this->courseOfQuestion == question.courseOfQuestion) && (this->questionText == question.questionText) && (this->subtopic == question.subtopic) && (this->studentIsCorrect == question.studentIsCorrect) && (this->correctAnswer == question.correctAnswer));
    }
    TrueFalse &operator=(const TrueFalse &question)
    {
        this->courseOfQuestion = question.courseOfQuestion;
        this->questionText = question.questionText;
        this->subtopic = question.subtopic;
        this->studentIsCorrect = question.studentIsCorrect;
        this->correctAnswer = question.correctAnswer;
        
        return *this;
    }

};

class Subjective : public Question
{
public:
    Subjective() : Question() {}
    Subjective(string courseOfQuestion, string questionText, string subtopic, bool isCorrect) : Question(courseOfQuestion, questionText, subtopic, isCorrect) {}

    void print() const override
    {
        cout << getQuestionText() << endl;
    }
    bool askQuestion() override
    {
        this->print();

        cout << "Enter answer: ";
        string studentAnswer;
        cin.ignore();
        getline(cin, studentAnswer);

        this->studentIsCorrect = checkAnswer(studentAnswer);
        return this->studentIsCorrect;
    }

    bool checkAnswer(string studentAnswer)
    {
        // Checking if the answer has 100 words (as 0.1 marks are awarded for each word)
        int wordCount = 0;
        for (int i = 0; i < studentAnswer.length(); i++)
        {
            if ((studentAnswer[i] == ' ' && studentAnswer[i - 1] != ' ') || studentAnswer[i] == '\n' || (i == 0 && studentAnswer[i] != ' '))
                wordCount++;
        }

        (wordCount >= 100) ? this->studentIsCorrect = 1 : this->studentIsCorrect = 0;
        return this->studentIsCorrect;
    }
    void writeQuestion(string fileName) override
    {
        // Writing to file
        ofstream writeFile;
        writeFile.open(fileName, ios::app);

        writeFile << getCourseOfQuestion() << endl;
        writeFile << getQuestionText() << endl;
    }

    bool operator==(const Subjective &question) const
    {
        return ((this->courseOfQuestion == question.courseOfQuestion) && (this->questionText == question.questionText) && (this->subtopic == question.subtopic) && (this->studentIsCorrect == question.studentIsCorrect));
    }
    Subjective &operator=(const Subjective &question)
    {
        this->courseOfQuestion = question.courseOfQuestion;
        this->questionText = question.questionText;
        this->subtopic = question.subtopic;
        this->studentIsCorrect = question.studentIsCorrect;        
        return *this;
    }

};

class Quiz
{
private:
    static int quizCount;
    int quizID;
    int totalMarks;
    int duration;
    int marksObtained;
    int totalQuestions;
    int marksPerQuestion;
    vector<MCQ> mcqQuestions;
    vector<TrueFalse> trueFalseQuestions;
    vector<Subjective> subjectiveQuestions;
    int totalMCQQuestions;
    int totalTFQuestions;
    int totalSubjectiveQuestions;

public:
    
    Quiz(int totalMarks = 100) : totalMarks(totalMarks)
    {
        this->quizID = quizCount++;
        this->marksObtained = 0;
        this->totalQuestions = 0;
        this->marksPerQuestion = 0;
        this->totalMCQQuestions = 0;
        this->totalTFQuestions = 0;
        this->totalSubjectiveQuestions = 0;
    }

    void printDetails() const
    {
        cout << "Quiz ID: " << quizID << endl;
        cout << "Total Questions: " << totalQuestions << endl;
        cout << "Total Marks: " << totalMarks << endl;
        cout << "Time: " << duration << endl;
    }
    void setQuestionCounts(int totalMCQQuestions, int totalTFQuestions, int totalSubjectiveQuestions)
    {
        this->totalMCQQuestions = totalMCQQuestions;
        this->totalTFQuestions = totalTFQuestions;
        this->totalSubjectiveQuestions = totalSubjectiveQuestions;
        this->totalQuestions = totalMCQQuestions + totalTFQuestions + totalSubjectiveQuestions;
    }
    void setTotalMarks(int totalMarks)
    {
        this->totalMarks = totalMarks;
    }
    void setDuration(int duration)
    {
        this->duration = duration;
    }
    void setMarksPerQuestion(int marksPerQuestion)
    {
        this->marksPerQuestion = marksPerQuestion;
    }

    int getQuizID() const
    {
        return quizID;
    }
    int getMarksObtained() const
    {
        return marksObtained;
    }
    int getTotalMarks() const
    {
        return totalMarks;
    }
    int getDuration() const
    {
        return duration;
    }
    int getMarksPerQuestion() const
    {
        return marksPerQuestion;
    }    
    int getTotalQuestions() const
    {
        return totalQuestions;
    } 
    int getTotalMCQQuestions() const
    {
        return totalMCQQuestions;
    }
    int getTotalTFQuestions() const
    {
        return totalTFQuestions;
    }
    int getTotalSubjectiveQuestions() const
    {
        return totalSubjectiveQuestions;
    }

    MCQ getMCQ(int index) const
    {
        return mcqQuestions[index];
    }
    TrueFalse getTF(int index) const
    {
        return trueFalseQuestions[index];
    }
    Subjective getSubjective(int index) const
    {
        return subjectiveQuestions[index];
    }

    void setMCQ(int index, MCQ question)
    {
        mcqQuestions[index] = question;
    }
    void setTF(int index, TrueFalse question)
    {
        trueFalseQuestions[index] = question;
    }
    void setSubjective(int index, Subjective question)
    {
        subjectiveQuestions[index] = question;
    }

    void setMarksObtained(int marksObtained)
    {
        this->marksObtained = marksObtained;
    }
    void updateTotalQuestions()
    {
        this->totalQuestions = totalMCQQuestions + totalTFQuestions + totalSubjectiveQuestions;
    }
    void updateTotalMCQQuestions()
    {
        this->totalMCQQuestions = mcqQuestions.size();
    }
    void updateTotalTFQuestions()
    {
        this->totalTFQuestions = trueFalseQuestions.size();
    }
    void updateTotalSubjectiveQuestions()
    {
        this->totalSubjectiveQuestions = subjectiveQuestions.size();
    }
    void updateMarksPerQuestion()
    {
        this->marksPerQuestion = totalMarks / totalQuestions;
    }
    void setTotalQuestions(int totalQuestions)
    {
        this->totalQuestions = totalQuestions;
    }
    void setTotalMCQQuestions(int totalMCQQuestions)
    {
        this->totalMCQQuestions = totalMCQQuestions;
    }
    void setTotalTFQuestions(int totalTFQuestions)
    {
        this->totalTFQuestions = totalTFQuestions;
    }
    void setTotalSubjectiveQuestions(int totalSubjectiveQuestions)
    {
        this->totalSubjectiveQuestions = totalSubjectiveQuestions;
    }

    void addMCQ(MCQ question)
    {
        mcqQuestions.push_back(question);
    }
    void addTF(TrueFalse question)
    {
        trueFalseQuestions.push_back(question);
    }
    void addSubjective(Subjective question)
    {
        subjectiveQuestions.push_back(question);
    }

    int attemptQuiz()
    {
        clearScreen();

        // Calculating the time limit for the quiz in seconds
        int timeLimit = duration * 60;

        // Starting the timer
        time_t startTime = time(NULL);

        // Asking questions (MCQ, True False, Subjective but randomizing which is asked)
        int questionCount = 0;
        int mcqsAsked = 0;
        int tfsAsked = 0;
        int subjectiveAsked = 0;
        bool printedQuestionNumber = 0;
        
        while (time(NULL) - startTime < timeLimit && questionCount < totalQuestions)
        {
            // Checking if to print question number
            if (printedQuestionNumber == 0)
            {
                // Preventing an extra blank line from being printed at the start of attempting
                if (questionCount != 0)
                    cout << endl;
                
                cout << 'Q' << questionCount + 1 << ". ";
            }
            printedQuestionNumber = 1;
            
            int randomType = rand() % 3;
            if (randomType == 0 && totalMCQQuestions > 0 && mcqsAsked < totalMCQQuestions)
            {
                printedQuestionNumber = 0;
                
                if (mcqQuestions[mcqsAsked].askQuestion())
                    this->marksObtained += this->marksPerQuestion;

                mcqsAsked++;
            }
            else if (randomType == 1 && totalTFQuestions > 0 && tfsAsked < totalTFQuestions)
            {
                printedQuestionNumber = 0;
                
                if (trueFalseQuestions[tfsAsked].askQuestion())
                    this->marksObtained += this->marksPerQuestion;

                tfsAsked++;
            }
            else if (randomType == 2 && totalSubjectiveQuestions > 0 && subjectiveAsked < totalSubjectiveQuestions)
            {
                printedQuestionNumber = 0;
                
                if (subjectiveQuestions[subjectiveAsked].askQuestion())
                    this->marksObtained += this->marksPerQuestion;

                subjectiveAsked++;
            }
            else
                continue;

            questionCount++;
        }

        return marksObtained;
    }

    Quiz& operator=(const Quiz &quiz)
    {
        this->quizID = quiz.quizID;
        for (int i = 0; i < quiz.mcqQuestions.size(); i++)
            this->mcqQuestions.push_back(quiz.mcqQuestions[i]);
        for (int i = 0; i < quiz.trueFalseQuestions.size(); i++)
            this->trueFalseQuestions.push_back(quiz.trueFalseQuestions[i]);
        for (int i = 0; i < quiz.subjectiveQuestions.size(); i++)
            this->subjectiveQuestions.push_back(quiz.subjectiveQuestions[i]);
        this->totalQuestions = quiz.totalQuestions;
        this->totalMCQQuestions = quiz.totalMCQQuestions;
        this->totalTFQuestions = quiz.totalTFQuestions;
        this->totalSubjectiveQuestions = quiz.totalSubjectiveQuestions;
        this->totalMarks = quiz.totalMarks;
        this->duration = quiz.duration;
        this->marksObtained = quiz.marksObtained;
        this->mcqQuestions = quiz.mcqQuestions;
        this->trueFalseQuestions = quiz.trueFalseQuestions;
        this->subjectiveQuestions = quiz.subjectiveQuestions;
        return *this;
    }
};
int Quiz::quizCount = 0;

string findCourseTeacher(string courseName);

vector<string> findStudentsInCourse(string courseToCheck);

class Course
{
private:
    string courseName;
    string courseCode;
    string courseTeacher;
    int totalStudents;
    vector<string> studentsRegistered;
    vector <MCQ> mcqQuestions;
    vector <TrueFalse> tfQuestions;
    vector <Subjective> subjectiveQuestions;
    int mcqQuestionsCount;
    int tfQuestionsCount;
    int subjectiveQuestionsCount;
    vector<Quiz> quizzes;
public:
    
    Course(string courseName = "", string courseCode = "", string courseTeacher = "", int totalStudents = 0)
    {
        this->courseName = courseName;
        this->courseCode = findCourseCode(courseName);
        this->courseTeacher = findCourseTeacher(courseName);
        studentsRegistered = findStudentsInCourse(courseName);
        this->totalStudents = studentsRegistered.size();
        this->mcqQuestionsCount = 0;
        this->tfQuestionsCount = 0;
        this->subjectiveQuestionsCount = 0;
        quizzes = {};
    }
    Course(string courseName, string courseCode, string courseTeacher, int totalStudents, vector<MCQ> mcqQuestions, vector<TrueFalse> tfQuestions, vector<Subjective> subjectiveQuestions)
    {
        this->courseName = courseName;
        this->courseCode = courseCode;
        this->courseTeacher = courseTeacher;
        this->totalStudents = totalStudents;
        this->mcqQuestions = mcqQuestions;
        this->tfQuestions = tfQuestions;
        this->subjectiveQuestions = subjectiveQuestions;
        this->mcqQuestionsCount = mcqQuestions.size();
        this->tfQuestionsCount = tfQuestions.size();
        this->subjectiveQuestionsCount = subjectiveQuestions.size();
        quizzes = {};
    }

    string getCourseName() const
    {
        return courseName;
    }
    string getCourseCode() const
    {
        return courseCode;
    }
    string getCourseTeacher() const
    {
        return courseTeacher;
    }
    int getTotalStudents() const
    {
        return totalStudents;
    }
    int getTotalQuestions() const
    {
        return mcqQuestionsCount + tfQuestionsCount + subjectiveQuestionsCount;
    }
    int getTotalSubtopics() const
    {
        // Checking the number of subtopics in the question bank
        int totalSubtopics = 0;

        // If unique subtopic is found, totalSubtopics is incremented
        for (int i = 0; i < mcqQuestionsCount; i++)
        {
            bool isUnique = true;
            for (int j = 0; j < i; j++)
            {
                if (mcqQuestions[i].getSubtopic() == mcqQuestions[j].getSubtopic())
                {
                    isUnique = false;
                    break;
                }
            }
            if (isUnique)
                totalSubtopics++;
        }

        for (int i = 0; i < tfQuestionsCount; i++)
        {
            bool isUnique = true;
            for (int j = 0; j < i; j++)
            {
                if (tfQuestions[i].getSubtopic() == tfQuestions[j].getSubtopic())
                {
                    isUnique = false;
                    break;
                }
            }
            if (isUnique)
                totalSubtopics++;
        }

        for (int i = 0; i < subjectiveQuestionsCount; i++)
        {
            bool isUnique = true;
            for (int j = 0; j < i; j++)
            {
                if (subjectiveQuestions[i].getSubtopic() == subjectiveQuestions[j].getSubtopic())
                {
                    isUnique = false;
                    break;
                }
            }
            if (isUnique)
                totalSubtopics++;
        }

        return totalSubtopics;
    }
    int getTotalQuizzes() const
    {
        return quizzes.size();
    }
    int getMCQQuestionsCount() const
    {
        return mcqQuestionsCount;
    }
    int getTFQuestionsCount() const
    {
        return tfQuestionsCount;
    }
    int getSubjectiveQuestionsCount() const
    {
        return subjectiveQuestionsCount;
    }
    
    vector<string> getStudentsRegistered() const
    {
        return studentsRegistered;
    }
    vector<MCQ> getMCQQuestions() const
    {
        return mcqQuestions;
    }
    vector<TrueFalse> getTFQuestions() const
    {
        return tfQuestions;
    }
    vector<Subjective> getSubjectiveQuestions() const
    {
        return subjectiveQuestions;
    }
    
    vector<Quiz> getQuizzes()
    {
        return quizzes;
    }
    
    vector<MCQ> getMCQQuestionsOfSubtopic(string subtopic) const
    {
        // Creating a temporary vector of MCQ questions of the subtopic
        vector<MCQ> temp;
        for (int i = 0; i < mcqQuestions.size(); i++)
        {
            if (mcqQuestions[i].getSubtopic() == subtopic)
            {
                temp.push_back(mcqQuestions[i]);
            }
        }
        return temp;
    }
    vector<TrueFalse> getTFQuestionsOfSubtopic(string subtopic) const
    {
        // Creating a temporary vector of TrueFalse questions of the subtopic
        vector<TrueFalse> temp;
        for (int i = 0; i < tfQuestions.size(); i++)
        {
            if (tfQuestions[i].getSubtopic() == subtopic)
            {
                temp.push_back(tfQuestions[i]);
            }
        }
        return temp;
    }
    vector<Subjective> getSubjectiveQuestionsOfSubtopic(string subtopic) const
    {
        // Creating a temporary vector of Subjective questions of the subtopic
        vector<Subjective> temp;
        for (int i = 0; i < subjectiveQuestions.size(); i++)
        {
            if (subjectiveQuestions[i].getSubtopic() == subtopic)
            {
                temp.push_back(subjectiveQuestions[i]);
            }
        }
        return temp;
    }
    MCQ getMCQ(int index) const
    {
        return mcqQuestions[index];
    }
    TrueFalse getTF(int index) const
    {
        return tfQuestions[index];
    }
    Subjective getSubjective(int index) const
    {
        return subjectiveQuestions[index];
    }

    void setCourseName(string courseName)
    {
        this->courseName = courseName;
    }
    void setCourseCode(string courseCode)
    {
        this->courseCode = courseCode;
    }
    void setCourseTeacher(string courseTeacher)
    {
        this->courseTeacher = courseTeacher;
    }
    void setTotalStudents(int totalStudents)
    {
        this->totalStudents = totalStudents;
    }

    void setMCQQuestionsCount(int mcqQuestionsCount)
    {
        this->mcqQuestionsCount = mcqQuestionsCount;
    }
    void setMCQQuestions(vector<MCQ> mcqQuestions)
    {
        this->mcqQuestions = mcqQuestions;
        this->mcqQuestionsCount = mcqQuestions.size();
    }
    void setMCQQuestions(vector<MCQ> mcqQuestions, int mcqQuestionsCount)
    {
        this->mcqQuestions = mcqQuestions;
        this->mcqQuestionsCount = mcqQuestionsCount;
    }
    void setMCQQuestions(MCQ question, int index)
    {
        mcqQuestions[index] = question;
    }
    
    void setTotalQuestionsTF(int tfQuestionsCount)
    {
        this->tfQuestionsCount = tfQuestionsCount;
    }
    void setTFQuestions(vector<TrueFalse> tfQuestions)
    {
        this->tfQuestions = tfQuestions;
        this->tfQuestionsCount = tfQuestions.size();
    }
    void setTFQuestions(vector<TrueFalse> tfQuestions, int tfQuestionsCount)
    {
        this->tfQuestions = tfQuestions;
        this->tfQuestionsCount = tfQuestionsCount;
    }
    void setTFQuestions(TrueFalse question, int index)
    {
        tfQuestions[index] = question;
    }
    
    void setsubjectiveQuestionsCount(int subjectiveQuestionsCount)
    {
        this->subjectiveQuestionsCount = subjectiveQuestionsCount;
    }
    void setSubjectiveQuestions(vector<Subjective> subjectiveQuestions)
    {
        this->subjectiveQuestions = subjectiveQuestions;
        this->subjectiveQuestionsCount = subjectiveQuestions.size();
    }
    void setSubjectiveQuestions(vector<Subjective> subjectiveQuestions, int subjectiveQuestionsCount)
    {
        this->subjectiveQuestions = subjectiveQuestions;
        this->subjectiveQuestionsCount = subjectiveQuestionsCount;
    }
    void setSubjectiveQuestions(Subjective question, int index)
    {
        subjectiveQuestions[index] = question;
    }
    
    void setTotalStudents()
    {
        totalStudents = studentsRegistered.size();
    }
    void setStudentsRegistered(vector<string> studentsRegistered)
    {
        this->studentsRegistered = studentsRegistered;
        this->totalStudents = studentsRegistered.size();
    }

    void addQuiz(Quiz &quiz)
    {
        quizzes.push_back(quiz);
    }

    void setSubtopic(string subtopic, int index, string typeOfQuestion)
    {
        if (typeOfQuestion == "MCQ")
            mcqQuestions[index].setSubtopic(subtopic);
        else if (typeOfQuestion == "TrueFalse")
            tfQuestions[index].setSubtopic(subtopic);
        else if (typeOfQuestion == "Subjective")
            subjectiveQuestions[index].setSubtopic(subtopic);
    }
    void addMCQ(MCQ& questionToAdd)
    {
        mcqQuestionsCount++;
        mcqQuestions.push_back(questionToAdd);
    }
    void addTrueFalse(TrueFalse questionToAdd)
    {
        tfQuestionsCount++;
        tfQuestions.push_back(questionToAdd);
    }
    void addSubjective(Subjective questionToAdd)
    {
        subjectiveQuestionsCount++;
        subjectiveQuestions.push_back(questionToAdd);
    }

    void printDetails() const
    {
        cout << "Course Name: " << courseName << endl;
        cout << "Course Code: " << courseCode << endl;
        cout << "Course Teacher: " << courseTeacher << endl;
        cout << "Total Students: " << totalStudents << endl;
        cout << "Total Questions: " << getTotalQuestions() << endl;
    }
    
    bool operator==(const Course &course) const
    {
        return (courseName == course.courseName && courseCode == course.courseCode && courseTeacher == course.courseTeacher);
    }
};

void allocateQuizQuestions(Course &courseOfQuiz, Quiz &quizToAllocate);
void questionReader(Course &courseOfQuestions);

class Teacher : public User
{
private:
    string username;
    string courseTaughtName;
    string courseTaughtCode;

public:
    Teacher(string username, string password) : User(findTeacherName(username), password)
    {
        this->username = username;
        this->courseTaughtName = findTeacherCourse(username);
        this->courseTaughtCode = findCourseCode(courseTaughtName);
        this->username = username;
    }
    string getCourseTaughtName() const
    {
        return courseTaughtName;
    }
    string getCourseTaughtCode() const
    {
        return courseTaughtCode;
    }
    string getTeacherName() const
    {
        return name;
    }
    string getUsername() const
    {
        return username;
    }
    string getPassword() const
    {
        return password;
    }

    void setCourseTaughtName(string courseTaughtName)
    {
        this->courseTaughtName = courseTaughtName;
    }
    void setCourseTaughtCode(string courseTaughtCode)
    {
        this->courseTaughtCode = courseTaughtCode;
    }
    void setTeacherName(string name)
    {
        this->name = name;
    }
    void setUsername(string username)
    {
        this->username = username;
    }
    void setPassword(string password)
    {
        if (validatePassword(password))
            this->password = password;
        else
            inputPassword();
    }
    void overwritePassword(string password)
    {
        ifstream readFile;
        readFile.open("data/Teacher_Details.csv", ios::in);

        if (!readFile)
        {
            cout << "Unable to open file" << endl;
            return;
        }

        // Creating a vector for storing contents
        vector<string> fileContents;
        string line;

        while (getline(readFile, line))
            fileContents.push_back(line);

        // Closing the file after reading
        readFile.close();

        // Finding the username and username
        int sizeOfVector = fileContents.size();
        for (int i = 0; i < sizeOfVector; i++)
        {
            // Finding the index of the username and name using vector find function
            int foundAtIndex = fileContents[i].find(this->username + "," + this->name);

            // If found, replace the password
            if (foundAtIndex >= 0)
                fileContents[i] = this->username + "," + this->name + "," + this->courseTaughtName + "," + password;
        }

        // Writing the file with the new password
        ofstream writeFile;
        writeFile.open("data/Teacher_Details.csv", ios::out);

        for (int i = 0; i < sizeOfVector; i++)
            writeFile << fileContents[i] << endl;

        writeFile.close();
    }
    void addQuestionToBank()
    {
        string correctOptionMarker = "option dabfac4: ";
        string incorrectOptionMarker = "option: ";

        string typeOfQuestion;
        cout << "Enter the type of question (MCQ/TrueFalse/Subjective): ";
        cin >> typeOfQuestion;
        string subtopic;
        cout << "Enter the subtopic of the question: ";
        cin >> subtopic;
        string questionStatement;
        cout << "Enter the question statement: ";
        cin.ignore();
        getline(cin, questionStatement);

        string courseName = this->getCourseTaughtName();
        // Converting CourseName to have "_" instead of " "
        for (int i = 0; i < courseName.length(); i++)
        {
            if (courseName[i] == ' ')
                courseName[i] = '_';
        }
        // Converting CourseName to Alternating Caps
        for (int i = 0; i < courseName.length(); i++)
        {
            if ((courseName[i - 1] == '_' || i == 0) && (courseName[i] >= 'a' && courseName[i] <= 'z'))
            courseName[i] += 'A' - 'a';
        }

        ifstream readFile;
        string fileName = "data/Questions_for_" + courseName + ".txt";
        readFile.open(fileName, ios::in);

        if (!readFile)
        {
            cout << "Unable to open file" << endl;
            return;
        }

        // Creating a vector for storing contents
        vector<string> fileContents;
        string line;

        while (getline(readFile, line))
            fileContents.push_back(line);

        readFile.close();

        // Finding if the question already exists
        int questionFoundAtIndex = -1;
        int sizeOfVector = fileContents.size();

        for (int i = 0; i < sizeOfVector; i++)
        {
            questionFoundAtIndex = fileContents[i].find(questionStatement);
            if (questionFoundAtIndex >= 0)
            {
                cout << "Question already exists" << endl;
                return;
            }
        }

        // Finding the index of the subtopic
        int subtopicFoundAtIndex = -1;
        for (int i = 0; i < sizeOfVector; i++)
        {
            subtopicFoundAtIndex = fileContents[i].find(subtopic);
            if (subtopicFoundAtIndex >= 0)
                break;
        }
        // Checking if an empty line is present after the subtopic
        if (subtopicFoundAtIndex >= 0 && fileContents[subtopicFoundAtIndex + 1] == "")
            subtopicFoundAtIndex++;
        else
            fileContents.push_back("");

        // Else if subtopic not found, add a new subtopic
        if (subtopicFoundAtIndex < 0)
        {
            fileContents.push_back("a5380ee");
            fileContents.push_back(subtopic);
            fileContents.push_back("");
        }

        if (typeOfQuestion == "MCQ")
        {
            fileContents.push_back("2efcde9");
            fileContents.push_back(questionStatement);

            string option1, option2, option3, option4;
            cout << "Enter option 1: ";
            getline(cin, option1);
            cout << "Enter option 2: ";
            getline(cin, option2);
            cout << "Enter option 3: ";
            getline(cin, option3);
            cout << "Enter option 4: ";
            getline(cin, option4);

            string correctOption;
            cout << "Enter the correct option: ";
            cin >> correctOption;

            // Checking which option is the correct option and adding the correct option marker
            if (correctOption == option1)
            {
                option1 = correctOptionMarker + option1;
                option2 = incorrectOptionMarker + option2;
                option3 = incorrectOptionMarker + option3;
                option4 = incorrectOptionMarker + option4;
            }
            else if (correctOption == option2)
            {
                option2 = correctOptionMarker + option2;
                option1 = incorrectOptionMarker + option1;
                option3 = incorrectOptionMarker + option3;
                option4 = incorrectOptionMarker + option4;
            }
            else if (correctOption == option3)
            {
                option3 = correctOptionMarker + option3;
                option1 = incorrectOptionMarker + option1;
                option2 = incorrectOptionMarker + option2;
                option4 = incorrectOptionMarker + option4;
            }
            else if (correctOption == option4)
            {
                option4 = correctOptionMarker + option4;
                option1 = incorrectOptionMarker + option1;
                option2 = incorrectOptionMarker + option2;
                option3 = incorrectOptionMarker + option3;
            }
            else
            {
                cout << "Correct answer not found in options" << endl;
                return;
            }
            fileContents.push_back(option1);
            fileContents.push_back(option2);
            fileContents.push_back(option3);
            fileContents.push_back(option4);
        }
        else if (typeOfQuestion == "TrueFalse")
        {
            string correctOption;
            cout << "Enter the correct option: ";
            cin >> correctOption;

            fileContents.push_back("b94d27b");
            fileContents.push_back(questionStatement);

            // Checking which option is the correct option and adding the correct option marker
            if (correctOption == "True")
            {
                fileContents.push_back(correctOptionMarker + "True");
                fileContents.push_back(incorrectOptionMarker + "False");
            }
            else if (correctOption == "False")
            {
                fileContents.push_back(correctOptionMarker + "False");
                fileContents.push_back(incorrectOptionMarker + "True");
            }
            else
            {
                cout << "Correct answer not found in options" << endl;
                return;
            }
        }
        else if (typeOfQuestion == "Subjective")
        {
            fileContents.push_back("88f7ace");
            fileContents.push_back(questionStatement);
        }

        // Writing the contents to the file
        ofstream writeFile;
        writeFile.open(fileName, ios::out);
        sizeOfVector = fileContents.size();

        for (int i = 0; i < sizeOfVector; i++)
        {
            writeFile << fileContents[i] << endl;
        }
        writeFile.close();
    }
    void createQuizManually(Quiz& quizToCreate)
    {
        Time startTime;
        cout << "\nQuiz start time (in 24 hour clock):" << endl;
        cin >> startTime;

        Time endTime;
        cout << "\nQuiz end time (in 24 hour clock):" << endl;
        cin >> endTime;

        while (startTime > endTime)
        {
            cout << "End time cannot be before start time" << endl;
            cin >> endTime;
        }

        // Getting duration
        int duration = endTime - startTime;
        quizToCreate.setDuration(duration);

        int quizMarks;
        cout << "\nQuiz marks: ";
        cin >> quizMarks;
        quizToCreate.setTotalMarks(quizMarks);
        
        int numberOfQuestions;
        cout << "\nNumber of questions: ";
        cin >> numberOfQuestions;
        quizToCreate.setTotalQuestions(numberOfQuestions);
        
        cout << "\nAllocating questions to quiz..." << endl;
        Course temp;
        temp.setCourseCode(this->getCourseTaughtCode());
        temp.setCourseName(this->getCourseTaughtName());
        temp.setCourseTeacher(findCourseTeacher(this->getCourseTaughtName()));
        temp.setStudentsRegistered(findStudentsInCourse(this->getCourseTaughtName()));
        
        questionReader(temp);
        allocateQuizQuestions(temp, quizToCreate);
    }
};

Course *coursesRegistered(string rollNumber, int &registeredCount);

class Student : public User
{
private:
    Course *coursesEnrolled;
    int totalCoursesEnrolled;
    string rollNumber;

public:
    Student(string rollNumber, string password) : User(findStudentName(rollNumber), password)
    {
        this->rollNumber = rollNumber;

        if (validatePassword(password))
            this->password = password;
        else
            inputPassword();
        this->totalCoursesEnrolled = 0;
        Course *temp = coursesRegistered(this->rollNumber, this->totalCoursesEnrolled);
        coursesEnrolled = new Course[this->totalCoursesEnrolled];
        for (int i = 0; i < this->totalCoursesEnrolled; i++)
        {
            coursesEnrolled[i].setCourseCode(temp[i].getCourseCode());
            coursesEnrolled[i].setCourseName(temp[i].getCourseName());
            coursesEnrolled[i].setCourseTeacher(findCourseTeacher(coursesEnrolled[i].getCourseName()));
            coursesEnrolled[i].setStudentsRegistered(findStudentsInCourse(coursesEnrolled[i].getCourseName()));
            questionReader(coursesEnrolled[i]);
        }
    }

    Course *getCoursesEnrolled() const
    {
        return coursesEnrolled;
    }
    int getTotalCoursesEnrolled() const
    {
        return totalCoursesEnrolled;
    }
    Course getCourse(int index) const
    {
        return coursesEnrolled[index];
    }

    void setTotalCoursesEnrolled(int totalCoursesEnrolled)
    {
        this->totalCoursesEnrolled = totalCoursesEnrolled;
    }
    void setCoursesEnrolled(Course *coursesEnrolled)
    {
        this->coursesEnrolled = coursesEnrolled;
    }
    void setCourse(Course course, int index)
    {
        coursesEnrolled[index] = course;
    }
    string getRollNumber() const
    {
        return rollNumber;
    }
    string getPassword() const
    {
        return password;
    }
    string getStudentName() const
    {
        return name;
    }

    void setRollNumber(string rollNumber)
    {
        this->rollNumber = rollNumber;
    }
    void setPassword(string password)
    {
        if (validatePassword(password))
            this->password = password;
        else
            inputPassword();
    }
    void setStudentName(string name)
    {
        this->name = name;
    }

    void overwritePassword(string password)
    {
        ifstream readFile;
        readFile.open("data/Student_Logins.csv", ios::in);

        if (!readFile)
        {
            cout << "Unable to open file" << endl;
            return;
        }

        // Creating vector for storing contents
        vector<string> fileContents;
        string line;

        while (getline(readFile, line))
            fileContents.push_back(line);

        // Closing the file after reading
        readFile.close();

        // Finding the username and rollNumber
        int sizeOfVector = fileContents.size();
        for (int i = 0; i < sizeOfVector; i++)
        {
            // Finding the index of the username and rollNumber using vector find function
            int foundAtIndex = fileContents[i].find(this->rollNumber);

            // If found, replace the password
            if (foundAtIndex >= 0)
                fileContents[i] = this->rollNumber + "," + this->name + "," + password;
        }

        // Writing the file with the new password
        ofstream writeFile;
        writeFile.open("data/Student_Logins.csv", ios::out);

        for (int i = 0; i < sizeOfVector; i++)
            writeFile << fileContents[i] << endl;

        writeFile.close();
    }
    void printDetails()
    {
        cout << "Name: " << name << endl;
        cout << "Roll Number: " << rollNumber << endl;
        cout << "\nCourses Enrolled: " << endl;
        for (int i = 0; i < totalCoursesEnrolled; i++)
            cout << i << ". " << coursesEnrolled[i].getCourseName() << endl;
    }

    void generateQuizForCourse(int courseIndex)
    {
        questionReader(this->coursesEnrolled[courseIndex]);

        Quiz quiz(100);
        quiz.setDuration(60);
        quiz.setTotalQuestions(10);

        allocateQuizQuestions(this->coursesEnrolled[courseIndex], quiz);

        coursesEnrolled[courseIndex].addQuiz(quiz);
    }
    void addQuizToCourse(int courseIndex, Quiz &quizToAdd)
    {
        coursesEnrolled[courseIndex].addQuiz(quizToAdd);
    }
    
    ~Student()
    {
        delete[] coursesEnrolled;
    }
};

// other function definitions
void allocateQuizQuestions(Course &courseOfQuiz, Quiz &quizToAllocate)
{
    // Getting all subtopics from the questions as they store the subtopic name
    vector<string> subtopics;

    int mcqCount = courseOfQuiz.getMCQQuestionsCount();
    int tfCount = courseOfQuiz.getTFQuestionsCount();
    int subjectiveCount = courseOfQuiz.getSubjectiveQuestionsCount();
    
    // Adding only unique subtopics
    for (int i = 0; i < mcqCount; i++)
    {
        string currentSubtopic = courseOfQuiz.getMCQ(i).getSubtopic();
        if (find(subtopics.begin(), subtopics.end(), currentSubtopic) == subtopics.end())
            subtopics.push_back(currentSubtopic);
    }
    for (int i = 0; i < tfCount; i++)
    {
        string currentSubtopic = courseOfQuiz.getTF(i).getSubtopic();
        if (find(subtopics.begin(), subtopics.end(), currentSubtopic) == subtopics.end())
            subtopics.push_back(currentSubtopic);
    }
    for (int i = 0; i < subjectiveCount; i++)
    {
        string currentSubtopic = courseOfQuiz.getSubjective(i).getSubtopic();
        if (find(subtopics.begin(), subtopics.end(), currentSubtopic) == subtopics.end())
            subtopics.push_back(currentSubtopic);
    }

    // Randomizing the questions picked while ensuring every subtopic is covered
    vector<MCQ> mcqQuestions;
    vector<TrueFalse> tfQuestions;
    vector<Subjective> subjectiveQuestions;

    for (int i = 0; i < subtopics.size(); i++) 
    {
        vector<MCQ> mcqQuestionsForSubtopic;
        vector<TrueFalse> tfQuestionsForSubtopic;
        vector<Subjective> subjectiveQuestionsForSubtopic;

        for (int j = 0; j < mcqCount; j++)
        {
            MCQ mcq = courseOfQuiz.getMCQ(j);
            if (mcq.getSubtopic() == subtopics[i])
                mcqQuestionsForSubtopic.push_back(mcq);
        }
        for (int j = 0; j < tfCount; j++)
        {
            TrueFalse tf = courseOfQuiz.getTF(j);
            if (tf.getSubtopic() == subtopics[i])
                tfQuestionsForSubtopic.push_back(tf);
        }
        for (int j = 0; j < subjectiveCount; j++)
        {
            Subjective subjective = courseOfQuiz.getSubjective(j);
            if (subjective.getSubtopic() == subtopics[i])
                subjectiveQuestionsForSubtopic.push_back(subjective);
        }

        if (mcqQuestionsForSubtopic.size() > 0 && tfQuestionsForSubtopic.size() > 0 && subjectiveQuestionsForSubtopic.size() > 0)
        {
            int randomIndex = rand() % mcqQuestionsForSubtopic.size();
            mcqQuestions.push_back(mcqQuestionsForSubtopic[randomIndex]);

            randomIndex = rand() % tfQuestionsForSubtopic.size();
            tfQuestions.push_back(tfQuestionsForSubtopic[randomIndex]);

            randomIndex = rand() % subjectiveQuestionsForSubtopic.size();
            subjectiveQuestions.push_back(subjectiveQuestionsForSubtopic[randomIndex]);
        }
    }

    // Randomizing the questions picked while ensuring every subtopic is covered
    int availableMCQsCount = mcqQuestions.size();
    int availableTFsCount = tfQuestions.size();
    int AvailableSubjectivesCount = subjectiveQuestions.size();

    // Finding number of questions to store of each with ratio 2:2:1 of MCQ:TF:Subjective, using totalQuestions as a data member
    int mcqsToStore = (2 * quizToAllocate.getTotalQuestions()) / 5;
    int tfsToStore = (2 * quizToAllocate.getTotalQuestions()) / 5;
    int subjectivesToStore = (quizToAllocate.getTotalQuestions()) / 5;

    int mcqQuestionsIndex[mcqsToStore];
    int tfQuestionsIndex[tfsToStore];
    int subjectiveQuestionsIndex[subjectivesToStore];

    for (int i = 0; i < mcqsToStore; i++)
    {
        if (availableMCQsCount != 0)
        {
            int randomIndex = rand() % availableMCQsCount;
            quizToAllocate.addMCQ(mcqQuestions[randomIndex]);
            availableMCQsCount--;
            mcqQuestions[randomIndex] = mcqQuestions[availableMCQsCount];
        }
    }
    for (int i = 0; i < tfsToStore; i++)
    {
        if (availableTFsCount != 0)
        {
            int randomIndex = rand() % availableTFsCount;
            quizToAllocate.addTF(tfQuestions[randomIndex]);
            availableTFsCount--;
            tfQuestions[randomIndex] = tfQuestions[availableTFsCount];
        }
    }
    for (int i = 0; i < subjectivesToStore; i++)
    {
        if (AvailableSubjectivesCount != 0)
        {
            int randomIndex = rand() % AvailableSubjectivesCount;
            quizToAllocate.addSubjective(subjectiveQuestions[randomIndex]);
            AvailableSubjectivesCount--;
            subjectiveQuestions[randomIndex] = subjectiveQuestions[AvailableSubjectivesCount];
        }
    }
    quizToAllocate.updateTotalMCQQuestions();
    quizToAllocate.updateTotalTFQuestions();
    quizToAllocate.updateTotalSubjectiveQuestions();
    quizToAllocate.updateTotalQuestions();
    quizToAllocate.updateMarksPerQuestion();
}
int courseCounter()
{
    // Opening the file
    string fileName = "data/Course_Registration_Data_Sheet.csv";
    ifstream inputFile(fileName);

    // Getting the first line of the file
    string line;
    int courseCount = 0;

    getline(inputFile, line, '\n');

    // Creating a stringstream object to read the line
    stringstream lineStream(line);

    string cell;

    // Repeatedly getting the next cell if it is empty / ',' until it is not
    getline(lineStream, cell, ',');
    while (getline(lineStream, cell, ','))
    {
        while (cell == "")
            getline(lineStream, cell, ',');

        // If the cell is not empty, incrementing the course count
        courseCount++;
    }

    // Closing the file and returning the course count
    inputFile.close();
    return courseCount;
}

Course *coursesRegistered(string rollNumber, int &registeredCount)
{
    // Storing file contents in a vector temporarily
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

    // Finding the student and storing their courses in a string array
    Course *courses = new Course[courseCounter()];
    for (int i = 0; i < fileContents.size(); i++)
    {
        if (fileContents[i][1] == rollNumber)
        {
            // Finding the courses that the student is registered in (column will have '1')
            for (int j = 3; j < fileContents[i].size(); j++)
            {
                if (fileContents[i][j] == "1")
                {
                    courses[registeredCount].setCourseName(fileContents[0][j]);
                    courses[registeredCount].setCourseCode(fileContents[1][j]);
                    courses[registeredCount].setCourseTeacher(findCourseTeacher(fileContents[0][j]));
                    registeredCount++;
                }
            }
        }
    }
    return courses;
}
Course *courseFinder()
{
    string fileName = "data/Course_Registration_Data_Sheet.csv";
    ifstream inputFile(fileName);
    string line;
    int courseCount = courseCounter();

    // Creating a dynamic array of strings to store the course names
    Course *courses = new Course[courseCount];

    // Resetting the file pointer to the beginning of the file
    inputFile.clear();

    // Getting the first line of the file
    getline(inputFile, line, '\n');
    stringstream lineStream2(line);

    // Repeatedly getting the next cell if it is empty / ',' until it is not
    int courseIndex = 0;
    string cell;
    getline(lineStream2, cell, ',');
    while (getline(lineStream2, cell, ','))
    {
        while (cell == "")
            getline(lineStream2, cell, ',');

        // If reached the end of the course names, break out of the loop
        if (courseIndex == courseCount)
            break;

        // If reached the final course name, remove the '\r' from the end of the string
        if (courseIndex == courseCount - 1)
        {
            int startIndex = 0;
            for (startIndex; cell[startIndex] != '\0'; startIndex++)
            {
                if (cell[startIndex] == '\r')
                    break;
            }
            deleteCharacters(cell, startIndex, 1);
        }

        // If the cell is not empty, storing the course name in the array
        courses[courseIndex].setCourseName(cell);
        courseIndex++;
    }

    // Reading next line of the file for the course codes
    getline(inputFile, line, '\n');
    stringstream lineStream(line);

    // Repeatedly getting the next cell if it is empty / ',' until it is not
    courseIndex = 0;
    getline(lineStream, cell, ',');
    while (getline(lineStream, cell, ','))
    {
        while (cell == "S#" || cell == "Roll No." || cell == "Student Name")
            getline(lineStream, cell, ',');

        // If the course index is equal to the course count, break out of the loop
        if (courseIndex == courseCount)
            break;

        // If reached the final course code, remove the '\r' from the end of the string
        if (courseIndex == courseCount - 1)
        {
            int startIndex = 0;
            for (startIndex; cell[startIndex] != '\0'; startIndex++)
            {
                if (cell[startIndex] == '\r')
                    break;
            }
            deleteCharacters(cell, startIndex, 1);
        }

        // If the cell is not empty, storing the course code in the array
        courses[courseIndex].setCourseCode(cell);
        courseIndex++;
    }

    inputFile.close();
    return courses;
}

void questionReader(Course &courseOfQuestions)
{
    string nameOfCourse = courseOfQuestions.getCourseName();

    // Converting CourseName to have "_" instead of " "
    for (int i = 0; i < nameOfCourse.length(); i++)
    {
        if (nameOfCourse[i] == ' ')
            nameOfCourse[i] = '_';
    }
    // Converting CourseName to Alternating Caps
    for (int i = 0; i < nameOfCourse.length(); i++)
    {
        if ((nameOfCourse[i - 1] == '_' || i == 0) && nameOfCourse[i] >= 'a' && nameOfCourse[i] <= 'z')
            nameOfCourse[i] += 'A' - 'a';
    }

    // Getting the name of the file
    string fileName = "data/Questions_for_" + nameOfCourse + ".txt";

    // Reading the questions from the file
    ifstream inputFile(fileName);
    string line;

    // Checking if the file is open
    if (!inputFile.is_open())
    {
        cout << "Failed to open file!" << endl;
        return;
    }

    inputFile.clear();

    // variables for the questions
    vector<MCQ> mcqs;
    vector<TrueFalse> tfs;
    vector<Subjective> subjectives;

    int mcqCount = 0;
    int tfCount = 0;
    int subjectiveCount = 0;
    
    int questionsInSubtopic = 0;
    string subtopic = "";

    getline(inputFile, line, '\n');

    while (inputFile)
    {
        // Subtopics
        if (line == "a5380ee")
        {
            getline(inputFile, line, '\n');
            subtopic = line;
            questionsInSubtopic = 0;

            // Skipping a line
            inputFile.ignore();
            getline(inputFile, line, '\n');
        }

        // Subjective questions
        if (line == "88f7ace")
        {
            getline(inputFile, line, '\n');
            subjectives.push_back(Subjective());
            subjectives[subjectiveCount].setCourseOfQuestion(courseOfQuestions.getCourseName());
            subjectives[subjectiveCount].setQuestionText(line);
            subjectives[subjectiveCount].setSubtopic(subtopic);

            subjectiveCount++;
            questionsInSubtopic++;

            // Skipping a line
            inputFile.ignore();
            getline(inputFile, line, '\n');
        }

        // Truefalse questions
        if (line == "b94d27b")
        {
            // Question Text
            getline(inputFile, line, '\n');
            tfs.push_back(TrueFalse());
            tfs[tfCount].setCourseOfQuestion(courseOfQuestions.getCourseName());
            tfs[tfCount].setQuestionText(line);
            tfs[tfCount].setSubtopic(subtopic);

            // Getting the first option
            getline(inputFile, line, '\n');

            // Storing the "option " or "option dabfac4 "
            string optionText;
            stringstream lineStream(line);
            getline(lineStream, optionText, ':');

            // Getting all options
            while (optionText == "option" || optionText == "option dabfac4")
            {
                // If incorrect
                if (optionText == "option")
                {
                    // Move to next line
                    getline(inputFile, line, '\n');
                    lineStream.str(line);
                    getline(lineStream, optionText, ':');

                    if (optionText == "")
                        break;
                }

                if (optionText == "option dabfac4")
                {
                    string correctOption = line;

                    // Removing "option from line and storing into incorrectOption"
                    deleteCharacters(correctOption, 0, 16);
                    if (correctOption == "True" || correctOption == "true")
                        tfs[tfCount].setCorrectAnswer(true);
                    else if (correctOption == "False" || correctOption == "false")
                        tfs[tfCount].setCorrectAnswer(false);

                    // Moving to next line
                    getline(inputFile, line, '\n');
                    lineStream.str(line);
                    getline(lineStream, optionText, ':');

                    if (optionText == "")
                        break;
                }
            }
            tfCount++;
            questionsInSubtopic++;
        }

        // MCQ questions
        if (line == "2efcde9")
        {
            // Question Text
            getline(inputFile, line, '\n');
            mcqs.push_back(MCQ());
            mcqs[mcqCount].setCourseOfQuestion(courseOfQuestions.getCourseName());
            mcqs[mcqCount].setQuestionText(line);
            mcqs[mcqCount].setSubtopic(subtopic);

            // Getting the first option
            getline(inputFile, line, '\n');

            // Storing the "option " or "option dabfac4 "
            string optionText;
            stringstream lineStream(line);
            getline(lineStream, optionText, ':');

            // Getting all options
            int optionCount = 0;
            while (optionText == "option" || optionText == "option dabfac4")
            {
                // If incorrect
                if (optionText == "option")
                {
                    string incorrectOption = line;

                    // Removing "option: " from line and storing into incorrectOption"
                    deleteCharacters(incorrectOption, 0, 8);
                    switch (++optionCount)
                    {
                    case 1:
                        mcqs[mcqCount].setOption1(incorrectOption);
                        break;

                    case 2:
                        mcqs[mcqCount].setOption2(incorrectOption);
                        break;

                    case 3:
                        mcqs[mcqCount].setOption3(incorrectOption);
                        break;

                    case 4:
                        mcqs[mcqCount].setOption4(incorrectOption);
                        break;

                    default:
                        cout << "Error in setting options" << endl;
                        break;
                    }

                    // Move to next line
                    getline(inputFile, line, '\n');
                    lineStream.str(line);
                    getline(lineStream, optionText, ':');

                    if (optionText == "")
                        break;
                }

                if (optionText == "option dabfac4")
                {
                    string correctOption = line;

                    // Removing "option dabfac4: " from line and storing into incorrectOption"
                    deleteCharacters(correctOption, 0, 16);

                    // Storing into correct option in MCQ
                    mcqs[mcqCount].setCorrectAnswer(correctOption);

                    switch (++optionCount)
                    {
                    case 1:
                        mcqs[mcqCount].setOption1(correctOption);
                        break;

                    case 2:
                        mcqs[mcqCount].setOption2(correctOption);
                        break;

                    case 3:
                        mcqs[mcqCount].setOption3(correctOption);
                        break;

                    case 4:
                        mcqs[mcqCount].setOption4(correctOption);
                        break;

                    default:
                        cout << "Error in setting options" << endl;
                        break;
                    }

                    // Move to next line
                    getline(inputFile, line, '\n');
                    lineStream.str(line);
                    getline(lineStream, optionText, ':');

                    if (optionText == "")
                        break;
                }
            }
            mcqCount++;
            questionsInSubtopic++;
        }
        if (line == "")
            getline(inputFile, line, '\n');
    }

    // Storing the questions in the course 
    courseOfQuestions.setMCQQuestions(mcqs);
    courseOfQuestions.setTFQuestions(tfs);
    courseOfQuestions.setSubjectiveQuestions(subjectives);
    
    inputFile.close();
}
void writeQuizToFile(Course courseWithQuiz, int quizID)
{
    string nameOfCourse = courseWithQuiz.getCourseName(); 

    // Converting CourseName to have "_" instead of " "
    for (int i = 0; i < nameOfCourse.length(); i++)
    {
        if (nameOfCourse[i] == ' ')
            nameOfCourse[i] = '_';
    }
    // Converting CourseName to Alternating Caps
    for (int i = 0; i < nameOfCourse.length(); i++)
    {
        if ((nameOfCourse[i - 1] == '_' || i == 0) && nameOfCourse[i] >= 'a' && nameOfCourse[i] <= 'z')
            nameOfCourse[i] += 'A' - 'a';
    }
    
    string fileName = nameOfCourse + "_Quiz_" + to_string(quizID) + "_Details.txt";
    
    ofstream writeFile;
    writeFile.open(fileName, ios::out);
    writeFile << "QuizID: " << quizID << endl;
    writeFile << "Course: " << courseWithQuiz.getCourseName() << endl;
    writeFile << "Instructor: " << courseWithQuiz.getCourseTeacher() << endl;
    writeFile << "Duration: " << courseWithQuiz.getQuizzes()[quizID].getDuration() << endl;
    writeFile << "TotalMarks: " << courseWithQuiz.getQuizzes()[quizID].getTotalMarks() << endl;
    writeFile << "TotalQuestions: " << courseWithQuiz.getQuizzes()[quizID].getTotalQuestions() << endl;
    writeFile << "\nQuestions:" << endl;

    int totalMCQQuestions = courseWithQuiz.getQuizzes()[quizID].getTotalMCQQuestions();
    int totalTFQuestions = courseWithQuiz.getQuizzes()[quizID].getTotalTFQuestions();
    int totalSubjectiveQuestions = courseWithQuiz.getQuizzes()[quizID].getTotalSubjectiveQuestions();

    for (int i = 0; i < totalMCQQuestions; i++)
    {
        writeFile << "\nMCQ: ";
        writeFile << courseWithQuiz.getQuizzes()[quizID].getMCQ(i).getQuestionText() << endl;
        writeFile << "Option 1: " << courseWithQuiz.getQuizzes()[quizID].getMCQ(i).getOption1() << endl;
        writeFile << "Option 2: " << courseWithQuiz.getQuizzes()[quizID].getMCQ(i).getOption2() << endl;
        writeFile << "Option 3: " << courseWithQuiz.getQuizzes()[quizID].getMCQ(i).getOption3() << endl;
        writeFile << "Option 4: " << courseWithQuiz.getQuizzes()[quizID].getMCQ(i).getOption4() << endl;
        writeFile << "Correct Answer: " << courseWithQuiz.getQuizzes()[quizID].getMCQ(i).getCorrectAnswer() << endl;
    }
    for (int i = 0; i < totalTFQuestions; i++)
    {
        writeFile << "\nTrueFalse: ";
        writeFile << courseWithQuiz.getQuizzes()[quizID].getTF(i).getQuestionText() << endl;
        writeFile << "Correct Answer: " << ((courseWithQuiz.getQuizzes()[quizID].getTF(i).getCorrectAnswer())? "True": "False") << endl;
    }
    for (int i = 0; i < totalSubjectiveQuestions; i++)
    {
        writeFile << "\nSubjective: ";
        writeFile << courseWithQuiz.getQuizzes()[quizID].getSubjective(i).getQuestionText() << endl;
    }

    writeFile.close();
}
void writeResultsOfQuiz(string rollNumber, Course courseWithQuiz, int quizID, int obtainedMarks)
{
    // Storing the results of the quiz in the file
    string nameOfCourse = courseWithQuiz.getCourseName();

    // Converting CourseName to have "_" instead of " "
    for (int i = 0; i < nameOfCourse.length(); i++)
    {
        if (nameOfCourse[i] == ' ')
            nameOfCourse[i] = '_';
    }
    // Converting CourseName to Alternating Caps
    for (int i = 0; i < nameOfCourse.length(); i++)
    {
        if ((nameOfCourse[i - 1] == '_' || i == 0) && nameOfCourse[i] >= 'a' && nameOfCourse[i] <= 'z')
            nameOfCourse[i] += 'A' - 'a';
    }

    string fileName = nameOfCourse + "_Quiz_" + to_string(quizID) + '_' + "Results.txt";

    ofstream writeFile;
    writeFile.open(fileName, ios::app);
    writeFile << rollNumber << endl;
    writeFile << "QuizID: " << quizID << endl;
    writeFile << "Marks: " << obtainedMarks << '/' << courseWithQuiz.getQuizzes()[quizID].getTotalMarks() << endl;
}
void getResultsOfQuiz(string rollNumber, string courseName, int quizID)
{
    // Converting CourseName to have "_" instead of " "
    for (int i = 0; i < courseName.length(); i++)
    {
        if (courseName[i] == ' ')
            courseName[i] = '_';
    }
    // Converting CourseName to Alternating Caps
    for (int i = 0; i < courseName.length(); i++)
    {
        if ((courseName[i - 1] == '_' || i == 0) && courseName[i] >= 'a' && courseName[i] <= 'z')
            courseName[i] += 'A' - 'a';
    }

    string fileName = courseName + "_Quiz_" + to_string(quizID) + '_' + "Results.txt";

    ifstream inputFile;
    inputFile.open(fileName, ios::in);

    string line;
    getline(inputFile, line, '\n');
    while (line != rollNumber)
    {
        getline(inputFile, line, '\n');
        if (line == "")
            break;
    }

    if (line == rollNumber)
    {
        while(line != "QuizID: " + to_string(quizID))
        {
            getline(inputFile, line, '\n');
        }
        getline(inputFile, line, '\n');
        cout << line << endl;
        getline(inputFile, line, '\n');
        cout << line << endl;
    }
    else
        cout << "This student has not attempted this quiz." << endl;
}
bool checkForQuizValidity(Course checkCourse, int quizID)
{
    for (int i = 0; i < checkCourse.getTotalQuizzes(); i++)
    {
        if (checkCourse.getQuizzes()[i].getQuizID() == quizID)
            return true;
    }
    return false;
}

void studentActions(Course &teacherCourse, Quiz &teacherCreatedQuiz)
{
    clearScreen();
    cin.ignore();

    string rollNumber, password;
    cout << "Enter your roll number (in the format \'XXI-XXXX\'): ";
    getline(cin, rollNumber);
    cout << "Enter your password: ";
    PasswordField passwordField;
    password = passwordField.getline();
    cout << endl;

    // Checking if the roll number and password match
    while (!studentAuth(rollNumber, password))
    {
        cout << "Invalid login!" << endl;
        cout << "Enter your roll number: ";
        getline(cin, rollNumber);
        cout << "Enter your password: ";
        getline(cin, password);
    }

    // Creating student object
    Student loggedInStudent(rollNumber, password);

    while (true)
    { 
        clearScreen();

        int choice = 0;

        cout << "Welcome " << loggedInStudent.getName() << "!" << endl;
        cout << "\nWhat do you want to do?" << endl;
        cout << "1. Attempt a quiz" << endl;
        cout << "2. Change your password" << endl;
        cout << "3. Logout" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;

        while (choice < 1 || choice > 3)
        {
            cout << "\nInvalid choice! Please try again." << endl;
            cout << "Enter your choice: ";
            cin >> choice;
        }

        if (choice == 1)
        {
            clearScreen();
            loggedInStudent.printDetails();
            cout << endl;

            // Asking user for what course to select
            int courseIndex;
            cout << "Enter the index of the course you want to select: ";
            cin >> courseIndex;

            while (courseIndex < 0 || courseIndex >= loggedInStudent.getTotalCoursesEnrolled())
            {
                cout << "\nInvalid index! Please try again." << endl;
                cout << "Enter the index of the course you want to select: ";
                cin >> courseIndex;
            }

            cout << endl;
            loggedInStudent.getCourse(courseIndex).printDetails();
            cout << endl;

            // Generating 2 quizzes and adding it to the course
            loggedInStudent.generateQuizForCourse(courseIndex);
            loggedInStudent.generateQuizForCourse(courseIndex);
            
            // Adding the teacher created quiz to the course if the course is the same
            if (loggedInStudent.getCourse(courseIndex) == teacherCourse)
                loggedInStudent.addQuizToCourse(courseIndex, teacherCreatedQuiz);

            // Printing the quizzes for the course
            for (int i = 0; i < loggedInStudent.getCourse(courseIndex).getTotalQuizzes(); i++)
            {
                loggedInStudent.getCourse(courseIndex).getQuizzes()[i].printDetails();
                cout << endl;
            }
            // Asking user for what quiz to select
            int quizID;
            cout << "Enter the index of the quiz you want to select: ";
            cin >> quizID;
            
            while (!(checkForQuizValidity(loggedInStudent.getCourse(courseIndex), quizID)))
            {
                cout << "Invalid index! Please try again." << endl;
                cout << "Enter the index of the quiz you want to select: ";
                cin >> quizID;
            }
            cout << endl;

            int obtainedMarks = loggedInStudent.getCourse(courseIndex).getQuizzes()[quizID].attemptQuiz();
            writeQuizToFile(loggedInStudent.getCourse(courseIndex), quizID);
            writeResultsOfQuiz(loggedInStudent.getRollNumber(), loggedInStudent.getCourse(courseIndex), quizID, obtainedMarks);
            
            cout << "Quiz attempt successful." << endl;
            cout << "Press enter to continue.";
            cin.ignore();
            cin.get();
        }
        else if (choice == 2)
            loggedInStudent.inputPassword();
        else if (choice == 3)
            return;
    }    
}
void teacherActions(Course &loggedInTeacherCourse, Quiz &createdQuiz)
{
    clearScreen();
    cin.ignore();
    string username, password;
    cout << "Enter your username: ";
    getline(cin, username);
    cout << "Enter your password: ";
    PasswordField passwordField;
    password = passwordField.getline();
    cout << endl;

    // Checking if the username and password match
    while (!teacherAuth(username, password))
    {
        cout << "Invalid login!" << endl;
        cout << "Enter your username: ";
        getline(cin, username);
        cout << "Enter your password: ";
        password = passwordField.getline();
    }

    clearScreen();

    // Creating teacher object
    Teacher loggedInTeacher(username, password);

    while (true)
    {
        int choice = 0;

        cout << "Welcome " << loggedInTeacher.getName() << "!" << endl;
        cout << "\nWhat do you want to do?" << endl;
        cout << "1. Add a question to the question bank" << endl;
        cout << "2. Create a quiz" << endl;
        cout << "3. Change your password" << endl;
        cout << "4. Get results of a quiz" << endl;
        cout << "5. Logout" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;

        while (choice < 1 || choice > 5)
        {
            cout << "Invalid choice!" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
        }

        if (choice == 1)
        {
            loggedInTeacher.addQuestionToBank();
            clearScreen();
        }
        else if (choice == 2)
        {
            loggedInTeacherCourse.setCourseCode(loggedInTeacher.getCourseTaughtCode());
            loggedInTeacherCourse.setCourseName(loggedInTeacher.getCourseTaughtName());
            loggedInTeacherCourse.setCourseTeacher(loggedInTeacher.getName());
            
            loggedInTeacher.createQuizManually(createdQuiz);

            clearScreen();
        }
        else if (choice == 3)
        {
            loggedInTeacher.inputPassword();
            clearScreen();
        }
        else if (choice == 4)
        {
            clearScreen();

            int quizID;
            string rollNumberOfStudent;

            cout << "Enter ID of quiz to check results of: ";
            cin >> quizID;

            cout << "Enter roll number of student to check results of: ";
            cin >> rollNumberOfStudent;

            cout << endl;
            getResultsOfQuiz(rollNumberOfStudent, loggedInTeacher.getCourseTaughtName(), quizID);

            // Pausing the screen
            cout << "\nPress enter to continue.";
            cin.ignore();
            cin.get();

            clearScreen();
        }
        else if (choice == 5)
            return;
    }
}
