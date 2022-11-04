//This is the driver file for the smtpClient class
//this file and smtp client.h and smtp client.cpp authored by Remington Ward
//clientServer.cpp authored by professor Kresman
//CS 4390 HW4 task 2 with professor Kresman

//compile along with clientServer.cpp and smtpClient.cpp

//This file sends an email with text and attachment on the given smtp server and port
//arguments should be given as follows:
//serverName, portNO, senderEmail, rcvEmail, emailText, emailFileAttachement
//emailFileAttachment is optional. Include path if the file is in another directory.
//only text files (or similar file ie: .cpp) are supported at the moment.
//remember to put quotes around emailText if there are spaces in the message

#include <string>
#include <iostream>
#include "smtpClient.h"
using namespace std;

int main(int argc, char* argv[]) {
	//Varify argvs
	// serverName, portNO, senderEmail, rcvEmail, emailText, emailFileAttachement
	if (argc < 6) {
		cout << "Invalid arguments. Arguments should be as follows:" << endl;
		cout << "serverName, portNO, senderEmail, rcvEmail, emailText, emailFileAttachement" << endl;
		return 0;
	}
	bool hasAttachment = (argc == 7);
	string serverName = argv[1];
	int portNO = stoi(argv[2]);
	string senderEmail = argv[3];
	string rcvEmail = argv[4];
	string emailText = argv[5];
	string emailFileAttachment = "\0";
	if (hasAttachment)
		emailFileAttachment = argv[6];

	//cout << endl << "ARGUMETS: " << endl;
	//cout << "server name: " << serverName << endl;
	//cout << "portNO: " << portNO << endl;
	//cout << "senderEmail: " << senderEmail << endl;
	//cout << "rcvEmail: " << rcvEmail << endl;
	//cout << "emailText: " << emailText << endl;
	//cout << "emailFileAttachment: " << emailFileAttachment << endl;
	//cout << endl;

	SmtpClient client;
	client.connectToServer(serverName, portNO);
	client.setSender(senderEmail);
	client.setReciever(rcvEmail);
	client.setMessage(emailText);
	if (hasAttachment) {
		string emailFileAttachment = argv[6];
		client.addAttachment(emailFileAttachment);
	}
	client.sendEmail();
	client.disconnect();
}