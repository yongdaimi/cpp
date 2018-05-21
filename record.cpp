
进程
======================

1.操作为每个结构体都创建了一个结构体PCB

struct task_struct {
	pid_t pid;
	umask()
	文件描述符表
	userid
	groupid
	进程可以使用的资源上限
}

2. environ 指向环境变量表

	extern char **environ;

3.设置和获取当前环境变量getenv()&setenv(),  
#include <stdlib.h>
char *getenv(const char *name);
getenv 的返回值是指向value的指针，未找到则返回NULL

#include <stdlib.h>
int setenv(const char *name, const char *value, int rewrite);
void unsetenv(const char *name); // 删除原来的键

例：
int main()
{
	printf("PATH=%s\n", getenv("PATH"));
	setenv("PATH", "hello", 1);
}

4.fork()
fork() 函数调用一次返回两次，在父进程返回子进程的PID， 在子进程中返回0
pid_t pid;

int n = 10;
pid = fork();
if (pid > 0) {
	while (1) {
		printf("I am a parent\n");
		printf("cur n is: %d\n", n++);
		sleep(1);	
	}
}
else if (pid == 0) {
	while (1) {
		printf("I am a child");
		printf("cur n is: %d\n", n++);
		sleep(3);
	}
	
} else {
	perror("fork");
	exit(1);
}

读时共享，写时复制
copy on write


5.getuid()&geteuid()
uid_t getuid() // 返回实际用户ID
uid_t geteuid() // 返回有效用户ID

getgid() // 返回实际用户组ID
getegid()// 返回有效用户组ID


6.exec族
拿一个程序的代码段，数据段，堆栈替换另一个程序的代码段，数据段，堆栈
execl("/bin/ls", "ls", "-l", NULL); // 
execl("/home/yongdaimi/teach/code/test", "./test", "11", ,"aaa", NULL);
execp() // 找文件的时候去PATH环境变量里找
execv("bin/ps", ps_argv);

一般情况下是把exec和fork绑定起来用的

7.wait()&waitpid()
僵尸进程：子进程退出，父进程没有回收子进程资源(PCB),则子进程变成僵尸进程

int main()
{
	pid_t pid;
	pid = fork();
	if (pid > 0) { // 父进程
		
		while(1) {
			printf("I am parent \n");
			printf("my pid is: %d\n", getpid());
			sleep(1);
		}

 	} else if (pid == 0) {
				
		printf("I am child\n");
		printf("my pid is: %d\n", getpid());
		sleep(10); // 令子进程睡眠10秒后退出，此时子进程就变成Z+状态，也就是僵尸态(当子进程运行结束后，其用户空间已被释放，但它的PCB没有被立即释放，等待父进程将其释放)		
	} else {
		perror("fork");
	}
}

如何除去僵尸进程？调用wait后：

int main()
{
	pid_t pid;
	pid = fork();
	if (pid > 0) { // 父进程
		
		while(1) {
			printf("I am parent \n");
			printf("wait for child %d\n", wait(NULL)); // 父进程可以通过wait函数来回收子进程的PCB。返回值是子进程的ID号
			sleep(1);
		}

 	} else if (pid == 0) {
				
		printf("I am child\n");
		printf("my pid is: %d\n", getpid());
		sleep(10); // 令子进程睡眠10秒后退出，此时子进程就变成Z+状态，也就是僵尸态(当子进程运行结束后，其用户空间已被释放，但它的PCB没有被立即释放，等待父进程将其释放)		
	} else {
		perror("fork");
	}
}
PS：
1.wait()函数是一个阻塞函数，等待回收子进程资源，如果没有子进程，wait返回-1
2.waitpid()函数则是一个非阻塞函数，

waitpid(pid_t pid, int *status, int options);
<-1 回收指定进程组内的任意子进程
-1 回收任意指定子进程
0 回收和当前调用waitpid()一个组的所有子进程
>0 回收指定ID的子进程

kill -9 -25889 向这个进程组的所有进程都改善结束信号，注意：是负号。
kill -9 -[进程组名]

8.waitpid()
WNOHANG 如果没有子进程退出立即返回，这样就实现了非阻塞的wait
waitpid(0, NULL, WNOHANG);	


9.进程间通信
进程间通信的几种方式
用户空间不共用，内核空间共用
管道就是内核当中的一块缓冲区。

实现原理：每个进程各自有不同的用户地址空间， 任何一个进程的全局变量在另一个进程中都看不到，所有进程之前要交换数据
必须通过内核，在内核中开辟一块缓冲区，进程1把数据从用户空间拷贝到内核缓冲区，进程2再从内核缓冲区把数据读
走，内核提供的这种机制称为进程间通信。

管道是一种最基本的IPC机制，由pipe函数创建

