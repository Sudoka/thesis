/*
 *beej_ex_02.c - copy of code from beej's networking guide 
 *  demonstrating getaddrinfo by showing ip from hostname
 *  input.
 *dan cashman 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[]){
  struct addrinfo *p, *res, hints;
  int status;
  char ipstr[INET6_ADDRSTRLEN];
  char *hostname = argv[1];

  if(argc != 2){
    fprintf(stderr, "Usage: beej_ex_02 hostname\n");
    return 1;
  }

  
  
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }

  printf("IP address for %s\n", hostname);

  for(p = res; p != NULL; p = p->ai_next){
    void *addr;
    char *ipver;

    /*get the pointer to the address itself
      different fields in IPv4 and IPv6: */
    if(p->ai_family == AF_INET){
      struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";  //for printing only
    }else if(p->ai_family == AF_INET6){
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }else{
      printf("ai_family neither ipv4 or ipv6: unhandled case\n");
      return 3;
    }
    
    //convert the IP to a string and print it:
    inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
    printf(" %s: %s\n", ipver, ipstr);

    //example socket creation using getaddrinfo():
    int s;
    if((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0){
      fprintf(stderr, "socket() generated error num: %d\n", errno);
    }else{
      printf("Socket sucessfully created with id: %d\n", s);
    }
    
    
    /* equivalent to the above, but hard-coded */
    /*
    s = socket(PF_INET/PF_INET6, SOCK_STREAM/SOCK_DGRAM, 
           0/getprotobyname("udp")/getprotobyname("tcp));
    */
    if(close(s) != 0){
      fprintf(stderr, "Error closing socket descriptor: %dn", s);
      return 4;
    }
  }


  freeaddrinfo(res);
  
  return 0;
}
