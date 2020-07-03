#include<stdio.h>
#include<unistd.h>
extern char ** environ;
int main(int argc, char*argv[], char* envp[])
{
  int i = 0;
  char* v[2];
  char* newenv[3];

  if(argc < 2) return 0;
  v[0] = "/usr/bin/env";
  v[1] = NULL;
  newenv[0] = "AAA=aaa";
  newenv[1] = "BBB=bbb";
  newenv[2] = NULL;

  switch(argv[1][0])
  {
    case '1':
      execve(v[0], v, NULL);
    case '2':
      execve(v[0], v, newenv);
    case '3':
      execve(v[0], v, environ);
    default:
      execve(v[0], v, NULL);

  }
  return 0;

}