9.1 pipe管道(单向通信,单工,解决有血缘关系的进程通信)
int pipe(int filedes[2]);
int fd[2]; // 存放两个文件描述符，一个为管道的读端（fd[0]）,一个为管道的写端(fd[1])
当调用了pipe函数后，内核当中会创建一条管道(环形队列)
管道作用于有血缘关系的进程之间，通过fork()来传递，
通过pipe函数创建管道后，出存在两个文件描述符，可以[使子进程继承文件描述符]

实现原理：
继承PCB
继承文件描述符表

创建好管道后，需要[确定通信方向]
思考：两个进程通过一个管道只能实现单向通信？
确定单工的目的在于：假设父和子都持有写的文件描述符，假设父通过管道写了一段内容，子通过管道也写了一段内容，回头分不清楚，这段内容究竟是
谁写的。

1.父写子读（关闭父读,关闭子写）
2.子写父读(关闭子读,关闭父写)
若要实现双向通信，则需要建立两个管道。

int main()
{
	int fd[2];
	pid_t pid;
	char str[] = "hello baidu.com";
	char buf[1024];
	if (pipe(&fd) < 0) {
		perror("pipe");
		exit(1);
	}
	pid = fork();
	// 父写子读
	if (pid > 0) { // parent
		// 关闭父读,关闭子写
		close(fd[0]);
		write(fd[1], str, strlen(str));
		wait(NULL); // 回收子进程资源	
	} else if (pid == 0) { // child
		close(fd[1]);
		len = read(fd[0], buf, sizeof(buf));
		write(STDOUT_FILENO, buf, len);
	} else {
		perror("fork");
	}
}


注意事项：
1.写端关闭，读端读完管道里的内容时，再次读，返回0,相当于读到EOF
2.写端未关闭，写端暂时无数据，读端读完管道里的数据后，再次读，阻塞
3.读端关闭，写端写管道，产生SIGPIPE信号(17)，这个信号会导致写进程终止
4.读端未读管道数据，当写端写满管道数据后，再次写，阻塞

fpathconf(int fd, int name) 测试管道缓冲区大小，_PC_PIPE_BUF
一种是打开的时候增加非阻塞标志，另一种是打开了通过fcntl函数设置O_NONBLOCK标志。

令父进程睡眠5秒后，向子进程写数据

改变管道的非阻塞属性

int flags;
flags = fcntl(fd[0], F_GETFL);
flags |= O_NONBLOCK;
fcntl(fd[0], F_SETFL, flags); // 设置非阻塞

tryagain:
len = read(fd[0], buf, sizeof(buf));
if (len == -1) {
	if (errno == EAGAIN) { // 读一个非阻塞文件，如果没数据到达，就会出现EAGAIN
		write(STDOUT_FILENO, "try again\n", 10);
		sleep(1);
		goto tryagain;
	} else {
		perror("read");
		exit(1);
	}
}
write(STDOUT_FILENO, buf, len);
close(fd[0]);

9.2 fifo有名管道
解决无血缘关系的进程间通信
prw-rwxrwx
p这种文件类型称之为管道类型 Named pipe
mkfifo myfifo

实现原理：
在磁盘当中创建了一个文件结点：myfifo,仅仅是一个文件结点，没有大小，标记内核当中的一个pipe，将内核当中的管道标记成一个名
实际上也是在内核当中创建了一个缓冲区
打开文件的时候可以指定文件的打开属性。


void sys_error(char *str, exitno)
{
	perror(str);
	exit(exitno);
}


int main(int argc, char *argv[])
{
	int fd;
	char buf[] = "";
	fd = open(argv[1], O_WRONLY);
	if (fd < 0) {
		error("open", 1);
	}	
	write(fd, buf, strlen(buf));
	close(fd);
	
	return 0;
}

读：
O_RDONLY

可以先用access函数判断是否有管道，然后再
man 1 是命令，man 2 是函数

9.3 内存共享映射
mmap/munmap
mmap可以把磁盘文件的一部分直接映射到内存，这样文件中的位置直接就有对应的内存地址，对文件的读可以直接用指针来做，而不需要read/write函数

可以选中磁盘文件中的某一部分，以及我要映射的长度len,把这段内容映射到内存当中来
#include <sys/mman.h>


还需要为映射的内存分配权限：
PROT_EXEC
PROT_READ
PROT_WRITE
PROT_NONE

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
addr: 希望在内存的什么位置开始映射，即映射内存的起始位置。可设置成NULL，内核会自己在进程地址空间中选择合适的地址建立映射
length：申请内存的长度。
prot： 设置映射内存的执行权限。
flags： 状态标志。指定映射类型，SHARED（共享）/PRIVATED(私有)
fd： 文件描述符，磁盘文件的文件描述符
offset: 你要映射的磁盘文件从什么地址开始偏移。偏移量必须是4096的整数倍，偏移的时候是按页面为单位偏移的。

返回值 void *,mmap()函数所申请的内存的首地址


SHARED映射
磁盘或内存中的文件可以同步修改

PRIVATED映射
内存当中文件的修改，并不影响磁盘，反之亦然


len = lseek(fd, 0, SEEK_END);
mmap(NULL, len, PROT_READ|PROT_WRITE, );

