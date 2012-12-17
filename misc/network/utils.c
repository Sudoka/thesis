#include "utils.h"

void dump_bytes(const unsigned char *data_buffer, const unsigned int length){
  unsigned char byte;
  unsigned int i, j;
  for(i = 0; i < length; i++){
    byte = data_buffer[i];
    printf("%02x ", data_buffer[i]); //display byte in hex
    if(((i%16) == 15) || (i == length - 1)){ //line size of 16 characters
      for(j = 0; j < 15 - (i%16); j++){
	printf("  "); //2 hex chars per byte
      }
      printf("| "); //hex vs string delim
      for(j = (i - (i%16)); j <= i; j++){  //print out string ver
	byte = data_buffer[j];
	if((byte > 31) &&(byte < 127)){ //printable char
	  printf("%c", byte);
	}else{
	  printf("."); //some crazy data
	}
      }
      printf("\n"); //return at end of line
    }//end of if for line length
  }
  return;
}

/* isValidIpAddress 
 * 
 */
bool isValidIpAddress(char *ipAddress){
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
  return result != 0;
}
	
