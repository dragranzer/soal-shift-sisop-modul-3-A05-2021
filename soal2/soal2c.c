#include <stdlib.h>
#include <unistd.h>

int p;
int pipe1[2];
int pipe2[2];

void main() {

  // create pipe1
  pipe(pipe1);

  // fork (ps aux)
  p = fork();
  if (p == 0) {
    exec1();
  }
  // parent

  // create pipe2
  pipe(pipe2);

  // fork (sort -nrk)
  p=fork();
  if (p == 0) {
    exec2();
  }
  // parent

  // close unused fds
  close(pipe1[0]);
  close(pipe1[1]);

  // fork (head -5)
  p=fork();
  if (p == 0) {
    exec3();
  }
  // parent
}


void exec1() {
  // input from stdin (already done)
  // output to pipe1
  dup2(pipe1[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  // exec
  char *arg[] = {"ps", "aux", NULL};
  execv("/bin/ps", arg);
  // exec didn't work, exit
  perror("bad exec ps");
  _exit(1);
}

void exec2() {
  // input from pipe1
  dup2(pipe1[0], 0);
  // output to pipe2
  dup2(pipe2[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  char *arg[] = {"sort", "-nrk", "3.3", NULL};
  execv("/bin/sort", arg);
  // exec didn't work, exit
  perror("bad exec grep root");
  _exit(1);
}

void exec3() {
  // input from pipe2
  dup2(pipe2[0], 0);
  // output to stdout (already done)
  // close fds
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  char *arg[] = {"head","-5", NULL};
  execv("/bin/head", arg);
  // exec didn't work, exit
  perror("bad exec grep sbin");
  _exit(1);
}
