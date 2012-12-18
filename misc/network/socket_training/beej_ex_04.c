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
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;


  if(status = (getaddrinfo("localhost", "33333", &hints, &res))
     != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }
  
  //create a socket
  if((sockfd = socket(AF_INET, SOCK_STREAM, 
		      0)) < 0){
    fprintf(stderr, "socket failed with error num: %d\n", errno);
    return 2;
  }


  //bind socket to our desired port!
  if((status = bind(sockfd, res->ai_addr, res->ai_addrlen))
     != 0){
    fprintf(stderr, "Binding socket failed with error num: %d\n", 
	    status);
    return 3;
  }else{
    if(res->ai_family == AF_INET){
      struct sockaddr_in *ipv4 = (struct sockaddr_in *) res->ai_addr;
      void *addr = &(ipv4->sin_addr);
      unsigned short port = ipv4->sin_port;
      char ipstr[17];
      inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));
      printf("Bound socket to IP: %s and Port: %d\n", ipstr, ntohs(port));
    }
  }

  //now connect to example.com
  
  //first create a new sockadrr_in
  struct addrinfo *res2;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if((status = getaddrinfo("www.example.com", "80", &hints, &res2))
     != 0){
    fprintf(stderr, "Error getting addrinfo for %s: %s\n",
	    "www.example.com", gai_strerror(status));
    return 1;
  }

  //create a socket (again for debug)
  int sockfd2;
  if((sockfd2 = socket(res2->ai_family, res2->ai_socktype, 
		      res2->ai_protocol)) < 0){
    fprintf(stderr, "socket failed with error num: %d\n", errno);
    return 2;
  }


  /* more debug, using alternative connect method */

  struct sockaddr_in serv_addr;
  struct hostent *server;

  server = gethostbyname("www.example.com");
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(80);


  /* end 'more debug' */

  if((status = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) 
     < 0){
    fprintf(stderr, "Error making connection on socket %d. "
	    "%s\n", sockfd, strerror(errno));
    return 4;
  }

  if((status = close(sockfd)) != 0){
    fprintf(stderr, "Error closing socket descriptor %d. "
	    "Error number %d\n", sockfd, errno);
    return 5;
  }

  if((status = close(sockfd2)) != 0){
    fprintf(stderr, "Error closing socket descriptor %d. "
	    "Error number %d\n", sockfd, errno);
    return 5;
  }

  return 0;
}
