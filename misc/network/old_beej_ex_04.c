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

  int sockfd, status;
  struct sockaddr_in cli_addr, serv_addr;
  
  cli_addr.sin_family = AF_INET;
  cli_addr.sin_port = htons(4447); // short, network byte order
  cli_addr.sin_addr.s_addr = inet_addr("192.168.43.128");
  //  cli_addr.sin_addr.s_addr = INADDR_ANY;
  memset(cli_addr.sin_zero, '\0', sizeof cli_addr.sin_zero);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(44444); // short, network byte order
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  //serv_addr.sin_addr.s_addr = inet_addr("192.0.43.10");
  memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero);

  
  //create a socket
  if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){

    fprintf(stderr, "socket failed with error num: %d\n", errno);
    return 2;
  }

  //bind socket to our desired port!
  if((status = bind(sockfd, (struct sockaddr *)&cli_addr, sizeof cli_addr))
     != 0){
    fprintf(stderr, "Binding socket failed with error num: %d\n", 
	    status);
    return 3;
  }else{  //print sanity check
    struct sockaddr_in *ipv4 = (struct sockaddr_in *) &cli_addr;
    void *addr = &(ipv4->sin_addr);
    unsigned short port = ipv4->sin_port;
    char ipstr[17];
    inet_ntop(AF_INET, addr, ipstr, sizeof(ipstr));
    printf("Bound socket to IP: %s and Port: %d\n", ipstr, ntohs(port));
    }


  //now connect to example.com
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

  close(sockfd);

  return 0;
}
