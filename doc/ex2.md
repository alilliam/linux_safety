#### 1.实验原理
 ![pic1](./assets/8.png)


#### 2.实验环境：
Ububtu 16.04
#### 3.实验过程：
##### 实验1 打印环境变量
```c
#include<stdio.h>
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
```
 ![pic1](./assets/10.png)

##### 实验2 shell 变量和环境变量

 ![pic1](./assets/7.png)
 ![pic1](./assets/9.png)
 ![pic1](./assets/11.png)

##### 实验3 动态链接环境变量LD_PRELOAD

```C
//mytest.c
#include<stdio.h>
#include<unistd.h>
int main(int argc, char*argv[], char* envp[])
{
  sleep(1);
  return 0;

}
```
```C
//sleep.c
#include<stdio.h>

void sleep(int s)
{
  printf("I am not sleeping.\n");

}
```

* 将sleep.c编译成动态链接库 libmylib.so.1.0.1并加入LD_PRELOAD环境变量中
![pic1](./assets/12.png)
![pic1](./assets/13.png)

* 当EUID和RUID不同时，LD_PRELOAD环境变量被忽视
  ![pic1](./assets/14.png)
  ![pic1](./assets/15.png)

##### 实验4 外部程序注入
```c
//vul.c
#include<stdlib.h>
int main(int argc, char*argv[], char* envp[])
{
  system("cal");
  return 0;

}
```

```c
//cal.c
#include<stdlib.h>
int main(int argc, char*argv[], char* envp[])
{
  system("/bin/dash");
  return 0;

}
```

* 修改PATH
  
![pic1](./assets/17.png)
![pic1](./assets/16.png)
* ppt中这里获得了root权限，本次实验结果是用户权限

#### 4.总结

修改环境变量可能造成程序有意向不到的结果