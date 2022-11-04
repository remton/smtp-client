# smtp-client
simple smtp-client program to send a message with an optional text file attachment via command line on Linux 

Author: Remington Ward
Date: 11/04/2022

This project was made for a class
CS 4390 Networking with professor Kresman

This program runs on linux and will send an smtp email on the given server and port number.
Sends emailText with emailFileAttachment as senderEmail to rcvEmail.
The file attachment must be a text or similar file. Binary files are not supported.

Files required in directory: 
main.cpp, smtpClient.h, smtpClient.cpp, clientServer.h, clientServer.cpp

Arguments:
serverName, portNO, senderEmail, rcvEmail, emailText, emailFileAttachement
emailFileAttachment is optional. Include path if the file is in another directory.

Compile command: 
g++ clientServer.cpp main.cpp smtpClient.cpp

Example run command:
./a.out smtp.bgsu.edu 25 rtward@bgsu.edu rtward@bgsu.edu "a message" FILE.txt

