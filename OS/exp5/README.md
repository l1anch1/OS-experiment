5、消息的创建、发送和接收
主函数中使用 fork() 系统调用创建两个子进程 sender 和 receiver。
sender 负责接收用户输入的一个字符串，并构造成一条消息，传送给 receiver。Receiver 在接收到消息后在屏幕上显示出来。
此过程一直持续直到用户输入 exit 为止。
在程序设计过程中使用 msgget()、msgsnd()、msgrcv()、msgctl()。
