#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

//UDP - Server
int main()
{

   int Sock;

   //Default port and network interface on which the server will listen if these values are not given when starting the server.
   int Server_Port = 13333;
   char * Server_IP = "127.0.0.1";
   int Rem_Port = 13334;
   char * Rem_IP = "127.0.0.1";

   int nbytes = 0;
   char buf [80];
   int end = 0;
   struct sockaddr_in add;
   struct sockaddr_in from;
   int adl = sizeof(from);

   printf("You can run this program as follows: %s [Server_Port [Server_IP]]\n");

   //Creating socket 
   if((Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))<0) {
      perror("Error. Socket not created.");
      return 1;
   }
   printf("Socket created. Descriptor: %d\n", Sock);

//Binding the socket to the network interface and port on which the server will listen.
   memset(&add, 0, sizeof(struct sockaddr_in) );
   add.sin_family = AF_INET;
   add.sin_addr.s_addr = inet_addr(Server_IP);
   add.sin_port = htons((u_short) Server_Port);

   if(bind(Sock, (struct sockaddr*) &add, sizeof(add))<0) {
      perror("Error. Socket not bound.");
      close(Sock);
      return 1;
   }
   printf("Socket is bound\n");

//Waiting for data
   while(! end) {
      memset(buf, 0, 80);
      memset(&from, 0, sizeof(struct sockaddr_in));
      from.sin_family = AF_INET;
      from.sin_addr.s_addr = htonl(INADDR_ANY);
      from.sin_port = htons((u_short) 0);

//Receiving data (if available)
      nbytes = recvfrom( Sock, buf, 80, 0, (struct sockaddr*) &from, &adl );
      if(nbytes>0) {

         printf( "Received req from Sock %s:%d (%d bytes)\n", inet_ntoa( from.sin_addr ), ntohs(from.sin_port), nbytes );
         printf( "Data: \n%s\n", buf );
      }

   if (!strncmp("get time",buf,8)){
        time_t mytime = time(NULL);
        char * time_str = ctime(&mytime);
        time_str[strlen(time_str)-1] = '\0';
        for (int i = 0;i<strlen(time_str)-1;i++)
        {
                buf[i] = time_str[i];
                printf("%c", time_str[i]);
        }
        //Sending data to client
        memset(&add, 0, sizeof(struct sockaddr_in));
        add.sin_family = AF_INET;
        add.sin_addr.s_addr = inet_addr(Rem_IP);
        add.sin_port = htons((u_short) Rem_Port);
        sendto(Sock, buf, strlen(buf), 0, (struct sockaddr*) &add, sizeof(add) );
        close(Sock);
        end = 1;
        }
   else{
        perror("Unknown request");
        }
}
return 0;
}