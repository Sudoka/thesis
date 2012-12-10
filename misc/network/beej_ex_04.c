/* beej_ex_04.c - example using teh connect() call to establish a
 *  connection.  Example from beej's networking guide
 * dcashman
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
  
  /* connect usage:
      int connect(int sockfd, struct sockaddr *serv_addr,
          int addr_len);
  */
  /* sockfd - the socket file descriptor on this machine
     serv_addr - destination addres: port and IP
     addrlen - size of struct serv_addr points to
  */

  struct addrinfo hints, *res;
  int sockfd, status;
 

  // first, load up addres struct with getaddrinfo()
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if(status = (getaddrinfo("localhost", "3490", &hints, &res))
     != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }
  
  //create a socket
  if((sockfd = socket(res->ai_family, res->ai_socktype, 
		      res->ai_protocol)) < 0){
    fprintf(stderr, "socket failed with error num: %d\n", errno);
    return 2;
  }

