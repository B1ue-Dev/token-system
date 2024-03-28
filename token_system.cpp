#include <iostream>
#include <string>
#include <vector>

/*
Clear all the output, depends on the current OS.
*/
void clearScreen() {
    #ifdef _WIN32
    std::system("cls";
    #else
    std::system("clear");
    #endif
}

class Token {
private:
    std::string message;
    std::string secret_code;
public:
    // user_name of the owner.
    std::string creator;
    /*
     user_name of the receiver.
    if receiver="*", the message is public
    */
    std::string receiver;
    Token(std::string message, std::string creator, std::string receiver, std::string secret_code) :
        message(message), creator(creator), receiver(receiver), secret_code(secret_code) {} //constructor

    /*
    Access the content of the message.
    std::string usr_name The user name.
    std::string usr_code The message code.
    */
    std::string ReadMessage(std::string usr_name, std::string usr_code) {
        if (usr_name != receiver) {
            return "You are not the intended receiver of this message.";
        }

        if (secret_code.empty()) {
            return message;
        } else {
            if (usr_code == secret_code) {
                return message;
            } else {
                return "Incorrect secret code. You cannot read this message.";
            }
        }
    }
};

std::vector<Token> token_list;

/*
A function to get the user input, for writing less code.
*/
std::string getUserInput() {
    std::string inp;
    std::cin >> inp;
    return inp;
}

/*
Displays the main menu.
*/
void mainMenu() {
    clearScreen();
    std::cout << "1. Press 1 to login" << std::endl;
    std::cout << "0. Press 0 to quit" << std::endl;
}

/*
Displays the session menu.
*/
void sessionMenu() {
    bool first_time = true;
    std::string above_message = "";
    clearScreen();
    std::string usr_name;
    std::cout << "Enter your user name: ";
    std::cin >> usr_name;
    clearScreen();
    std::cout << "Welcome, " << usr_name << "!\n\n";

    while (true) {
        if (first_time == true) {
            first_time = false;
        } else {
            clearScreen();
        }

        std::cout << above_message;
        std::cout << "1. Press 1 to create a new message to everyone." << std::endl;
        std::cout << "2. Press 2 to create a new message to a person." << std::endl;
        std::cout << "3. Press 3 to read messages for you." << std::endl;
        std::cout << "0. Press 0 to close this session (return to main menu)" << std::endl;
        above_message = "";

        std::string inp = getUserInput();

        if (inp == "1") {
            std::string message;
            std::cout << "Enter your message: ";
            std::cin.ignore();
            std::getline(std::cin, message);
            Token new_token(message, usr_name, "*", "");
            token_list.push_back(new_token);
            above_message = "Created a message to everyone successfully!\n\n";

        } else if (inp == "2") {
            std::cout << "Message to a person." << std::endl;

        } else if (inp == "3") {
            for (Token token : token_list) {
                if (token.creator != usr_name) {
                    if (token.receiver == "*") {
                        above_message += "Public message from  "+ token.creator + ": " + token.ReadMessage("*", "") + "\n\n";
                    }
                }
            }

        } else if (inp == "0") {
            break;
        } else {
            above_message = "Invalid option. Please try again.";
        }
    }
}

int main() {
    while (true) {
        mainMenu();
        std::string inp = getUserInput();

        if (inp == "0") {
            break;
        } else if (inp == "1") {
            sessionMenu();
        } else {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    }

    return 0;
}