close(fd); // 不会影响文件映射

基本思想： 共享一个磁盘文件，来实现进程间通信

9.4 Unix Domian Socket


信号
======================
一.信号的概念

1.1 信号编号
kill 默认会传递SIGINT信号
当操作系统不想让某个进程运行的时候，会给这个进程发送相应的结束信号
man 7 man第七章专门用来讲机制

1.2 kill() 函数
#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);

函数或者kill命令也可以向某一个函数或进程发送信号
int kill(pid_t pid, int sig)

当
pid > 0
	sig 发送给ID为pid的进程 (向某一个指定的进程发信号)
pid == 0 
	发送给与发送进程同组的所有进程
pid < 0
	发送给组ID为|-pid|的进程
pid == -1
	发送给发送进程有权限向他们发送信号的系统上的所有进程
sudo kill -9 -1

if (argc < 3) {
	printf("argc is error");
}
atoi((pid_t)argv[2]); // 将字符串转换成整型
atoi(argv[1]);

kill();
return 0;

信号的权限保护
普通用户只能向自己创建的进程发信号，无法向root用户或其它用户生成的进程发信号。

二.信号产生种类

1.3 raise() & abort()
int raise() // 可以自己向自己发信号
void abort(void) // 相当于调用进程自己向自己发送了一个SIGABRT(6)的信号

操作系统在管理进程的时候，为每个进程都分配了一个定时器(闹钟)，alarm, 
unsigned int alarm(unsigned int seconds);
精度为秒，可以设置定时多少秒，返回值是未睡够的秒数

alarm()可以定时指定的秒数，当指定的秒数到达以后，会向当前进程发送一个SIGALRM的信号，
sleep(1);
alarm(1); 仅仅是定时器定时1秒，并不会导致阻塞。


三. 进程处理信号的行为

1.默认处理动作
2.忽略
3.捕捉
	为某一个信号设置一个捕捉函数，当信号到达时，调用此函数
为一个信号设置默认捕捉函数

四 信号集处理函数
主要用来操作信号集

每个进程的PCB里面都维护了两个信号集

信号在内核中的表示示意图：

向当前进程发送一个(2)SIGINT信号
||
||
\/
=============		=============		=============
PCB			PEND未决信号集		阻塞信号集
			1号信号	0		1号信号	0	==>	handler(默认,忽略,捕捉)，只能是其中的一种
			2号信号	0		2号信号	1
			3号信号	0		3号信号	0
			4号信号	0		4号信号	0
			5号信号	0		5号信号	0
			...			...
			


内核把这个信号发送给进程，进程内部维护了两个信号集(未决信号集,阻塞信号集)，未决信号集记录：当前这个信号产生了,但是最终还没有被当前进程响应。在信号未产生之前，未决信号集默认初始值都是0,而指定编号的信号到来后，它会把指定信号置为1,表示几号信号产生了，2号信号产生了之后，它会继续把这件事向下一个信号集传送，如果此时指定编号的阻塞信号集是1,则代表当前信号不能被通过，此时handler所对应的默认处理动作就无法被执行，因为它被阻塞了，没有通过。此时，这个信号的状态就被称为未决态(信号产生了，但是还没有被执行)，

反之，假设信号产生了，而指定编号的阻塞信号集的标志位为0,(表示没有阻塞此信号), 那么这个信号会继承向下传递，此时会去判断，2号信号的默认处理动作，根据其动作再执行相应操作， 此时会将未决信号集中指定的标志位自动翻转成0。

信号产生并且被响应，这个过程我们称之为递达态。
信号产生没有被响应，这个过程我们称之为未决态。

阻塞信号集,用户可以设置（用户可以设置当前进程阻塞哪个信号）。
未决信号集,用户不可以设置（但是可以读），由内核自动设置。


有以下场景需要注意：
如果将2号信号SIGINT阻塞信号集置为1,则该信号会被阻塞，如果频繁按Ctrl+C,此信号仍然只会被记录一次，前32个信号不支持排队，不记录信号产生多少次，后32个信号(实时信号，跟驱动绑定的比较深)支持排队

五.设置阻塞信号集
内核提供了一组信号集操作函数来操作信号集(而不是操作阻塞信号集和未决信号集)， 只是去操作一个sigset_t 的信号集，不是去操作阻塞信号集和未决信号集
例：阻塞2号信号

int sigemptyset(sigset_t *set); // 清空设置信号集， 把所有信号全部置为0
int sigfillset(sigset_t *set); // 把信号集全部置为1
int sigaddset(sigset_t *set, int signo); // 把这个信号集的某一个信号位置为1
int sigdelset(sigset_t *set, int signo); // 把这个信号集的某一个信号位置为0
int sigismember(const sigset_t *set, int signo); // 判断这个信号集的第几个信号，是否置为1， 这是一个测试函数

操作阻塞信号集的步骤：
1.先定义一个信号集,sigset,再调用sigemptyset(); 将这个信号集清空
2.然后再sigaddset() 把2号信号的标志位置为1
3.然后再通过注册的方法，把当前信号集注册到阻塞信号集当中。

