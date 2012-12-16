/* slow_server.c - a server which receives messages and responds
 *   by  echo-ing back input after a delay.
 *
 * for educational and concrete purposes, this does not use the 
 *   newer getaddrinfo() workflow.  This uses ipv4.
 * Dan Cashman
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "utils.h"


#define LISTEN_PORT 4444
#define INPUT_BUFF_SIZE 1024 // 1 kb
#define BACKLOG_QUEUE_SIZE 10

int main(int argc, char *argv[]){

  int sockfd, new_sockfd;
  short listen_port;
  struct sockaddr_in serv_addr, cli_addr; //using ipv4.
  int bytes_recvd = 1, status;
  socklen_t addr_size;
  char input_buffer[INPUT_BUFF_SIZE];
  
  if(argc != 2){
    fprintf(stderr, "Error.  Usage: serv port-number\n");
    return -1;
  }else{
    if((sscanf(argv[1], "%hd", &listen_port)) < 0){
      fprintf(stderr, "Input port must be an integer. %s\n", 
	      strerror(errno));
      return -1;
    }
  }




  //create socket on which server listens
  if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Initial socket creation failed: %s\n", strerror(errno));
    return -1;
  }

  //create serv address and bind socket to our port
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(listen_port);
  serv_addr.sin_addr.s_addr = INADDR_ANY; //listen on our host port, could also be 0.
  memset(serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

  /* set socket options */
    int sockop_flag = 1;
  if(status = (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
			  &sockop_flag, sizeof(sockop_flag))) < 0){
    fprintf(stderr, "Socket option setting failed %s\n", strerror(errno));
    return -1;
    } 

  if((status = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)))
     < 0){
    fprintf(stderr, "Binding to designated server port failed: %s\n", 
	    strerror(errno));
    return -1;
  }

  if((status = listen(sockfd, BACKLOG_QUEUE_SIZE)) < 0){
    fprintf(stderr, "Attemp to listen failed: %s\n", strerror(errno));
    return -1;
  }
  while(1){  //accept loop
    addr_size = sizeof(cli_addr);
    new_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &addr_size);
    if(new_sockfd < 0){
      fprintf(stderr, "Error accepting connection request and creating"
	      " new socket: %s\n", strerror(errno));
      return -1;
    }
    printf("Accepted connection from %s on port %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
    
    //send greeting
    send(new_sockfd, "Hello from server!\n", 13, 0);
    bytes_recvd = recv(new_sockfd, &input_buffer, INPUT_BUFF_SIZE, 0);
    while(bytes_recvd > 0){
      printf("RECV: %d bytes.\n", bytes_recvd, input_buffer);
      dump_bytes(input_buffer, bytes_recvd);
      send(new_sockfd, input_buffer, bytes_recvd, 0);
      bytes_recvd = recv(new_sockfd, &input_buffer, INPUT_BUFF_SIZE, 0);
      //sleep(5);  better to do this as client
    }
    close(new_sockfd);
  }
  
  close(sockfd);
  return 0;
}
 


