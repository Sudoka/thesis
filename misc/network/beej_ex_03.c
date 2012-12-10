#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

/*
 * beej_ex_03.c - from beej's guide to networking, example of 
 *   simple port-binding
 * dcashman
 */

int main(int argc, char *argv[]){
  struct addrinfo hints, *res;
  int sockfd; //the socket file descriptor
  int status;

  //first, load up address structs with getaddrinfo()
 
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  
  if((status = getaddrinfo(NULL, "3490", &hints, &res)) != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }

  if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol))
     < 0){
    fprintf(stderr, "socket() failed with error num: %d\n", errno);
    return 2;
  }

  //bind it to the port we passed in to getaddrinfo()
  /* bind(int sockfd, sockaddr *my_addr, int addrlen) 
   *    sockfd - socket file descriptor
   *    my_addr - pointer to struct sock_addr which has port, address, etc.
   *    addr_len - bytes for my_addr
   */
  if(bind(sockfd, res->ai_addr, res->ai_addrlen) < 0){
    fprintf(stderr, "bind() failed with error nuum: %d\n", errno);
    return 3;
  }

  /* the OLD way of doing things, without getaddrinfo():
   *   (without error checking for brevity)  
   */
 
  /* 
  int sockfd;
  struct sockaddr_in my_addr; //ipv4 struct
  sockfd = socket(PF_INET, SOCK_STREAM, 0);

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(MYPORT); //port is 2 bytes (short)
  //could use INADDR_ANY instead of "10.12.128.57" for localhost (below)
  my_addr.sin_addr.s_addr = inet_addr("10.12.128.57"); 
  memset(my_addr.sin_zero, '\0', sizeof(myaddr.sin_zero));

  bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr));
  */

  if(close(sockfd) != 0){
    fprintf(stderr, "Error closing socket descriptor: %dn", sockfd);
    return 4;
  }

  return 0;
}
