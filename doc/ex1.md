#### 1.实验原理
- RUID：真实用户ID。用于在系统中标识一个用户是谁，当用户使用用户名和密码成功登录后一个UNIX系统后就唯一确定了他的RUID.
- EUID： 有效用户ID。用于系统决定用户对系统资源的访问权限，通常情况下等于RUID。
- SUID：保留用户ID。用于对外权限的开放。跟RUID及EUID是用一个用户绑定不同，它是跟文件而不是跟用户绑定。

通过修改EUID实现提权后的资源访问。


#### 2.实验环境：
Ububtu 16.04
#### 3.实验过程：
##### 实验1：修改文件所属用户及文件的用户权限
chown root filename 修改文件所属为root

chmod 4755 filename

若用chmod 4755 filename可使此程序具有root的权限.

chmod 4755 filename中的第一位数4表示高位特殊权限（Set-UID比特）

主要有三种高位特殊权限 S ：SUID SGID SBIT

![1.png](https://i.loli.net/2021/03/08/3O8EAWRcPYlvTQj.png)

 SUID 4 :当一个设置了SUID 位的可执行文件被执行时，该文件将以所有者的身份运行，也就是说无论谁来执行这个文件，他都有文件所有者的特权。如果所有者是 root 的话，那么执行人就有超级用户的特权了。（对文件进行操作，eg：修改密码的时候除了root用户可以修改密码，普通用户可以修改密码，原因就是因为采用了SUID，普通用户以root用户的权限来执行修改密码的操作）

##### 实验2：使用id命令查看euid

Linux id命令用于显示用户的ID，以及所属群组的ID。
id会显示用户以及所属群组的实际与有效ID。若两个ID相同，则仅显示实际ID。若仅指定用户名称，则显示目前用户的ID。

![2.png](https://i.loli.net/2021/03/08/AG3lcf74KxHkhOo.png)

##### 实验3：资源泄露
###### 实验原理：
执行exec后，进程ID没有改变，但新程序从调用进程继承了文件描述符的状态（除非特地用fcntl设置了关闭标志）
文件句柄打开后没有及时关闭

###### getuid()获取实际用户id
###### geteuid()获取有效用户id
```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
void main()
{  
    int fd;  
    char *v[2];  
    /* Assume that /etc/zzz is an important system file,   
    * and it is owned by root with permission 0644.   
    * * Before running this program, you should create   
    * * the file /etc/zzz first. */  
    fd = open("/etc/zzz", O_RDWR | O_APPEND);    
    if (fd == -1) 
    {     
        printf("Cannot open /etc/zzz\n");     
        exit(0);  
    }  
    // Print out the file descriptor value  
    printf("fd is %d\n", fd);  
    // Permanently disable the privilege by making the  
    // effective uid the same as the real uid  
    setuid(getuid());           
    // Execute /bin/sh  
    v[0] = "/bin/sh"; 
    v[1] = 0;  
    execve(v[0], v, 0);        
}

```
![3a.png](https://i.loli.net/2021/03/09/oZcWp2OPzl9BuQ3.png)
![3.png](https://i.loli.net/2021/03/08/S3fsDmnJHh47gyw.png)


##### 实验4：没成功
###### 实验原理：仅当对程序文件设置了设置用户ID位时，exec函数才设置有效用户ID（EUID）
```c
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    char *cat = "/bin/cat";
    if(argc < 2)
    {
        printf("Input a file name.\n");
        return 1;
    }
    char *command = malloc(strlen(cat) + strlen(argv[1]) + 2);
    sprintf(command, "%s %s", cat, argv[1]);
    system(command);
    return 0;
}
```

![4.png](https://i.loli.net/2021/03/09/VmbWf1n7DuQFkCR.png)

###### 有些实现通过更改/bin/sh，当有效用户ID与实际用户ID不匹配时，将有效用户ID设置为实际用户ID，这样可以关闭安全漏洞。
In Ubuntu 16.04, /bin/sh points to /bin/dash, which has a countermeasure
It drops privilege when it is executed inside a set-uid process

![5.png](https://i.loli.net/2021/03/09/scxetTUWoig4Av3.png)


#### 4.总结

在程序中使用execve函数替代system，使代码和数据分开，防止注入
