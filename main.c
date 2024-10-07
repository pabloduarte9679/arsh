#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUFSIZE 1024

//integrated commands 

void exit_arsh(){
  exit(EXIT_SUCCESS);
}



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

char **parsearguments(char *cmd){
  char **args = (char**)malloc(sizeof(char*)*100);
  int index = 0;
  char *arg = strtok(cmd, " ");
  while(arg != NULL){
    args[index++] = arg;
    arg = strtok(NULL, " ");
  }
  args[index] = NULL;
  return args;
}
int run(char **args){
  pid_t proc;
  int status, wpid;
  proc = fork();
  if(proc == 0){
    if(execvp(args[0], args) == -1)
      perror("arsh");
    exit(EXIT_FAILURE);
  }else if(proc < 0){
    perror("arsh");
  }else{
    do{
      int wpid = waitpid(proc, &status, WUNTRACED);
    }while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return status;
}
void mainloop(char *usr, char *hostname, char *homedir){
  while(1){
    char *buffer = NULL;
    char dir[40];
    char rdir[20] = {' '};
    char **args = NULL;
    getcwd(dir, 40);
    if(strcmp(homedir, dir) == 0){
      dir[0] = '~';
      dir[1] = '\0';
    }
    relativepath(dir, rdir);
    printf("[%s@%s %s]$ ", usr, hostname, rdir);
    size_t size = BUFSIZE;
    getline(&buffer, &size, stdin);
    int bsize = strlen(buffer);
    buffer[bsize - 1] = '\0';
    char go[] = {"exit"};
    if(strcmp(go, buffer) == 0){
      exit_arsh();
    }
    args = parsearguments(buffer);
    int status = run(args);
    free(args);
    free(buffer);
  }
}

int main(void){
  signal(SIGINT, SIG_IGN);
  char *username = getlogin();
  char hostname[20];
  gethostname(hostname, 20);
  char *homedir;
  homedir = getenv("HOME");
  mainloop(username, hostname, homedir);
  return 0;
}
