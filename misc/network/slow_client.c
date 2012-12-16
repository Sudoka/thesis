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
#include "utils.h"

/* hard coded port values now user-input.  Deprecated */
#define LOCAL_PORT 55555
#define DEST_PORT 4445

#define RECV_BUFFER_SIZE 1024 //1 kb

int main(int argc, char *argv[]){
  short local_port, dest_port;
  int sockfd, status;
  struct sockaddr_in cli_addr, serv_addr;

  if(argc != 3){
    fprintf(stderr, "Error.  Usage: cli local-port dest-port\n");
    return -1;
  }else{
    if((sscanf(argv[1], "%hd", &local_port)) < 0){
      fprintf(stderr, "Input local-port must be a short integer. %s\n", 
	      strerror(errno));
      return -1;
    }
    if((sscanf(argv[2], "%hd", &dest_port)) < 0){
      fprintf(stderr, "Input dest-port must be a short integer. %s\n", 
	      strerror(errno));
      return -1;
    }
  }
  /* init client address info */
  cli_addr.sin_family = AF_INET;
  cli_addr.sin_port = htons(local_port); // short, network byte order
  cli_addr.sin_addr.s_addr = INADDR_ANY;
  memset(cli_addr.sin_zero, '\0', sizeof cli_addr.sin_zero);
  /* init server address info */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(dest_port); // short, network byte order
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero);
  
  //create a socket
  if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){

    fprintf(stderr, "socket failed with error num: %d\n", errno);
    return 2;
  }


  /* set socket options */
  int sockop_flag = 1;
  if(status = (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
			  &sockop_flag, sizeof(sockop_flag))) < 0){
    fprintf(stderr, "Socket option setting failed %s\n", strerror(errno));
    return -1;
  } 


  //bind socket to our desired port!
  if((status = bind(sockfd, (struct sockaddr *)&cli_addr, sizeof cli_addr))
     != 0){
    fprintf(stderr, "Binding socket failed: %s\n", 
	    strerror(errno));
    return 3;
  }else{  //print sanity check
    struct sockaddr_in *ipv4 = (struct sockaddr_in *) &cli_addr;
    void *addr = &(ipv4->sin_addr);
    unsigned short port = ipv4->sin_port;
    char ipstr[17];
    inet_ntop(AF_INET, addr, ipstr, sizeof(ipstr));
    printf("Bound socket to IP: %s and Port: %d\n", ipstr, ntohs(port));
    }


  //now connect to server
  if((status = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) 
     < 0){
    fprintf(stderr, "Error making connection on socket %d. "
	    "%s\n", sockfd, strerror(errno));
    return 4;
  }

  /* send test */
  int bytes_recvd;
  char recv_buffer[RECV_BUFFER_SIZE];
  bytes_recvd = recv(sockfd, &recv_buffer, RECV_BUFFER_SIZE, 0);
  send(sockfd, "ping\n", 5, 0);
  while(bytes_recvd > 0){
    bytes_recvd = recv(sockfd, &recv_buffer, RECV_BUFFER_SIZE, 0);
    dump_bytes(recv_buffer, bytes_recvd);
    send(sockfd, recv_buffer, bytes_recvd, 0);
    sleep(10);
  }

  if((status = close(sockfd)) != 0){
    fprintf(stderr, "Error closing socket descriptor %d. "
	    "Error number %d\n", sockfd, errno);
    return 5;
  }

  close(sockfd);

  return 0;
}