5.1 注册的函数

阻塞信号集也叫做信号屏蔽字
内核提供了一个sigprocmask()函数用来注册,它可以读取或更改进程的信号屏蔽字
sigprocmask(int how, const sigset_t *set, sigset_t *oset);

oset // 传出参数 传递原来进程的信号屏蔽字是什么
set  // 传入参数 传递新的信号屏蔽字是什么
how  // 

SIG_BLOCK	>>> set包含了我们希望添加到当前信号屏蔽字的信号,相当于mask=mask|set
SIG_UNBLOCK 		set包含了我们希望从当前信号屏蔽字中解除阻塞的信号,相当于mask=mask&~set, 取消原有阻塞信号集的某些信号屏蔽字
SIG_SETMASK 		设置当前信号屏蔽字为set所指向的值,相当于mask=set


sigpending(sigset_t *set)     // 获取当前进程的未决信号集


sigset_t s, p;
sizeof(s)

64个信号，每个信号占两个字节

sigismember

如果调用sigprocmask解除了对当前若干个信号的阻塞，则在sigprocmask返回前，至少将其中一个信号递达


C标准库提供的函数
======================================
一.C标准库提供的操作信号的函数
signal(int signum, sighandler_t handler); 这里的handler也是一个函数指针
缺点：不能设置信号屏蔽字，且无法获取原来的信号处理动作
优点：跨平台

int system(const char *command);
集合fork, exec, wait 于一体
system("firefox http://www.baidu.com");

二.可重入函数
信号会导致异步事件发生，因此在捕捉函数里面应尽量调用可重入函数
可重新进入函数

strtok函数的使用
readdir函数的使用


在信号捕捉函数里禁止调用不可重入函数
strtok
strtok_r
readdir
readdir_r

注意：sigempset(&act.sa_mask); // 注意这个值一定需要清空，因为act是一个局部变量，里面的sa_mask是一个垃圾值


三. 信号引起的时序竞态和异步IO
pause(void)
	使调用进程挂起，直到有信号递达，如果递达信号是忽略，则继续挂起

sigsuspend(const sigset_t *mask)


long long

四.SIGCHILD 信号处理
当子进程终止(运行结束)的时候，内核会向父进程发一个SIGCHLD的信号，告诉父进程你儿子挂了(父进程有义务要回收子进程)，wait() waitpid()
wait（） 阻塞等
waitpid() 可变成非阻塞
因此父进程可以在收到内核发送的结束信号时,再去调用wait(null),这样该函数就不会被阻塞了。


4.1 SIGCHILD的产生条件
子进程终止时
子进程接收到SIGSTOP信号停止时
子进程处在停止态，接收到SIGCONT后唤醒时。

由于这些原因，父进程还需要根据具体原因进行判断。看看子进程是否是因为正在终止导致的。

可以利用系统提供的下列宏函数来判断子进程的终止状态

wait()和waitpid的区别，waitpid() 还可以获取到子进程退出时的状态
WIFEXITED（status） 子进程是否已经正常退出
	WEXITSTATUS(status) 返回子进程的正常退出值
WIFSIGNALED(status)
	是否被信号终止
		WTERMSIG(status) 返回终止子进程的信号wfhg
WIFSTIOPPED（status）
	子进程是否被停止
	WSTOPSIG(status) 返回停止子进程的信号值

WIFCONTINUED（status）
	子进程由停止态转为就绪态(被激活)，返回真

options
	WNOHAND 没有子进程结束立即返回，其实就是把waitpid()变成非阻塞
	WUNTRACED 如果子进程由于被停止产生的SIGCHID，waitpid则立即返回
	WCONTINUED 如果子进程由于被SIGCONT唤醒而产生的SIGCHlD,waitpid则立即返回

回收子进程的方式
轮询 waitpid
阻塞 wait
信号（SIGCHLD， 推荐）sigchld+waitpid\

练习：
创建10个子进程
在子进程中打印自己是谁pid

在父进程中


先做一个没有回收子进程的程序，子进程会变成僵尸进程
回收僵尸进程(使用信号来回收)

子进程会克隆父进程，它会继承父进程所设的信号屏蔽字
先阻塞SIGCHLD信号

// 先设置捕捉
// 再解除对SIGCHLD的阻塞

SIGCHLD信号是经典信号，不支持排队

成功返回回收的进程ID，失败返回-1


4.2 向信号捕捉函数传参
int sigqueue(p
id_t pid, int signo, const union sivval value);
union sigval {
	int sival_int;
	void *sival_ptr;
};

pid 向哪个进程发送信号
也就意味着可以向一个进程传递一个整型或一个地址
因为两个进程的地址不一样，所以传递一个地址过去会很怪异。(如果这两个进程血缘关系则另当别论，因为fork出来的子进程,继承了父进程的一些东西)

