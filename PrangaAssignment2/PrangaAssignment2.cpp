/*| Written by: Dylan Pranga								|*/
/*|	Note: AvlTree class original author: Mark Allen Weiss	|*/
/*|		  Modified by Dylan Pranga.							|*/
/*| Grand Valley State University							|*/
/*|															|*/
/*| Course:			CIS263 - SP/SU 2020 - Section 02		|*/
/*| Professor:		Dr. Jonathan P. Leidig					|*/
/*| Project:		Top Secret Message Application (AVL)	|*/
/*| Description:	Collects user message data and stores	|*/
/*|					the information in an AVL tree to be	|*/
/*|					accessed using a password.				|*/
/*|															|*/
/*| Project Notes/Comments:									|*/
/*|	- The "DataValidator" class can be better implemented	|*/
/*|	  to further sanitize bad input.						|*/
/*|	- When submitting duplicate UIDs, it might be a good	|*/
/*|	  idea to throw an error instead of saying "success."	|*/

#include <iostream>
#include <string>
#include "AvlTree.h"

//Definitions for console text coloring.
#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

using namespace std;

//Static class used to get user input.
static class DataValidator
{
public:
	//Asks the user to provide a message ID and returns it as an integer.
	int getUserUID() {
		int id;
		cout << "Provide a unique message ID: ";
		cin >> id;
		return id;
	}
	//Asks the user to provide a password and returns it as a string.
	string getUserPass() {
		string pass;
		cout << "Provide the password: ";
		if (cin.peek() == '\n')
			cin.ignore();
		getline(cin, pass);
		return pass;
	}
	//Asks the user to provide a message and returns it as a string.
	string getUserMsg() {
		string msg;
		cout << "Provide your message: ";
		if (cin.peek() == '\n')
			cin.ignore();
		getline(cin, msg);
		return msg;
	}
	//Asks the user to provide a maximum attempt count and returns it as an integer.
	int getUserAtt() {
		int att;
		cout << "Provide the maximum number of attempts to access your message: ";
		cin >> att;
		return att;
	}
} DV;

class Menu {
public:
	//Varible used as a "message center" for application status.
	string notification = "";
	AvlTree<int> database;

	//Main method for providing the user main menu options.
	void provideUserOptions() {
		system("cls");
		int userSelection;
		cout << notification << endl;
		cout << RESET;
		cout << "==================================================" << endl;
		cout << "Top Secret Message Menu" << endl;
		cout << "==================================================" << endl;
		cout << "1.) Create a new message." << endl;
		cout << "2.) Access a message." << endl;
		cout << "3.) Exit" << endl;
		cout << "==================================================" << endl;
		cout << "What would you like to do?: ";
		cin >> userSelection;
		//This if statement is used for input validation. (Prevents infinite loop/exception.)
		if (cin.fail()) {
			cin.clear();
			std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			userSelection = -1;
		}

		switch (userSelection) {
		case 1:
			cout << YELLOW;
			notification = "[NAVIGATION] Entering message creation module.";
			makeNewMessage();
			break;
		case 2:
			cout << YELLOW;
			notification = "[NAVIGATION] Entering message access module.";
			accessMessage();
			break;
		case 3:
			exit(0);
			break;
		default:
			cout << RED;
			notification = "[ERROR] Invalid main menu option selected.";
			provideUserOptions();
			break;
		}
	}

private:
	//Method used for creating a new secret message. (Takes user input.)
	void makeNewMessage() {
		system("cls");
		cout << notification << endl;
		cout << RESET;
		cout << "==================================================" << endl;
		cout << "Create New Top Secret Message" << endl;
		cout << "==================================================" << endl;
		int uid = DV.getUserUID();
		string pass = DV.getUserPass();
		string msg = DV.getUserMsg();
		int att = DV.getUserAtt();

		database.insert(uid, pass, msg, att);
		cout << GREEN;
		notification = "[ACTION] Message successfully submitted to database.";
	}
	//Method used for accessing previously stored messages. (Takes user input.)
	void accessMessage() {
		system("cls");
		cout << notification << endl;
		cout << RESET;
		cout << "==================================================" << endl;
		cout << "Access a Top Secret Message" << endl;
		cout << "==================================================" << endl;
		int uid = DV.getUserUID();
		string pass = DV.getUserPass();
		string test = "";

		try {
			string TopSecretMessage = database.tryRetrieve(uid, pass);
			cout << GREEN;
			notification = "[ACTION] Message successfully retrieved: " + TopSecretMessage;
		}
		catch (RequestException ex) {
			cout << RED;
			notification = "[ERROR] " + ex.content;
		}
	}
};


int main( )
{
	Menu M = Menu();

	//The application will run indefinitely until the user decides to exit the program.
	while (true) {
		M.provideUserOptions();
	}

	return 0;
}