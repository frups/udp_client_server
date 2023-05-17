#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

int Sock;
int LocalPort = 13334;
char * LocalIP = "127.0.0.1";
int RemPort = 13333;
char * RemIP = "127.0.0.1";
int nbytes = 0;
int end = 0;

//UDP - Client
int main(int argc, char *argv[])
{
   char buf [80];
   struct sockaddr_in add;
   struct sockaddr_in from;
   int adl = sizeof(from);

//Creating socket 

   if((Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))<0) {
      perror("Error. Socket not created.");
      return 1;
   }
   printf("Socket created. Descriptor: %d\n", Sock );

//Binding socket to the LocalIP=127.0.0.0.1 and port assigned by system (LocalPort=13334).

   memset(&add, 0, sizeof(struct sockaddr_in) );
   add.sin_family = AF_INET;
   add.sin_addr.s_addr = inet_addr(LocalIP);
   add.sin_port = htons((u_short) LocalPort);

   if(bind( Sock, (struct sockaddr*) &add, sizeof(add))<0) {
      perror("Error. Socket not bound.");
      close(Sock);
      return 1;
   }
   printf("Socket is bound\n");

   printf("Sock will communicate with %s:%d\n", RemIP, RemPort);

   while(! end) {
        memset(buf, 0, 80);
        printf("Enter data string to send: ");
        fgets(buf,80,stdin);
        add.sin_family = AF_INET;
        add.sin_addr.s_addr = inet_addr(RemIP);
        add.sin_port = htons((u_short) RemPort);
        //Sending to server
        sendto(Sock, buf, strlen(buf), 0, (struct sockaddr*) &add, sizeof(add) );
        add.sin_family = AF_INET;
        add.sin_addr.s_addr = htonl(INADDR_ANY);
        add.sin_port = htons((u_short) RemPort);
        //Receiving data
        nbytes = recvfrom( Sock, buf, 80, 0, (struct sockaddr*) &from, &adl );
        if(nbytes>0){
        printf( "Received req from Sock %s:%d (%d bytes)\n", inet_ntoa( from.sin_addr ), ntohs(from.sin_port), nbytes );
        printf( "Data: \n%s\n", buf );
        }
  }
   close(Sock);
   return 0;
}