假设我在父进程的数据段上定义了一个变量n,这个变量n所在地址在两个子进程当中所在的地址是相同的。这种情况下传递变量n是没有问题的。

当然了，如果你使用的是sigqueue给对方发送的信号，那对方接收的时候就不能再使用sa_handler了

需要调用下列函数去接收
sa_sigaction(int , siginfo_t *, void *)
siginfo_t {
	
}


我们传递的联合体的值封装在sigInfo_t结构体下的sigval_t 这个联合体当中

同时需要改变的还有sa_flags,这个时候需要使用
sa_flags = SA_SIGINFO 这个参数决定了sa_handler携带了3个参数，不是一个的那个

信号中断系统调用
read 阻塞时，信号中断系统调用
1.返回部分读到的数据
2.read调用失败，errno设成EINTER


四.进程间关系
4.1 终端
init进程-fork->exec->getty->用户输入帐号->login->输入密码->exec->shell

可以通过who 命令查看终端的名称
tty3 代表字符终端
pts 代表图形界面终端
Alt+F7 

0
1
2

标准输入，标准输出， 标准出错 dev/tty 指向当前所用的终端

4.2 可以通过ttyname打印当前终端的名字

进程里记录了自己的控制终端是谁
ps ajx 里面的tty代表当前进程使用的终端是什么， 打问号代表这个进程是没有控制终端的，有控制终端的会记录控制终端的编号是什么

4.3 进程组
一个或多个进程的集合，进程组ID是一个正整数。用来获得当前进程组的函数
getpgid(pid_t pid); // 获得指定进程ID的进程组ID，如果传递0，则代表获取当前进程的进程组ID
pid_t getpgrp(void)

组长进程可以创建一个进程组， 若组长进程此时终止， 则该进程组的其它进程的进程组号仍然不变，
当这个进程组的最后一个进程消亡之后，该进程组才真正消亡

setpgid() 加入一个现有的进程组或创建一个新进程组， 如改变父进程为新的组

想要操作的进程ID
进程组号

当进程ID和要设置的进程组ID号一同， 则意味着要使该进程成为组长进程


改变子进程为新的组， 应在fork后，exec前使用
非root进程只能改变自己创建的子进程， 或有权限操作的进程

信号可以向某个进程组发送消息

4.4 会话
ps ajx 中的SID是一个会话ID
会话ID是跟shell绑定的
SIGHUP 当用户退出shell时，由该shell启动的所有进程将收到这个信号， 默认动作为终止进程
向一个进程发送0号信号，测试这个进程是否存活


可以使一个程序成为一个新的会话(当控制终端结束之后，该进程还可以正常运行)
pid_t setsid()
1. 调用进程不能是进程组组长， 当调用完毕后，该进程会变成新会话首进程， 同时成为一个新进程组的组长进程
2.需要有root权限，ubuntu不需要
3.新会话丢弃原有的控制终端，该会话没有控制终端
4.若调用进程是组长进程，则出错返回
5.建立新会话时，先调用fork,父进程终止，子进程调用

linux上很多进程都是这样， tty是一个？
新会话脱离了控制终端，当这个窗口关闭后，这些没有控制终端，并且可以在后台保持长时间运行的进程，被称为守护进程


五 守护进程
5.1 概念
后台服务进程，生存期较长的进程，通过独立于控制终端并且周期性的执行某些任务或等待处理某些处理的事件
httpd 中的d,则说明这个进程是一个守护进程

5.2 守护进程编程步骤

改变当前工作目录，脱离之前的目录关系， 防止占用可卸载的文件系统
最终继承的文件屏蔽字是从shell进程继承下来的，防止继承的文件创建屏蔽字拒绝某些权限

关闭文件描述符
因为已经失去了控制终端，所以0,1,2 这些文件描述符已经没有意义了，

开始执行守护进程核心工作
守护进程退出处理

open("dev/null", O_RDWR);
dup2(0, 1); // 将0, 1, 2 这三个文件描述符都指向dev/null
dup2(0, 2);

周期性的做某些事&等待处理某些事(等待处理网络连接请求)

守护进程为什么要和控制终端脱离？
如果不小心把控制终端退出，守护进程就会结束

练习：
ctime() 将时间和日期以字符串格式显示
time()  函数可以得到当前的秒数

脱离控制终端

八. 线程概念
=================================================
8.1 线程和进程的关系
	线程被称为轻量级的进程，也有Pcb, 创建线程和创建进程使用的底层函数都是一样的
	从内核里看进程和线程是一样的,都有各自不同的PCB,但是PCB中指向内存资源的三级页(4096)表是相同的
	在linux下，线程是最小的执行单位，进程是最小的资源分配单位

	当创建了一个进程后，系统为该进程分配0-4G的地址空间，当创建一个线程的时候不会再额外开辟地址空间，而是和之前的进程共享地址空间
	
	
	查看LWP号 (轻量级进程)
	ps -Lf pid
	ps -eLf
	
	CPU 在执行的时候是以LWP为单位执行的

	只想查看某个进程的线程信息
	ps -Lw 3257 查看某一个进程的线程信息
	
	内核是以进程为单位分配资源， 操作系统在调度的时候则以LWP，也就是是以线程为单位调度的

