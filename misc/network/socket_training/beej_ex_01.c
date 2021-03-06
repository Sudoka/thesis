#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){

  //code from Beej's network guide
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo;
  
  //init hints addrinfo struct
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; //use ipv4 or ipv6
  hints.ai_socktype = SOCK_STREAM; //TCP socket
  hints.ai_flags = AI_PASSIVE; 

  if((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0){
    printf("getaddrinfo failed\n");
    exit(1);
  }

  char address[24];

  //servinfo now points to a linked list of 1 or more struct addrinfo

  freeaddrinfo(servinfo);  //free the addrinfo structure

  return 0;
}
