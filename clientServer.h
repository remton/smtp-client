#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
 

 

#define ERROR -1
#define OK 0
 
#define systemMessageExit()   (_fatal_status (__LINE__,__FILE__))
#define myMessageExit(s) (_fatal_message (__LINE__,__FILE__,(s)))
 

int connectToHost(int socketNO, char* hostName, int portNo);
int buildServer(int socketNO, int portNO);
int  hookupWithClient(int socketNO);

//Remi made this one
void getLocalHostName(char* name, int buffSize);