8.1 线程间共享资源有哪些
	1.文件描述符表
	2.每种信号的处理方式(sigaction 信号处理函数， 不共享信号屏蔽字， 每个线程内都有独立的信号屏蔽字， 未决信号集也共享)
	3.当前工作目录(可通过chdir来改)
	4.用户ID和组ID
	5.内存地址空间(0-3G 地址空间， 一个LWP通过malloc申请一块地址，另一个同进程的LWP通过返回的地址也可以访问到这块内存)
	
	代码段
	数据段 已初始化的全局变量以及已初始化的静态变量
	BSS段 未初始化的全局变量以及未初始化的静态变量， 其地址为0
	堆	
	共享库	(同一个共享库在物理内存中只有一份，其余的进程可通过mmap映射到内存中)
	
	不共享的区域
	每个线程自己有自己独立的栈， 注意不是内核栈(保存处理器现场)，而是用户空间的栈(变量存储，开辟存储空间)
	线程ID( 不同于LWP，只在进程内有效，出了进程就无效了，主要是为了当前进程识别自己身份用的)
	errno 变量
	信号屏蔽字
	调度优先级
	
8.2 线程优缺点
	
	优点：
	提高程序并发性
	开销小，不用重新分配内存
	通信和共享数据方便
	
	缺点：
	线程不稳定(库函数实现)	
	线程调试比较困难(gdb支持不好)
	线程无法使用unix经典事件，例如信号
	
man -k pthread

8.3 线程原语
	
8.3.1 pthreaed_create
	

arg1: 保存线程ID，注意传递的是地址(传出参数) typedef unsigned long int pthread_t
arg2: 线程属性。创建一个线程，你希望它的用户空间的栈有多大，采用默认属性可以传NULL，还可以用来设置线程优先级
arg3: 函数指针， 创建一个新的线程， 让这个线程是是执行哪个函数,返回值void * 参数也是void*
arg4: 就是函数里面的arg,就是启用这个函数的时候，你想给里面传入什么arg
调用成功返回0, 调用失败返回错误码(当函数调用失败，再去设置errno全局变量容易搞混)

线程编程的时候需要包含pthread.h
线程是以库的形式提供的，所以需要加上 -lpthread llibpthread.so

8.3.2 pthread_self
pthread_self(void); 获取线程tid号

整个进程结束，进程的资源会被系统释放掉
主线程里面的tid是否等价于子线程调用pthread_self()

需要注意的是：
1.创建线程 2.线程号填写到tid, 函数调用返回
pthread_self 和 pthread_create中返回的tid 是不等价的，一旦线程调用函数先行执行完毕，而又来不及将线程号填写到tid，则返回的tid可能无效

err = pthread_create
if (err != 0) {
	fprintf(stderr, "can't create thread: %s\n", strerror(err))
	exit(1);
}

注意：
1.由于pthread_create的错误码不保存在errno中，因此不能直接用perror打印错误信息，可以先用strerror把错误码转换成错误信息再打印。
2.如果任意一个线程调用了exit或_exit,则整个进程的所有线程都终止，由于从main函数return 也相当于调用了exit,为了防止新创建的线程还没有得到执行就终止，我们在main函数return 之前延时1s,不过这只是一种权宜之计，即使主线程等待1s,内核也不一定会调度新创建的线程去执行。

_exit实际上是exit函数的一个更底层函数，是由linux提供的底层函数，该函数直接导致进程退出，关闭未关闭的文件描述符。
exit()关闭C标准文件流， 刷新缓冲区


8.3.3 pthread_exit
与上述的exit和_exit函数不同的是，该函数只会释放当前线程。另外，pthread_exit或者return 返回的指针所指向的内存单元必须是全局的或者是用malloc分配的，不能在线程函数的栈上分配， 因为当其它函数得到这个返回指针时线程函数已经退出了。如果什么都不需要返回，可以返回NULL

8.3.4 pthread_join
该函数等价于进程里面的wait,用来回收线程里面的退出值，如果线程没有退出，则阻塞 把这个函数结合android里面的AsyncTask来理解。

arg1:指定回收的线程ID
arg2:接收退出线程传递的返回值，因为退出线程的返回值是void *

该函数的作用主要在于：
阻塞，等待回收这个线程的资源，回收一个退出值，并且join函数在回收完这个线程后，会把这个线程的资源给回收掉。，join函数可以回收线程的PCB，若不回收，则该线程会变成僵尸线程
成功返回0, 失败返回errno的值，可通过strerror打印errno的信息

不过，如果一个线程是通过exit或pthread_exit退出的，pthread_join的第二个参数保存的就是线程退出的返回值
如果一个线程通过pthread_cancel终止的(类似于kill发信号)，线程的退出值是PTHREAD——CANCELD。
如果对退出线程的返回值不感兴趣，则传null


