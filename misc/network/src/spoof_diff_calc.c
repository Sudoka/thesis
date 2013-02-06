#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  unsigned int response_seq;
  unsigned int current_seq; 
  unsigned int spoof_seq;
  sscanf(argv[1], "%u", &response_seq);
  sscanf(argv[2], "%u", &current_seq);
  sscanf(argv[3], "%u", &spoof_seq);
  unsigned int result = (response_seq - current_seq) + spoof_seq;
  printf("%u", result);
  return 0;
}
