#include<string>
#include<cstdlib>
#include<fstream>
using namespace std;

class SmtpClient{
public:
	//connects to the smtp server
	//returns true if could connect to server
	bool connectToServer(string serverAddress, int portNum);

	//disconnects and ends the session
	void disconnect();

	//sets the recipient of the message
	void setReciever(string recieverAddress);

	//sets the sender of the message
	void setSender(string senderAddress);

	//sets the text message of the email
	void setMessage(string message);

	//Adds the file at filepath as an attachment
	//returns true if file was found and successfully added
	bool addAttachment(string filePath);

	//sends the email. returns true if successfully sent
	bool sendEmail();
private:
	int socketNum;
	bool connectedToServer = false;
	bool hasRecipient = false;
	bool hasSender = false;
	bool hasAttachment;
	string message;
	string attachmentFileName;
	string attachmentData;
	//string attachmentType;
};