8.3.4 pthread_cancel
一个进程内的线程可以互相终止。类似于向某一个线程发信号。你要终止的线程ID
被取消的线程，退出值， 定义在linux的pthread库中常数pthread_canceld的值是-1，
#define PTHREAD_CANCELED ((void *) -1)



不同返回值介绍：保存退出值
正常返回
pthread_exit 等价于return 
pthread_cancel 取消

8.3.5 pthread_detach
用来设置线程为分离态 

引入背景：某些线程不关闭它的退出值，但是还需要调用pthread_join()去回收它，显得太过麻烦。
分离怸线程
置成分离态的线程，当它调用结束，系统会自动回收它的资源。不用主控线程再去回收。
pthread_detach 与 pthread_join是互斥的。不能同时调用
不能对一个处于detach状态的函数调用join, 这样的调用将返回EINVAL，


8.3.5 pthread_equal
比较两个函数是否相等，注意：如果两个线程ID相等，则返回一个非0的值。否则，返回0。


8.4 线程属性
修改线程默认占用的栈大小

线程同步
==============================================
九.线程同步出现的背景
9.1 线程同步的目的 
“同步”的目的，是为了避免数据混乱，解决与时间有关的错误。

9.2 互斥量 mutex
Linux提供了一把互斥锁mutex(也称为互斥量)
每个线程在对资源操作之前都尝试先加锁，成功加锁才能操作，操作结束解锁。
资源还是共享的，线程间还是竞争的
通过“锁”就将资源的访问变成互斥操作，而后与时间有关的错误也不会再产生了。
注意：同一时刻，只能有一个线程持有该锁

需要注意的是：

	互斥锁实质上是操作系统提供的一把{“建议锁”（又称“协同锁”）}，建议程序中有多线程访问共享资源的时候使用该机制。但，并没有强制限定。
	因此，即使有了mutex，如果有线程不按规则来访问数据，依然会造成数据混乱。

9.3 pthread_mutex_init()
pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr)

arg1: 定义的互斥量地址
arg2: 设置互斥量的属性，默认传NULL

9.4 pthread_mutex_destroy(pthread_mutex_t *mutex)
销毁一个互斥锁

9.5 restrict 关键字
该关键字起限制作用，只能用来限制“指针”，限定这个指针变量，所有对这个指针变量所指向的内存的修改和操作，必须由本指针完成。

9.6 静态初始化锁和动态初始化锁
动态：
pthread_mutex_t mutex;
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
静态：
在一个全局位置定义：
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


9.7 pthread_mutex_lock(pthread_mutex_t *mutex) & pthread_mutex_unlock()


9.8 pthread_mutex_lock()与pthread_mutex_trylock()的区别

9.9 lock与unlock
lock会尝试加锁， 如果加锁不成功，线程会阻塞， 阻塞到持有该互斥量的其它线程为止。
unlock()函数主动解锁，同时会[将阻塞在该锁上的所有线程全部唤醒]，至于哪个线程先被唤醒，取决于优先级，调度。默认：
先阻塞、先唤醒

pthread_mutex_t mutex;

int main()
{

	pthread_mutex_init(&mutex, NULL);
	
		
	pthread_mutex_destroy(&mutex);
	return 0;
}

重要：在访问共享资源前加锁，访问结束后应立即解锁。锁的“粒度”应越小越好。
切记下列写法是不对的。
while (1) {
	pthread_mutex_lock(&mutex);
	// TODO something...
	// TODO something...
	pthread_mutex_unlock(&mutex);
}

9.9 死锁出现的原因
9.9.1 线程试图对同一个互斥量A加锁两次
第一次加锁成功，第二次加锁就不可能成功，因为不可能成功，第二次加锁就会一直阻塞在那儿,因此导致死锁。
9.9.2 线程1拥有A锁，请求获得B锁；线程2拥有B锁，请求获得A锁。

9.10 读写锁（读共享，写独占，写的优先级高）

pthread_rwlock_t 类型，用于定义一个读写锁变量
pthread_rwlock_t rwlock;

其非常适合处理对数据结构读的次数远大于写的次数

读写锁只有一把锁，只不过可以对该锁设置两种状态
读写锁函数簇

pthread_rwlock_init()
pthread_rwlock_destroy()
pthread_rwlock_rdlock()
pthread_rwlock_wrlock()
pthread_rwlock_tryrdlock()
pthread_rwlock_trywrlock()
pthread_rwlock_unlock()

内核默认写操作的优先级要高于读的优先级，当一个线程进行了写的操作，另一个线程打算进行读的操作，则必须先等到写的线程先做完，再轮到到读的线程。

读写锁特性
1.1.读写锁是“写模式加锁”时， 解锁前，所有对该锁加锁的线程都会被阻塞。
1.2.读写锁是“读模式加锁”时， 如果线程以读模式对其加锁会成功；如果线程以写模式加锁会阻塞。
1.3.读写锁是“读模式加锁”时， 既有试图以写模式加锁的线程，也有试图以读模式加锁的线程。那么读写锁会阻塞随后的读模式锁请求。优先满足写模式锁。读锁、写锁并行阻塞，写锁优先级高

