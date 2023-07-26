#include <future>
#include <iostream>
#include <string>
using namespace std;

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
