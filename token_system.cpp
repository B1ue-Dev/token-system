#include <iostream>
#include <string>
#include <vector>

// Clears all the output, depends on the OS.
void clearScreen() {
    #ifdef _WIN32
    std::system("cls");
    #else
    std::system("clear");
    #endif
}

class Token {
private:
    std::string message;
    std::string secret_code;
public:
    std::string creator;
    std::string receiver;
    Token(std::string message, std::string creator, std::string receiver, std::string secret_code) :
        message(message), creator(creator), receiver(receiver), secret_code(secret_code) {} // class constructor

    /*
    Access the content of the message.
    std::string usr_name The user name.
    std::string usr_code The message code.
    */
    std::string ReadMessage(std::string usr_name, std::string usr_code) {
        if (secret_code.empty()) {
            return "Public message from " + creator + ": " + message;
        } else {
            if (usr_code == secret_code) {
                return "Private message from " + creator + ": " + message;
            } else {
                return "Incorrect secret code. You cannot read this message from " + creator + ".";
            }
        }
    }
    std::string DebugObject() {
        std::string msg = "";
        msg += message + "";
        msg += secret_code + "";
        msg += creator + "";
        msg += receiver + "";
        return msg;
    }
};

std::vector<Token> token_list;

// Displays the main menu.
void mainMenu() {
    clearScreen();
    std::cout << "1. Press 1 to login" << std::endl;
    std::cout << "0. Press 0 to quit" << std::endl;
}

// Displays the session menu.
void sessionMenu(std::string usr_name, std::string above_message) {
    std::cout << above_message << "\n" << std::endl;
    std::cout << "1. Press 1 to create a new message to everyone." << std::endl;
    std::cout << "2. Press 2 to create a new message to a person." << std::endl;
    std::cout << "3. Press 3 to read messages for you." << std::endl;
    std::cout << "0. Press 0 to close this session (return to main menu)" << std::endl;
}

// Handles the session menu selection.
void sessionMenuSelection() {
    clearScreen();
    std::string usr_name, above_message;
    std::cout << "Enter your user name: ";
    std::getline(std::cin, usr_name);
    above_message =  "Welcome, " + usr_name;

    while (true) {
        clearScreen();
        std::string inp;
        sessionMenu(usr_name, above_message);
        above_message = "";
        std::cout << "\nEnter your selection: ";
        std::getline(std::cin, inp);

        if (inp == "1") {
            clearScreen();
            std::string message;
            std::cout << "Create a new message to everyone.\n\n";
            std::cout << "Enter your message: ";
            std::getline(std::cin, message);
            Token new_token(message, usr_name, "*", "");
            token_list.push_back(new_token);
            above_message = "Created a message to everyone successfully!";

        } else if (inp == "2") {
            clearScreen();
            std::cout << "Create a message to a person.\n\n";
            std::string message, secret_code, receiver;
            std::cout << "Enter the user name to receive the message: ";
            std::getline(std::cin, receiver);
            if (receiver == usr_name) {
                above_message = "You cannot message to yourself.";
                continue;
            }
            std::cout << "Enter the password for the message: ";
            std::getline(std::cin, secret_code);
            if (secret_code.empty() == true) {
                above_message = "You cannot leave the password empty.";
                continue;
            }
            std::cout << "Enter your message: ";
            std::getline(std::cin, message);
            Token new_token(message, usr_name, receiver, secret_code);
            token_list.push_back(new_token);
            above_message = "Created a message to " + receiver + " successfully!";

        } else if (inp == "3") {
            for (Token token : token_list) {
                if (token.creator != usr_name) {
                    if (token.receiver == "*") {
                        above_message += token.ReadMessage("*", "")+ "\n";
                    } else if (token.receiver == usr_name) {
                        clearScreen();
                        std::cout << "Enter the secret code for the message from " << token.creator << ": ";
                        std::string inp_code;
                        std::getline(std::cin, inp_code);
                        above_message += token.ReadMessage(usr_name, inp_code) + "\n";
                    }
                }
            }
            if (above_message.empty() == true) {
                above_message = "You have no message.";
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
        std::cout << "\nEnter your selection: ";
        std::string inp;
        std::getline(std::cin, inp);

        if (inp == "0") {
            break;
        } else if (inp == "1") {
            sessionMenuSelection();
        } else if (inp == "2") {
            for (Token token: token_list) {
                std::cout << token.DebugObject() << std::endl;
            }
        } else {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    }

    return 0;
}