9.11 pthread_rwlock_unlock()
解锁的时候不区分当前锁到底是读锁还是写锁。
如果项目里既有读又有写的情况，使用读写锁则更高效

9.12 条件变量
条件变量本身不是锁，但是可以调用它的wait()函数造成一个阻塞的现象。通常与互斥锁配合使用。给多线程提供一个会合的场所。
条件变量函数簇
pthread_cond_init() // 初始化一个条件变量
pthread_cond_destroy() // 销毁一个条件变量
pthread_cond_wait()  // 阻塞等待一个条件的满足。1.阻塞等待条件变量满足；2.释放已掌握的互斥锁，相当于pthread_mutex_unlock(&mutex);
// 3. 当被唤醒的时候，该函数会返回，解除阻塞并重新申请获取互斥锁。
pthread_cond_timewait() // 
pthread_cond_signal() // 唤醒阻塞在这个条件上的函数
pthread_cond_broadcast() // 唤醒全部阻塞在条件变量上的函数

阻塞是阻塞等待一个条件变量
此时wait,不是wait在一个锁身上，而是wait在一个条件变量身上。等待条件是否满足

pthread_cond_wait() 进行等待，使用pthread_cond_signal和pthread_cond_broadcast进行唤醒
如果是signal，则唤醒阻塞在条件变量上的一个线程，如果是broadcast，则唤醒所有阻塞在该条件变量上的线程

条件变量类型
pthread_cond_t 
pthread_cond_t cond

9.13 条件变量——生产者，消费者模型


协议格式
===============================
一. UDP 数据报格式
源端口号(16位)+目的端口号(16位)+UDP长度(16位)+UDP校验和(16位)+ 数据(长度) [UDP首部]
例：
05 d4 00 45 00 3f ac 40
源端口： 0x05d4
目的端口：0x0045
UDP长度：0x003f,即：63个字节，此长度包含UDP首部(8个字节)+UDP层payload的长度
校验和：0xac 0x40

Tips: UDP协议不面向连接，也不保证传输的可靠性。发送端的UDP协议层只管把应用层传来的数据封装成段并给IP协议层就算完成任务了，
如果因为网络故障导致该段无法发送到对方，UDP协议层也不会向应用层返回任何错误信息。

同时：
接收端的UDP协议层只管把收到的数据根据端口号交给相应的应用程序就算完成任务
了,如果发送端发来多个数据包并且在网络上经过不同的路由,到达接收端时顺序已经错乱
了,UDP协议层也不保证按发送时的顺序交给应用层。

二. TCP数据报格式
源端口号(16位)+目的端口号(16位)+序号(32位)+确认序号(32位)+首部长度(4位)+保留长度(6位)+交互类型(URG/ACK/PSH/RST/SYN/PIN)[6位]+窗口大小(16位)
+校验和(16位)+紧急指针(16位)

2.1 TCP和UDP的最主要区别
发送端是一K一K地发送数据,而接收端的应用程序可以两K两
K地提走数据,当然也有可能一次提走3K或6K数据,或者一次只提走几个字节的数据,也就
是说,应用程序所看到的数据是一个整体,或说是一个流(stream),在底层通讯中这些数
据可能被拆成很多数据包来发送,但是一个数据包有多少字节对应用程序是不可见的,因此
TCP协议是面向流的协议。而UDP是面向消息的协议,每个UDP段都是一条消息,应用程序必
须以消息为单位提取数据,不能一次提取任意字节的数据,这一点和TCP是很不同的

三. Socket 编程

3.1 Socket API
int socket(int domain, int type, int protocol);
arg1: 	AF_INET: IPv4, AF_INET6, AF_UNIX
arg2:	

四.sockaddr数据结构
struct sockaddr 诞生早于IPv4协议
struct sockaddr_in IPv4协议
struct sockaddr_in6 IPv6协议

in_addr 32位的IP地址

五.C/S模型TCP
由于客户端不需要固定的端口号，因此不必调用bind(), 客户端的端口号由内核自动分配。

六.sockfd和 struct sockaddr_in
sockfd 代表一个文件描述符
sockaddr_in 代表描述一个IP地址和端口的结构体，两者没有必然关系



一.C++对C的扩展
=================================================
1.C++命名空间
	为解决合作开发时的命名冲突问题，C++引入了命名空间的概念：语法格式为：
	
	namespace name{
    //variables, functions, classes
	}
	在函数中声明命名空间std,那它的作用范围就位于main()函数内部，如果在其它函数中又用到了std,则需要重新声明。

2.输入输出
	cin用于从控制台获取用户输入,cout用于将数据输出到控制台
	string, cin, cout都位于命名空间std












































































































































	


	

	
	





	


	
	


	































































































































	














































































































			
			
			



	























































































