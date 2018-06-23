
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
	2.1 cin用于从控制台获取用户输入,cout用于将数据输出到控制台
	2.2 string, cin, cout都位于命名空间std
	2.3 在编写C++程序时，如果需要使用输入输出，则需要包含头文件<iostream>, 它包含了用于输入输出的对象，cin代表标准输入，cout代表标准输出，cerr代表
	标准错误。
	2.4 cin和cout都是C++内置对象，而不是关键字。C++库定义了定义了大量的类，可以使用它们来创建对象，cout和cin分别是ostream和istream类的对象。
	2.5 ">>" 是输入运算符, "<<" 是输出运算符
	★★★★★★★★★★★
	cout是ostream类的对象，cin是istream类的对象，C++标准库本身已经对左移运算符<<和右移运算符>>分别进行了重载
	使其能够用于不同数据的输入输出(包括：bool、int、double、string、complex、ofstream、ifstream)
	所以如果自己定义了一种新的数据类型，还想使用<<和>>输入输出，则必须重载<<或>>运算符。


3.变量定义位置和布尔类型
	3.1 C++中的变量可以声明在任意位置，如int i, 可以定义在任意位置。下列写法也被C++支持
		for (int i = 0; i < 5; i+)
		{
		}
	3.2 C++中引入bool类型. 语法：bool flag = true; bool flag = false; // true和false是C++中的关键字，占一个字节长度

4.C++中的const与C语言中的区别
	4.1 C++中的const 更类似于#define, 下列语句：
		const int n = 100;
		int *p = (int *)&n;
		*p = 80; 
		printf("n is: %d\n", n); // 在C语言中输出结果是：80;在C++中输出结果是100。因为在C中编译器会重新从n所指向的内存中取数据.C++则不会。
	4.2 C++中全局const变量的可见范围是当前文件
		在C语言中，不管全局变量有没有被const修饰，只要在其它文件中使用extern声明过了，就可以直接使用。
		在C++中，如果全局变量没有被const修饰，则它在其它文件中仍然可见。反之，如果一个全局变量已被const修饰，则它仅在当前文件可见,其它文件即使使用extern声明，仍无法调用。

5.C++中的new和delete关键字
	C++新增了new和delete关键字来动态分配内存， 在C++中【建议】使用new和delete来管理内存，因为它们可以自动调用构造函数和析构函数。例：
	/* 一个数据 */
	int *p = new int; // 分配1个int类型的内存空间，new操作符会根据后面的数据类型在推断所需空间的大小。
	delete p; // 释放内存
	/* 一组数据*/
	int *q = new int[10]; // 分配10个int类型空间
	delete[] q;

6.C++中的inline函数
	【引入背景】
	函数调用是有时间和空间开销的。程序在执行一个函数之前需要做一些准备工作，要将实参、局部变量、返回地址以及若干寄存器都压入栈中，
	然后才能执行函数体中的代码；函数体中的代码执行完毕后还要清理现场，将之前压入栈中的数据都出栈，才能接着执行函数调用位置以后的代码。

	如果函数体代码比较多，需要较长的执行时间，那么函数调用机制占用的时间可以忽略；
	如果函数只有一两条语句，那么大部分的时间都会花费在函数调用机制上，这种时间开销就就不容忽视。
	【解决方案】
	为消除函数调用开销，C++提供了一种提高效率的方法，即【在编译时将函数调用处函数体替换】，类似于C语言中的宏展开。这种在函数调用处直接
	嵌入函数体的函数称为【内联函数】，也叫内嵌函数或内置函数
	【inline 本质】
	【内联函数在编译时会将函数调用处用函数体替换，编译完成后函数就不存在了】，所以在链接时不会引发重复定义错误。
	这一点和宏很像，宏在预处理时被展开，编译时就不存在了。从这个角度讲，内联函数更像是编译期间的宏。
	【示例Demo】
	void swap(int *a, int *b)
	{
		int tmp;
		tmp = *a;
		*a = *b;
		*b = tmp;
	}
	int main()
	{
		int m, n;
		cin >> m >> n;
		cout << m << ", " << n << endl;
		swap(&m, &n); // 当执行到这个地方，会用实参替换形参，并将代码替换成swap函数体内的代码
		cout << m << ", " << n << endl;
		system("pause");
		return 0;
	}
	【使用须知】
	注意-1：必须要在【函数定义】处添加inline关键字，在函数声明处添加无效。可以将内联函数定义在main方法上面，省略函数声明。
	注意-2：将函数声明inline只是对编译器的一种建议，非强制性，编译器会依据实际情况自行决定是否采纳。
	注意-3：内联函数是可以定义在头文件中的，不用加statis关键字，且头文件被多次#include不会引发重复定义错误。
			非内联函数不允许定义在头文件中，它所在头文件被多次#include会引发重复定义错误。

