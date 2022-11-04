/**

BG Linux instructor code, BGlinux-Instructor-Code.cpp.
You can use this code instead of instructor obj code.
Compile yuor code and instructor code, as in

$g++ yourHomework3.cpp BGlinux-Instructor-Code.cpp

*/


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "clientServer.h"

int connectToHost (int socketNO, 
	char * hostName, int portNo) {

 struct sockaddr_in socketAddr ;
 struct hostent *hp;
 if ((hp = gethostbyname(hostName)) == NULL)  {
    fprintf(stderr,"%s: no such host ?\n",hostName);
    exit(1);
 }  

 else {
   printf("hostname is  %s\n",hp->h_name); 
   //bcopy (hp->h_addr,  (char *)socketAddr.sin_addr.s_addr,hp->h_length); 
   bcopy (hp->h_addr,  (char *)&socketAddr.sin_addr, hp->h_length); 
   socketAddr.sin_family = AF_INET;

   socketAddr.sin_port = htons(portNo);  /* port number given in n/w format */
   
   if (connect(socketNO,( struct sockaddr *) &socketAddr,
		sizeof(socketAddr)) < 0)  {
   		perror("connect");
   exit(1);
  }
 }
 return (OK);
}



int buildServer(int socketNO, int portNO)
{
 struct sockaddr_in socketAddr;
 struct hostent *hp;
 char hostName[33];
 int lengthOfStruct; 


 gethostname(hostName, 32); 
 if ((hp = gethostbyname(hostName)) == NULL)  {
    	fprintf(stderr,"%s: no such host ?\n",hostName);
    	exit(1);
 }  

 else {
   	printf("\nServer Hostname:     %s\n",hp->h_name); 

   	socketAddr.sin_family = AF_INET;
   	socketAddr.sin_port =  portNO;
   	socketAddr.sin_addr.s_addr = INADDR_ANY;

 	if (bind (socketNO, (struct sockaddr *) &socketAddr,
		sizeof(socketAddr)) < 0){
   		perror("bind");
   		exit(1);
 	}                                                         

 	lengthOfStruct = sizeof (socketAddr);
 	if (getsockname(socketNO,( struct sockaddr *)&socketAddr,
		(socklen_t *) &lengthOfStruct)) {
    		perror("getting socket name");
    	exit(1);
 	}
 	printf("Server Port Number:  %d\n",ntohs(socketAddr.sin_port));
 }
 return (OK);
}


int  hookupWithClient (int socketNO)
{
 struct sockaddr_in fullSocketAddr;
 int lengthOfStruct, connNO;

 lengthOfStruct = sizeof (fullSocketAddr);
 if ((connNO = accept(socketNO,(struct sockaddr *)&fullSocketAddr,
		(socklen_t *) &lengthOfStruct)) < 0)  {
      perror("accept");
      exit(1);
 }
 return (connNO);
}


