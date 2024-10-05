#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 1024

void relativepath(char *dir, char *rdir){
  char c;
  int sbs = 0;
  char tmp[strlen(rdir)];
  for(int i = strlen(dir); i >= 0; i--){
    if(dir[i] != '/'){
      tmp[sbs++] = dir[i];
    }else{
      break;
    }
  }
  for(int i = 0; i < sbs; i++){
    rdir[i] = tmp[sbs - i - 1];
  }
  rdir[sbs] = '\0';
}

void mainloop(char *usr, char *hostname, char *homedir){
  while(1){
    char *buffer = NULL;
    char dir[40];
    char rdir[20] = {' '};
    getcwd(dir, 40);
    if(strcmp(homedir, dir) == 0){
      dir[0] = '~';
      dir[1] = '\0';
    }
    relativepath(dir, rdir);
    printf("[%s@%s %s]$ ", usr, hostname, rdir);
    size_t size = BUFSIZE;
    getline(&buffer, &size, stdin);
    char cmp[2] = {"\n\0"};
    if(strcmp(buffer, cmp) != 0){
     printf("%s", buffer); 
    }
    
    free(buffer);
  }
}

int main(void){
  char *username = getlogin();
  char hostname[20];
  gethostname(hostname, 20);
  char *homedir;
  homedir = getenv("HOME");
  mainloop(username, hostname, homedir);
  return 0;
}