7.C++的默认参数
	7.1 C++允许在定义函数时给形参指定一个默认的值，这样在调用这个函数时如果没有给给定的形参赋值，就会使用这个默认值。
		例：
		// 带默认参数的函数
		void func(int n, float b=1.2, char c='@'){
			cout<<n<<", "<<b<<", "<<c<<endl;
		}
		int main(){
			//为所有参数传值
			func(10, 3.5, '#');
			//为n、b传值，相当于调用func(20, 9.8, '@')
			func(20, 9.8);
			//只为n传值，相当于调用func(30, 1.2, '@')
			func(30);
		}
		// 也可以使用表达式
		float d = 10.8;
		void func(int n, float b=d+2.9, char c='@'){
			cout<<n<<", "<<b<<", "<<c<<endl;
		}
	7.2 如果函数在声明处指定了默认参数，在另一份文件的定义处又指定了不同的默认参数，那么编译器会优先选择在声明处的默认参数。
	
	7.3 C++对这种便携性的写法也有额外的要求，即：
		7.3.1【C++规定，默认参数只能放在形参列表的最后，而且一旦为某个形参指定了默认值，那么它后面的所有形参都必须有默认值】
			// 正确写法
			void func(int a, int b=10, int c=20){ }
			void func(int a, int b, int c=20){ }
			// 错误写法
			void func(int a, int b=10, int c=20, int d){ }
			void func(int a, int b=10, int c, int d=20){ }
		
		7.3.2 【C++规定，在给定的作用域只能指定一次默认参数。也就是说，在同一份文件中的同一个函数，不能既在声明处指定默认参数，又在定义处指定默认参数】
			void func(int a, int b = 10, int c = 36);
			int main(){
				func(99);
				return 0;
			}
			void func(int a, int b = 10, int c = 36){
				cout<<a<<", "<<b<<", "<<c<<endl;
			} // 无法编译通过
		7.3.3 【C++规定，可以多次声明同一函数，但在给定作用域中的一个形参只能被赋予一次默认参数。也就是说，
				函数的后续声明只能为之前那些没有默认值的形参添加默认值，而且该形参右侧的所有形参必须都有默认值】
			
			//多次声明同一个函数
			void func(int a, int b, int c = 36);
			void func(int a, int b = 5, int c);
			int main(){
				func(99);
				return 0;
			} // 可以正常编译通过，不过在VS2013上提示警告，无论如何，若一个参数有默认值，其后面的形参也必须有默认值

8. C++函数重载
	类似于Java


