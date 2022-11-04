#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <bitset>
#include "smtpClient.h"
#include "clientServer.h"

using namespace std;

void writeData(int socketNum, string dataString);
string readData(int socketNum);
string clientName();

//connects to the smtp server
//returns true if could connect to server
bool SmtpClient::connectToServer(string serverAddress, int portNum) {
    connectedToServer = false;

    //create socket
    socketNum = socket(AF_INET, SOCK_STREAM, 0);
    if ((socketNum) < 0) {
        cout << "--- FAILED TO CREATE SOCKET ---" << endl;
        return false;
    }
    cout << "--- Socket Created ---" << endl;

    //connect to server
    char serverName[serverAddress.length() + 1];
    strcpy(serverName, serverAddress.c_str());
    int connected = connectToHost(socketNum, serverName, portNum);
    if (connected < 0) {
        cout << "--- FAILED TO CONNECT TO SERVER ---" << endl;
        return false;
    }
    cout << "--- Connected to server ---" << endl << endl;
    connectedToServer = true;

    //Read response from server
    string msg;
    msg = readData(socketNum);
    cout << "RECIEVED: " << msg << endl;

    //If we did not recieve a good response
    //220 <domain> Service ready
    if (msg.find("220") == string::npos) {
        cout << "--- Not good connection response ---" << endl;
        disconnect();
        return false;
    }

    //Send HELO
    msg = "HELO ";
    msg += clientName();
    msg += "\r\n";
    writeData(socketNum, msg);
    cout << "SENT: " << msg << endl;

    //Read HELO response from server
    msg = readData(socketNum);
    cout << "RECIEVED: " << msg << endl;
    if (msg.find("250") == string::npos) {
        cout << "--- Not good HELO response ---" << endl;
        disconnect();
        return false;
    }
    return true;
}

//ends the session and disconnects
void SmtpClient::disconnect() {
    if (!connectedToServer)
        return;
    string msg;
    msg = "QUIT \r\n";
    writeData(socketNum, msg);
    cout << "SENT: " << msg << endl;
    connectedToServer = false;
    hasSender = false;
    hasRecipient = false;
}


//sets the recipient of the message
void SmtpClient::setReciever(string recieverAddress) {
    string msg;

    //Send RCPT TO: command to server
    msg = "RCPT TO: <";
    msg += recieverAddress;
    msg += ">\r\n";
    writeData(socketNum, msg);
    cout << "SENT: " << msg << endl;

    //Read response from server
    msg = readData(socketNum);
    cout << "RECIEVED: " << msg << endl;
    if (msg.find("250") == string::npos) {
        cout << "--- Not good RCPT TO: response ---" << endl;
        disconnect();
        return;
    }
    hasRecipient = true;
}

//sets the sender of the message
void SmtpClient::setSender(string senderAddress) {
    if (!connectedToServer)
        return;
    string msg;

    //Send MAIL FROM command to server
    msg = "MAIL FROM: <";
    msg += senderAddress;
    msg += ">\r\n";
    writeData(socketNum, msg);
    cout << "SENT: " << msg << endl;

    //Read response from server
    msg = readData(socketNum);
    cout << "RECIEVED: " << msg << endl;
    if (msg.find("250") == string::npos) {
        cout << "--- Not good MAIL FROM: response ---" << endl;
        disconnect();
        return;
    }
    hasSender = true;
}

//sets the text message of the email
void SmtpClient::setMessage(string message) {
    this->message = message;
}

//Adds the filePath as the attachment if it can be opened
//returns true if file was found and successfully added
bool SmtpClient::addAttachment(string filePath) {
    //Open file and copy to string fileData
    ifstream file;
    file.open(filePath);
    if (!file.is_open()) {
        cout << " --- File: " << filePath << " could not be opened ---" << endl;
        hasAttachment = false;
        return false;
    }

    //Parse path for file name
    size_t lastSlash = filePath.find_last_of("/");
    if (lastSlash == string::npos)
        attachmentFileName = filePath;
    else
        attachmentFileName = filePath.substr(lastSlash);

    //Read data
    hasAttachment = true;
    attachmentData.clear();
    string line;
    while (getline(file, line)) {
        attachmentData += line;
        attachmentData.push_back('\n');
    }
    file.close();

    return true;
}

//sends the email. returns true if successfully sent
bool SmtpClient::sendEmail() {
    if (!connectedToServer || !hasRecipient || !hasSender) {
        cout << "--- Failed to send email --- " << endl;
        return false;
    }

    string msg; // message sent or read by server
    string msg_condense; // a condensed version of msg to be written to console when needed

    //Send DATA command
    msg = "DATA \r\n";
    writeData(socketNum, msg);
    cout << "SENT: " << msg << endl;

    //Read response from server
    msg = readData(socketNum);
    cout << "RECIEVED: " << msg << endl;
    if (msg.find("354") == string::npos) {
        cout << "--- Not good DATA response ---" << endl;
        return false;
    }

    //Send message using MIME protocol in order to support attachments
    //Arbitrary boundry for mime protocol
    //must be a string not in the message body. Just kinda hoping its not there at this point
    string mimeBoundry = "-BOUNDARY-3a839m8-"; 
    //mime header and specify as multipart message
    if (hasAttachment) {
        //header
        msg = "MIME-Version: 1.0\n";
        msg += "Content-Type: multipart/mixed;\n";
        msg += "\tboundary=\"" + mimeBoundry + "\"\n\n";
        msg += "This is a multipart message in MIME format.\n\n";
        msg += "--" + mimeBoundry + "\n";

        //message
        msg += "Content-Type: text/plain;\n\n";
        msg += message + "\n\n";
        msg += "--" + mimeBoundry + "\n";

        //attachment
        msg += "Content-Type: text/plain;\n";
        msg += "Content-Disposition: application;\n";
        msg += "\tfilename=" + attachmentFileName + ";\n\n";
        msg_condense = msg;
        msg += attachmentData;
        msg_condense += "<file data>\n";
    }
    //No Attachment. Just set message for normal SMTP
    else {
        msg = message;
        msg_condense = message;
    }
    msg += "\r\n.\r\n";
    msg_condense += "\r\n.\r\n";

    //send to server
    writeData(socketNum, msg);
    cout << "SENT: " << msg_condense << endl;

    //Read response from server
    msg = readData(socketNum);
    cout << "RECIEVED: " << msg << endl;
    if (msg.find("250") == string::npos) {
        cout << "--- Not good message send response ---" << endl;
        return false;
    }

    return true;
}


//writes all data to socket
void writeData(int socketNum, string dataString) {
    //Intentionally do not include null terminator '\0'
    const int BUFFER_SIZE = dataString.length();
    char data[BUFFER_SIZE];
    strcpy(data, dataString.c_str());
    send(socketNum, data, BUFFER_SIZE, 0);
}

//returns a string that contains the data read from socket
string readData(int socketNum) {
    const int BUFFER_SIZE = 1024;
    char data[BUFFER_SIZE];
    int numRcvBytes;
    string dataString = "";
    do {
        numRcvBytes = recv(socketNum, data, BUFFER_SIZE, 0);
        if(numRcvBytes > 0)
            dataString += data;
    } while (numRcvBytes >= BUFFER_SIZE);
    return dataString;
}

//Returns the client's local machine name
//Used with HELO command
string clientName() {
    char hostName[128];
    gethostname(hostName, 128);
    string name = hostName;
    return hostName;
}