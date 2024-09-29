#include <stdio.h>
#include <string.h>
#include <unistd.h>

void prompt(char *username, char *hostname, char *currentdir);
void mainloop();
int main(void){
  char *buffer;
  buffer = getlogin();
  printf("%s\n", buffer);
  return 0;
}

void prompt(char *username, char *hostname, char *currentdir){
  printf("$%s@%s->%s ");
}

void mainloop(){}