二.类和对象
=================================================
1. 类和对象
	1.1 类只是一个模板，【不占用内存空间】，所以在定义类时，不能对成员变量进行初始化。只有创建对象后才会给成员变量分配内存。
	1.2 C++中的类在定义结束后，需要加一个;号代表类定义结束。

	例：
	class Student {
	public:
		char *name;
		int age;
		float score;
		typedef int TNT; // 通过typedef定义的类型只能通过类来访问

		void say() {
		cout << name << ", age is " << age << ", score is " << score << endl;
	};
	
	int main()
	{
		Student stu;
		// Student *pStu = &stu;
		Student *pStu = new Student;
		pStu->name = "xp.chen";
		pStu->age = 15;
		pStu->score = 92.5f;
		pStu->say();
		delete pStu;

		Student::TNT n = 10; // 类似于static 变量
		system("pause");
		return 0;
	}
2. 类的成员函数最好在类体外声明
	2.1 可以在类体中直接定义成员函数，也可以在类外定义，在类外定义时，需要在函数名前加上类名予以限定。
	【成员函数必须在类体中作原型声明，然后在类外定义，也就是说类体的位置应在函数定义之前】
	例：
	class Student{
	public:
		//成员变量
		char *name;
		int age;
		float score;
		//成员函数
		void say();  //函数声明
	};
	//函数定义
	void Student::say(){
		cout<<name<<"的年龄是"<<age<<"，成绩是"<<score<<endl;
	}

	2.2 在类体和类外定义的成员函数是有区别的：在类体中定义的成员函数会自动成为【内联函数】，在类体外则不会。内联函数一般不是期望的，它会将
		函数调用处用函数体替代。因此【建议在类体内部对函数作声明，类体外部进行定义】。

3.	C++类成员的访问权限
	3.1 C++中的public, private, protected只能修饰类的成员，不能修饰类，与java/C#不同，C++中的类没有公共私有之分。
	3.2 C++中成员变量大都是m_开头，这是一种编写习惯。
	3.3 如果成员变量既不声明为private，又不声明为public, 则默认为private,这与java又不同。
	3.4 C++中对象的大小只受成员变量影响，与成员函数没有关系。

4.	C++对象的内存模型
	4.1 类是对象的模板，不占用内存空间。编译器会将成员变量和成员函数分开存储：分别为每个对象的成员变量分配内存，但是所有对象都共享同一段函数代码。
	4.2 C++与C语言的编译方式不同。C语言的函数在编译时名字不变，或者只是简单的加一个下划线"_", C++中的函数在编译时会根据它所在的命名空间，所属的类，
	以及它的参数列表等信息全新命名，形成一个新的函数名。
	4.3 C++中的成员函数最终还是会被编译成与对象无关的全局函数。

5.	C++中成员函数的调用【重点】
	成员函数最终被编译成与对象无关的全局函数，如果函数体中没有成员变量，那问题就很简单，不用对函数做任何处理，直接调用即可。
	如果成员函数中使用到了成员变量该怎么办呢？成员变量的作用域不是全局，不经任何处理就无法在函数内部访问。
	【C++规定，编译成员函数时要额外添加一个参数，把当前对象的指针传递进去，通过指针来访问成员变量】
	例：
	假设 Demo 类有两个 int 型的成员变量，分别是 a 和 b，并且在成员函数 display() 中使用到了，如下所示：
	void Demo::display(){
		cout<<a<<endl;
		cout<<b<<endl;
	}
	编译后代码：
	void new_function_name(Demo * const p){
		//通过指针p来访问a、b
		cout<<p->a<<endl;
		cout<<p->b<<endl;
	}

6. 构造函数
	6.1 C++中的写法
		class Student{
		private:
			char *m_name;
			int m_age;
			float m_score;
		public:
			//声明构造函数【注意！！！：C++中的构造函数必须是public的，否则创建对象时无法调用,不能有返回值，也不能有return语句】
			Student(char *name, int age, float score);
			//声明普通成员函数
			void show();
		};
		//定义构造函数
		Student::Student(char *name, int age, float score){
			m_name = name;
			m_age = age;
			m_score = score;
		}
		//定义普通成员函数
		void Student::show(){
			cout<<m_name<<"的年龄是"<<m_age<<"，成绩是"<<m_score<<endl;
		}
		int main(){
			//创建对象时向构造函数传参
			Student stu("小明", 15, 92.5f);
			stu.show();
			//创建对象时向构造函数传参
			Student *pstu = new Student("李华", 16, 96);
			pstu -> show();
			return 0;
		}
	6.2 C++中的构造函数与java中的基本类似，当一个类没有写明构造函数时，编译器也会为这个类自动生成构造函数，
		不过C++中无参构造函数可以省略括号。如Student *pStu = new Student;

	6.3 可以使用【参数初始化表】来简写构造函数，注意！仅限于构造函数使用。上面带参的Student构造函数可简写为：
		Student::Student(char *name, int age, float score): m_name(name), m_age(age), m_score(score){
			// 注意：！！！成员变量的赋值顺序仍然以类中声明的顺序为准，也就是说，即使m_age写在m_name前面，也优先赋值m_
		} 
		这种写法还可以初始化const 成员变量，且初始化const成员变量只能采用此方法：
		class VLA{
		private:
			const int m_len;
			int *m_arr;
		public:
			VLA(int len);
		};
		//必须使用参数初始化表来初始化 m_len
		VLA::VLA(int len): m_len(len){
			m_arr = new int[len];
		}
		// 绝对不可以写成下列样子, java中是可以的^_^
		VLA::VLA(int len){
			m_len = len;
			m_arr = new int[len];
		}
7.	析构函数
	7.1 C++中的写法。【注意：析构函数没有参数，不能被重载】
		VLA::~VLA(){
			delete[] m_arr;  //释放内存
		}
	7.2 C++中的new和delete分别用来分配和释放内存。用new分配内存时会自动调用构造函数，用delete删除时会自动调用析构函数。

8.	C++中的this关键字
	this 是一个const指针，实质是成员函数的一个形参，在调用成员函数时编译器会将对象的地址作为实参传递给this。

9.	C++中的static 成员变量和static 成员函数
	9.1	static 成员变量的用法和作用与java基本类似，不同的是C++中的静态成员变量必须在类声明的外部初始化。语法格式：
		type class::name = value; // int Student::m_total = 0;
		static 成员函数的用法与java相同也基本类似，都是需要在外部初始化。且只能访问静态成员变量。
		声明：
		static int m_total;  // static 放在开头
		定义：
		int Student::m_total = 0; // 成员变量定义不用加static
		声明：
		static int getTotal();
		定义：
		int Student::getTotal(){
			return m_total; // 成员函数定义不用加static
		}

10.	C++中的const成员函数和const成员变量
	10.1 与java 类似，const函数不能修改成员变量的值，const 函数的声明和定义分别为：
		声明：
		char *getName() const; // 注意：const放在最后
		定义：
		char * Student::getName() const
		{
			return m_name; // 注意函数签名与声明一致。
		}
	10.2 如果将一个对象声明为const, 则该对象只能访问const成员变量和const成员函数。

11. 友元函数与友元类【非重点，可跳过】
	11.1 ◆出现背景：
			由于有访问修饰符的限制，对象可以访问public成员，只有本类的函数可以访问本类的private成员。
		    【通过友元函数，可以使其它类的成员函数以及全局范围内的函数访问当前类的private成员】
		
		 ◆ 定义方式：
			 函数需要在类中声明，并添加friend关键字。在类外定义的时候不必带上类名及friend关键字。
			 class Student
			 {
				public:
					friend void show(Student *pStu);
			 };
			 void show(Student *pstu){
				cout<<pstu->m_name<<"的年龄是 "<<pstu->m_age<<"，成绩是 "<<pstu->m_score<<endl;
			 }
			 ★★★★★★ 注意！！！ ★★★★★★
		     友元函数毕竟不是类的成员函数，在友元函数中仍然不能直接访问类的成员，必须借助对象，所以下列写法是错误的：
		     成员函数在调用时因为会隐式的增加this指针，非成员函数则不会，因为需要通过参数传递对象
			 void show(){
				cout<<m_name<<"的年龄是 "<<m_age<<"，成绩是 "<<m_score<<endl;
			 }

12.	C++中class和struct的区别(C++中的struct既可以包含成员变量，也可以包含成员函数)

13. C++ string类
	13.1 使用string类需要包含头文件<string>。使用方法如：
			string s2 = "c plus plus"; // 与C风格的字符串不同，string结尾没有结束符'\0'
			string s4 (5, 's'); // 初始化由5个's'组成的字符串"sssss"
		
	13.2 可以调用string的c_str()方法，将C++的字符串类转换成C语言的const char *
			string path = "D:\\demo.txt";
			const char *c_str = path.c_str();
	
	13.3 访问字符串中的字符
			for(int i=0,len=s.length(); i<len; i++){
				cout<<s[i]<<" "; // 仍然可以使用[]来访问string中的每个字符
			}

	13.4 字符串拼接
		可以使用"+"号运算符进行字符串拼接，C++中字符串拼接仅限于下列类型
		string s1 = "first";
		string s2 = "second";
		char *s3 = "third";
		char s4[] = "fourth";
		char ch = '@';
		
		string s5 = s1 + s2;
		string s6 = s1 + s3;
		string s7 = s1 + s4;
		string s8 = s1 + ch;
	
		【与Java不同，C++中的字符串不能直接与int类型进行"+"号拼接】
		string a = "xp.chen";
		int b = 36;
		string c = a + b; // 这种写法是错误的！！！！


三.C++中的引用
=================================================
1. 引用的基本概念
	1.1 引用是C++对C语言的又一项扩充，可将其看做是数据的一个别名，类似于windows中的快捷方式，通过这个别名或者原来的名字都能找到这份数据。
   
	1.2 定义方式：
		type &name = data; // type是被引用数据的类型，name是引用的名称，data是被引用的数据

		例：
		int a = 99;
		int &b = a; // 有点类似于指针的定义，只是将*换成了&
		b = 47; // 也可以通过引用修改原始变量中所存储的值，若不希望修改原始数据，可以添加const限制：
				// const type &name = value; 或
				// type const &name = value;
	
	1.3 引用作为函数参数
		在定义和声明函数时，可以将函数的形参指定为引用的形式，这样在调用函数时就会将【实参和形参绑定在一起】，让它们都指向同一份数据。
		如此一来，若在函数体中修改了形参的数据，实参数据也会跟着被修改。
		----》例：swap(), 交换两个int变量的值
	
	1.4 引用作为函数返回值
		如：
		int &plus10(int &n){ // 定义方式有点类似于返回函数指针
			n = n + 10;
			return n;
		}
		/*
		int &plus10(int &n){
			int m = n + 10;
			return m;		//像这种返回局部数据的引用是错误的！！！,因为函数调用完之后,局部数据就会被销毁，有可能下次使用数据就不存在了。
		}*/

		int main(){
			int num1 = 10;
			int num2 = plus10(num1);
			cout<<num1<<" "<<num2<<endl;
		}

	1.5 注意要点
		1.5.1 引用必须在定义的同时初始化，并且之后不能再引用其它数据。引用在定义时需要加&，在使用时不需要加&，不然就变成了取地址。
		1.5.2 不能在函数中返回局部数据的引用。因为在函数调用后局部数据会被销毁，引用局部数据无意义。

2.	引用和指针的区别
	引用只是对指针做了简单的封装，其底层依然是通过指针实现的。引用占用的内存与指针占用的内存长度一样，32位4字节，64位8字节。
	【引用注意事项】
	2.1 引用必须在定义时初始化，且不能再指向其它数据；指针则无此限制，可不初始化，也可随意修改指向。

	2.2 不能有const 引用！因为引用本来就不能改变指向，加上const多此一举
		int a = 99;		
		const int &b = a; // 正确。表明不允许修改引用的值
		int const &b = a; // 正确。表明不允许修改引用的值
		int &const b = a; // 错误。引用本来就不能改变指向

	2.3 指针可以有多级，但引用只能有一级
		int ***p; // 合法
		int &&r;  // 不合法
	
	2.4 指针和引用的自增和自减意义不一样。指针使用++是指向下一份数据，引用表示指向的数据自身加+。
	2.5 当引用作为函数参数时，如果在函数内部不会修改引用所绑定的数据。尽量为该引用添加const限制。
		double volume(const double &len, const double &width, const double &hei){
			return len*width*2 + len*hei*2 + width*hei*2;
		}

四.C++中的继承
=================================================
1.	继承的基本写法
	class Student: public People, 
	public 代表继承方式为公有,还可以有private, protected, 若不写，默认为【private】,则子类的成员变量和成员
	函数默认也为private。 一般写成public
	C++中的继承就是冒号，不是java中的extends

2.	子类中的成员变量和成员函数会覆盖父类中的【同名成员变量和成员函数】,与java一致。
	注意：这不是重载！只要子类中有同名函数，就一定会覆盖父类的所有同名函数，不管参数是否一样。
	【在C++里面，重载仅限于同一个类的内层作用域里，父类和子类的同名函数不构成重载】

	如果执意要调用父类的同名函数，可以按照下列写法：
	Sub sub;
	sub.func("http://www.baidu.com");
	sub.func(true);
	// sub.func();  // compile error, 子类无法调用父类中的同名函数
	// sub.func(10);// compile error, 子类无法调用父类中的同名函数
	sub.Parent::func(); // 调用父类中同名函数的正确写法。
	sub.Parent::func(100);

3. 继承时构造函数的写法
	和java的写法类似，当调用子类的构造函数时，仍需要调用父类的构造函数。不同的是，C++里面只能通过参数初始化表
	形式实现，不能也没有super关键字。
	
	Student::Student(char *name, int age, float score) :People(name, age), m_score(score){}
	也可写成：
	Student::Student(char *name, int age, float score): m_score(score), People(name, age){ }
	不能像java一样写成
	Student::Student(char *name, int age, float score){
		People(name, age);
		m_score = score;
	}

4.	继承时不需要再调用析构函数
	4.1 当存在继承关系时，若子类未显式调用父类的构造函数，则子类默认会调用父类的无参构造函数
	4.2 由于析构函数只会有一个，所以子类不需要显式调用父类的析构函数，编译器会在子类析构函数调用完毕后，自行调用父类构造函数。

5. 多继承(不是重点)
	5.1 语法：
	class D: public A, private B, protected C{
    //类D新增加的成员
	}

	构造函数：
	D(形参列表): A(实参列表), B(实参列表), C(实参列表){
    //其他操作
	}
	5.2 多继承时的命名冲突问题
		如果两个或多个父类中存在相同的成员，若直接访问该成员，会产生命名冲突，编译器不知道使用哪个父类的成员，这时候需要在成员名字前
		加上类各和域解析符::
		class Derived: public BaseA, public BaseB{
		public:
			void display();
		};
		void Derived::display(){
			BaseA::show();  //调用BaseA类的show()函数
			BaseB::show();  //调用BaseB类的show()函数
			cout<<"m_e = "<<m_e<<endl;
		}

6. 虽然为成员变量设定了访问修饰符，但仍然能通过指针获得对象中成员变量的值
	class A{
	public:
		A(int a, int b, int c);
	private:
		int m_a;
		int m_b;
		int m_c;
	};

	A obj(10, 20, 30);
    int a1 = *(int*)&obj; // a1 = 10
    int b = *(int*)( (int)&obj + sizeof(int) ); // b = 20

7. 虚继承和虚基类
	7.1 虚继承用来解决的问题
	例：若B，C继承于A， D继承于B，C则构成了菱形继承，若在A中存在一个成员变量m_a,那么在D类的对象方法中访问m_a,就
		会发生错误，因为编译器不知道访问的是哪个m_a, B,C里面都继承了m_a, 为了解决多继承时的命名冲突问题，C++引入
		了【虚继承】，使得子类中只会保留一份间接父类的成员。其目的在于让某个类做出声明，承诺愿意共享它的基类。

		//间接基类A，由于它被承诺会被共享，也被称为【虚基类】
		class A{
		protected:
			int m_a;
		};
		//直接基类B
		class B: virtual public A{  //虚继承，【需要在继承方式前加上virtual 关键字】
		protected:
			int m_b;
		};
		//直接基类C
		class C: virtual public A{  //虚继承
		protected:
			int m_c;
		};
		//派生类D
		class D: public B, public C{
		public:
			void seta(int a){ m_a = a; }  //正确
			void setb(int b){ m_b = b; }  //正确
			void setc(int c){ m_c = c; }  //正确
			void setd(int d){ m_d = d; }  //正确
		private:
			int m_d;
		};

	7.2 虚继承时构造函数的写法
		以上面代码为例：
		若A是虚基类，D是最终的派生类，则D的构造函数应写成：
		D::D(int a, int b, int c, int d): A(a), B(90, b), C(100, c), m_d(d){}
		
		与普通继承时构造函数的写法不同，当有虚基类时，派生类的构造函数必须【手动调用虚基类的构造函数】。
		
		【略】：调用虚基类的构造函数有一定的必要性。若B，C都给公共的成员变量m_a赋值，编译器就不知道该采用哪个了，
				所以C++标准强制规定必须由最终的派生类D来初始化m_a,且只采纳D传入的值，调用顺序上先调用虚基类的构造
				函数，再按出现的顺序调用其它构造函数。

	7.3 将子类赋值给基类
		7.3.1 子类对象赋值给基类对象
				子类同名成员变量的值会替换父类同名成员变量的值，子类新增的成员变量会被丢弃。
		7.3.2 子类指针赋值给父类指针
				子类指针赋值给父类指针只会修改父类指针的指向
				对象的指针必须要指向对象的起始位置
		7.3.3 子类引用赋值给父类引用
				与指针类似


五.C++中的继承
=================================================
1.	虚函数和多态(C++里的虚函数是实现多态的必要手段)
	1.1 虚函数的引入背景
		例：
		class People{
		public:
			People(char *name, int age);
			void display();
		protected:
			char *m_name;
			int m_age;
		};
		People::People(char *name, int age): m_name(name), m_age(age){}
			void People::display(){
			cout<<m_name<<"今年"<<m_age<<"岁了，是个无业游民。"<<endl;
		}
		//派生类Teacher
		class Teacher: public People{
		public:
			Teacher(char *name, int age, int salary);
			void display();
		private:
			int m_salary;
		};
		Teacher::Teacher(char *name, int age, int salary): People(name, age), m_salary(salary){}
		void Teacher::display(){
			cout<<m_name<<"今年"<<m_age<<"岁了，是一名教师，每月有"<<m_salary<<"元的收入。"<<endl;
		}
		int main(){
			People *p = new People("王志刚", 23);
			p -> display();		// 会打印People的display()方法
			p = new Teacher("赵宏佳", 45, 8200);
			p -> display();		// 仍然会打印People的display()方法
			return 0;
		}

		上面例子说明，父类指针只能访问子类成员变量，却无法访问子类成员函数。为了解决此问题，使父类可以访问到
		子类成员函数，引入了【虚函数】的概念
	
	1.2 虚函数的写法
		只需要将父类的声明处改成：
		class People{
		public:
			People(char *name, int age);
			virtual void display(); // 在函数声明前加上virtual关键字，之后的函数实现可写可不写
		
		class Teacher: public People{
		public:
			Teacher(char *name, int age, int salary);
			virtual void display(); // 【注意！！！子类同名函数的virtual关键字其实可以省略。但函数签名必须与父类保持一致。】
		
	1.3 虚函数实现的功能
		加上virtual关键字之后，再次运行，此时父类指针终于可以调用子类的成员函数了。
		有了虚函数，指针指向哪个类的对象，就调用哪个类的虚函数。
		父类指针指向父类对象时就使用父类对象的成员(成员变量和成员函数)，指向子类对象时就使用子类对象的成员(
		成员函数和成员函数)，此时的父类指针就有了多种形态，这就是【多态】
		
		C++利用虚函数实现了多态。其提供虚函数的唯一目的就是为了实现多态。

2.	若用到了父类的析构函数，则最好把父类的析构函数声明为虚函数

	在实际开发过程中，若使用了父类指针指向子类的对象，且在接下来的操作中，在子类的析构函数中做了内存释放操作，那么
	必须将父类的析构函数声明为虚函数。否则，编译器将只会根据指针类型调用父类指针的析构函数，而不调用子类指针的析构
	函数，这样会有内存泄露的风险。

3.	纯虚函数和抽象类 (类似于java中的抽象函数和抽象类)
	3.1 纯虚函数的引入背景
		类似于java里面的抽象方法

	3.2 纯虚函数的写法
		virtual float area() = 0; // 纯虚函数没有函数体，只有函数声明，需要在虚函数结尾加上=0，表示此函数为纯虚函数，
								  // 和java不同，不是用abstract来修饰的，包含纯虚函数的类就是【抽象类】
	3.3 抽象类和纯虚函数的作用
		与java中用法一致，都没有函数声明，都是作为基类，让子类来实现纯虚函数。子类必须实现纯虚函数才能被初始化。

4.	C++利用虚函数表来实现多态
	【内部实现】
	在对象内部增加一个指针，该指针指向一个指针数组，该数组内部保存的是多个函数指针，每个函数指针指向该对象虚函数的入口地址。
			  
	【通过指针调用成员函数的具体步骤】
	4.1 首先判断该函数是否是虚函数。如果是非虚函数，则根据指针类型找到该函数，即：指针是哪个类的类型，就调用哪个类的函数；
		如果是虚函数，判断该指针指向的类是否含有同名函数，如果有，则调用指针指向的那个类的同名函数，否则则指向其父类的同
		名函数。
	4.2 当通过指针调用虚函数时，先根据指针找到 vtable(虚函数表) ，再根据 vtable  找到虚函数的入口地址。

5.	typeid运算符

	5.1 typeid的用法及作用
		这个东西有点类似于java里的instance of 运算符，可用来获得一个变量或表达式的数据类型。
		此运算符会把获取到的类型信息保存到一个type_info类型的对象里面，并返回该对象的引用。
		例：
		注：使用时需要包含#include <typeinfo>头文件
		
		//获取一个普通变量的类型信息
		int n = 100;
		const type_info &nInfo = typeid(n);
		cout<<nInfo.name()<<endl; // 输出int

		//获取一个对象的类型信息
		Base obj;
		const type_info &objInfo = typeid(obj)
		cout<<objInfo.name()<<endl; // 输出class Base

		//获取一个结构体的类型信息
		const type_info &stuInfo = typeid(struct STU);
		cout<<stuInfo.name()<<endl; // 输出struct STU
		
		// 可以用来做类型比较
		typeid(obj1) == typeid(obj2)
	
	5.2 typeid的底层实现
		之前提到，如果一个类中包含有虚函数，那么编译器在编译阶段会为这个类额外添加一个虚函数表，并在对
		象内存中插入一个指针，指向这个虚函数表，同时还会添加额外类型信息，也就是type_info对象，编译器会
		在这个虚函数表vttable的开头再插入一个指针，指向当前类对应的type_info对象。
		
		所以在程序运行阶段，如果我们想利用typeid获取类型信息时,编译器就能够通过现有的对象指针p找到虚函数
		表指针vfptr, 继而通过vfptr找到type_info指针，进而取得类型信息。类似于：
		**(p->vfptr-1)
		

6.	C++中的RTTI机制(运行时类型识别,参考java中的多态)
	基本概念[Runtime type identification,运行时类型识别]
		在程序运行后确定对象的类型信息的机制称为运行时类型识别（Run-Time Type Identification，RTTI）。
		在 C++ 中，只有类中包含了虚函数时才会启用 RTTI 机制，其他所有情况都可以在编译阶段确定类型信息。

六.运算符重载
=================================================
1.	基本概念和语法
	1.1 运算符重载主要目的
		使得程序书写更加人性化，易于阅读。运算符被重载后，其原有功能仍然		

	1.2 运算符重载的本质
		其本质就是定义一个函数，在函数体内部实现想要的功能，当用到该运算符时，编译器会自动调用这个函数。
		也就是说，运算符重载是通过函数实现的，它本质上是函数重载
	
	1.3 运算符重载语法格式
		返回值类型 operator 运算符名称 (形参表列){ // operator关键字专门用来定义重载运算符的函数
			//TODO:								   // 可将operator 运算符名称看做是函数名
		}										   // 运算符重载除了函数名是特定的格式，其它地方与普通函数一样
		
		例：
		complex complex::operator+(const complex &A) const{
			complex B;
			B.m_real = this->m_real + A.m_real;
			B.m_imag = this->m_imag + A.m_imag;
			return B;
		}

	1.4 运算符重载注意事项	
		1.4.1 当运算符重载函数作为类的成员函数时，其参数只能有一个。(以下均特指二元操作符)
		1.4.2 当运算符重载函数作为全局函数时，其参数必须有两个。
		1.4.3 当运算符重载函数是全局函数，那么两个参数要求最少有一个是对象，绝不能两个参数都是C++内置的数据类型！
			  如果两个都是基本数据类型，就会出现下面这种情况：
			  int operator + (int a,int b){				// 鬼知道表达式4+3的值到底是7还是1
				return (a-b);
			  }
			  要求其中最少有一个是对象的目的在于：让编译器能够区分这是程序员自定义的运算符

2.	C++重载[](下标运算符)
	C++规定，下标运算符[]必须以成员函数形式进行重载，该重载函数在类中的声明格式如下：
	返回值类型 & operator[ ] (参数);
	或者：
	const 返回值类型 & operator[ ] (参数) const;


七.C++函数模板
=================================================
1.	函数模板
	有点类似于java里面的泛型
	写法：
	template <typename 类型参数1 , typename 类型参数2 , ...> 返回值类型  函数名(形参列表){
    //在函数体中可以使用类型参数
	}

	例如：
	template<typename T> void Swap(T &a, T &b){ // 这里的typename关键字可以使用class替换，二者意义一样
		T temp = a;
		a = b;
		b = temp;
	}
	注意点：
	1.1 函数模板可以被重载。
	1.2 当函数形参是引用类型时，数组不会转换为指针。

2.	类模板
	基本写法：
	★-------------》声明
	template<typename 类型参数1 , typename 类型参数2 , …> class 类名{
    //TODO:
	};

	例如：
	template<typename T1, typename T2>  //这里不能有分号
	class Point{
	public:
		Point(T1 x, T2 y): m_x(x), m_y(y){ }
	public:
		T1 getX() const;  //获取x坐标
		void setX(T1 x);  //设置x坐标
		T2 getY() const;  //获取y坐标
		void setY(T2 y);  //设置y坐标
	private:
		T1 m_x;  //x坐标
		T2 m_y;  //y坐标
	};
	
	★-------------》实现
	template<typename 类型参数1 , typename 类型参数2 , …>
		返回值类型 类名<类型参数1 , 类型参数2, ...>::函数名(形参列表){
		//TODO:
	}
	例如：
	template<typename T1, typename T2>  //模板头
	T1 Point<T1, T2>::getX() const /*函数头*/ {
		return m_x;
	}
	有两点要注意!!!: 一是要加上模板头，二是函数声明不能像之前写成Point::getX(),而是Point<T1,T2>::getX()

	在使用时，还是要像java一样，带上尖括号
	Point<int, int> p1(10, 20);
	Point<int, char*> p2(10, "东经180度");
	Point<char*, char*> *p3 = new Point<char*, char*>("东经180度", "北纬210度");

3. 让模板能够根据不同的类型使用指定的算法(模板的显式具体化)
	
	3.1 该技术的现实意义
		模板属于一种泛型的技术，但是模板函数或模板类却无法匹配到所有的情况：如：
		template<class T> const T& Max(const T& a, const T& b){
			return a > b ? a : b;
		}
		a和b若都是基本数据类型，如int，float，这个模板函数就没有问题，如果是结构体类型或指针呢？
		这样的比较就毫无意义。
		因此特殊情况仍须特殊对待，模板显示具体化就像是对现有模板定义的补充。

	3.2	模板函数的显式具体化
		//函数模板
		template<class T> const T& Max(const T& a, const T& b);
		//函数模板的显示具体化（针对STU类型的显示具体化）
		template<> const STU& Max(const STU& a, const STU& b);

		template<class T> const T& Max(const T& a, const T& b){
			return a > b ? a : b;
		}
		template<> const STU& Max<STU>(const STU& a, const STU& b){
			return a.score > b.score ? a : b;
		}

		★模板函数的具体化相对简单，只需要将template<class T>和Max<STU>中尖括号里的内容去掉即可。

	3.3 模板类的显式具体化
		// 类模板
		template<class T1, class T2> class Point{
		public:
			Point(T1 x, T2 y): m_x(x), m_y(y){ }
		public:
			T1 getX() const{ return m_x; }
			void setX(T1 x){ m_x = x; }
			T2 getY() const{ return m_y; }
			void setY(T2 y){ m_y = y; }
			void display() const;
		private:
			T1 m_x;
			T2 m_y;
		};
		
		template<class T1, class T2>  //这里要带上模板头
		void Point<T1, T2>::display() const{
			cout<<"x="<<m_x<<", y="<<m_y<<endl;
		}

		//类模板的显示具体化（针对字符串类型的显示具体化）
		template<> class Point<char*, char*>{
		public:
			Point(char *x, char *y): m_x(x), m_y(y){ }
		public:
			char *getX() const{ return m_x; }
			void setX(char *x){ m_x = x; }
			char *getY() const{ return m_y; }
			void setY(char *y){ m_y = y; }
			void display() const;
		private:
			char *m_x;  //x坐标
			char *m_y;  //y坐标
		};

		//这里不能带模板头template<>
		void Point<char*, char*>::display() const{
			cout<<"x="<<m_x<<" | y="<<m_y<<endl;
		}
		
		★模板函数的具体化要注意3点：
			◇	template<class T1, class T2> 要变成template<>,尖括号内的内容要去掉；
			◇	类名后面要加上具体的类型名称，像class Point变成了class Point<char*, char*>；
			◇	成员函数的实现不能再有template<class T1, class T2>这种东西了，
				要说明具体的类型：void Point<char*, char*>::display(){}.

	3.4 模板类还可以进行部分显式具体化(只适用于模板类,不适用于模板函数)
		写法如下：
		template<typename T2> class Point<char*, T2>{
		public:
			Point(char *x, T2 y): m_x(x), m_y(y){ }
		public:
			char *getX() const{ return m_x; }
			void setX(char *x){ m_x = x; }
			T2 getY() const{ return m_y; }
			void setY(T2 y){ m_y = y; }
			void display() const;
		private:
			char *m_x;  //x坐标
			T2 m_y;  //y坐标
		};

		template<typename T2>  //这里需要带上模板头
		void Point<char*, T2>::display() const{
			cout<<"x="<<m_x<<" | y="<<m_y<<endl;
		}
		
		可以看到，这种情况既需要加上模板头，也需要在类名后加上尖括号，如：
		template<typename T2>
		void Point<char*, T2>::display() const{
			cout<<"x="<<m_x<<" | y="<<m_y<<endl;
		}






	
	

























	







		






	




































	


	




















			




			

		



		



		









	























	









		





		











	

	






















































































































































	


	

	
	





	


	
	


	































































































































	














































































































			
			
			



	























































































