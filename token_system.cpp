/*
 * This file is the source code of a token system.
 *
 * Developed for C++ project assignment in class E23CQCN01-N at PTIT HCM.
 * This product includes source code developed by Ngo Hoang Long and Truong Nhat Hoang.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

/// Clears all the output, depends on the OS.
void clearScreen() {
    #ifdef _WIN32
    std::system("cls");
    #else
    std::system("clear");
    #endif
}

/**
 * Token object.
 *
 * An object representing a token message created by the program.
 * When construct with the following order:
 * `message`, `creator`, `receiver`, `secret_code`;
 * The object will be created and the information can be read by
 * using the `ReadMessage` method provided by the object.
 */
class Token {
private:
    /// The message content of the token.
    std::string message;
    /// The message secret code of the token.
    std::string secret_code;
public:
    /// The message creator of the token.
    std::string creator;
    /// The message receiver of the token.
    std::string receiver;
    /// Object constructor.
    Token(std::string message, std::string creator, std::string receiver, std::string secret_code) :
        message(message), creator(creator), receiver(receiver), secret_code(secret_code) {}

    /**
     * Get the message content of the token.
     *
     * @param usr_name The user name.
     * @param usr_code The secret code of the token.
     * @return The `content` of the message token.
     */
    std::string ReadMessage(std::string usr_name, std::string usr_code) {
        if (secret_code.empty()) {
            return "Public message from " + creator + ": " + message;
        } else {
            if (usr_name == receiver && usr_code == secret_code) {
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

/**
 * Using Encapsulation and Data Hiding, we have a class
 * called `StoreToken` which is used for storing all
 * the tokens, and can be read through a function.
 */
class StoreToken {
private:
    std::vector<Token> token_list;
public:
    void addToken(const Token& token) {
        token_list.push_back(token);
    }

    /// Gets all the private and public token message for a user.
    std::string getMessagesForUser(const std::string& usr_name) const {
        std::string above_message;
        for (Token token : token_list) {
            if (token.creator != usr_name) {
                /**
                 * Compare to `*` for everyone (public) message.
                 * If not, check if the `receiver` of the token
                 * is the current user in the session.
                 */
                if (token.receiver == "*") {
                    above_message += token.ReadMessage("*", "")+ "\n";
                } else if (token.receiver == usr_name) {
                    clearScreen();
                    std::cout << "Enter the secret code for the message from " << token.creator << ": ";
                    std::string inp_code;
                    std::getline(std::cin, inp_code);
                    above_message += token.ReadMessage(usr_name, inp_code) + "\n";
                }
                /**
                 * This will append all messages to a variable `above_message`,
                 * which display above the screen and below is the session selection.
                 */
            }
        }
        return above_message;
    }
};

/// Displays the main menu.
void mainMenu() {
    clearScreen();
    std::cout << "1. Press 1 to login" << std::endl;
    std::cout << "0. Press 0 to quit" << std::endl;
}

/// Displays the session menu.
void sessionMenu(std::string usr_name, std::string above_message) {
    /**
     * `above_message` will act as the response from the program to the user.
     * This include showing the message that the user has, any error the user makes
     * while using the program, etc.
     */
    std::cout << above_message << "\n" << std::endl;
    std::cout << "1. Press 1 to create a new message to everyone." << std::endl;
    std::cout << "2. Press 2 to create a new message to a person." << std::endl;
    std::cout << "3. Press 3 to read messages for you." << std::endl;
    std::cout << "0. Press 0 to close this session (return to main menu)" << std::endl;
}

/// Handles the session menu selection.
void sessionMenuSelection(StoreToken& store) {
    clearScreen();
    std::string usr_name, above_message;
    while (usr_name.empty() == true) {
        std::cout << "Enter your user name: ";
        std::getline(std::cin, usr_name);
    }
    above_message =  "Welcome, " + usr_name;

    /// Put the program runs in a loop until the user ends the session.
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
            /**
             * Avoid sending an empty message.
             */
            if (message.empty() == true) {
                above_message = "You cannot send an empty message.";
                continue;
            }
            Token new_token(message, usr_name, "*", "");
            store.addToken(new_token);
            above_message = "Created a message to everyone successfully!";

        } else if (inp == "2") {
            clearScreen();
            std::cout << "Create a message to a person.\n\n";
            std::string message, secret_code, receiver;
            std::cout << "Enter the user name to receive the message: ";
            std::getline(std::cin, receiver);
            /// Avoid the user sending the token to themselves by checking the receiver.
            if (receiver == usr_name) {
                above_message = "You cannot message to yourself.";
                continue;
            }
            std::cout << "Enter the password for the message: ";
            std::getline(std::cin, secret_code);
            /**
             * std::string has a bool attribute .empty(), which allows us
             * to check if the string for the secret code is empty.
             */
            if (secret_code.empty() == true) {
                above_message = "You cannot leave the password empty.";
                continue;
            }
            std::cout << "Enter your message: ";
            std::getline(std::cin, message);
            /**
             * We cannot send an empty message.
             */
            if (message.empty() == true) {
                above_message = "You cannot send an empty message.";
                continue;
            }
            Token new_token(message, usr_name, receiver, secret_code);
            store.addToken(new_token);
            above_message = "Created a message to " + receiver + " successfully!";

        } else if (inp == "3") {
            std::string messages = store.getMessagesForUser(usr_name);
            if (messages.empty()) {
                above_message = "You have no message.";
            } else {
                above_message = messages;
            }
        } else if (inp == "0") {
            break;
        } else {
            above_message = "Invalid option. Please try again.";
        }
    }
}

int main() {
    StoreToken store;

    while (true) {
        mainMenu();
        std::cout << "\nEnter your selection: ";
        std::string inp;
        std::getline(std::cin, inp);

        if (inp == "0") {
            break;
        } else if (inp == "1") {
            sessionMenuSelection(store);
        } else {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    }

    return 0;
}
