#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <strings.h>
#include <errno.h>

/*
Action on SIGCHLD to manage Child termination
*/
void received_sigchld_on_tcpserver(int sig) {
	int status,pid;
	//wait for one child process ends
	pid=wait(&status);
	printf("wait pid=%d status=%d\n",pid,status);
}

void listen_sigchld() {
	struct sigaction eventSigChld;
	//Create the sigaction structure to handle SIGCHLD signal
    sigemptyset(&eventSigChld.sa_mask);
    eventSigChld.sa_flags=0;
    eventSigChld.sa_handler= received_sigchld_on_tcpserver;
	sigaction(SIGCHLD,&eventSigChld, NULL);
}

int listen_request(unsigned int port, unsigned int pool_size, connection_handler handler) {
	int mainfd,clientfd,n;
	struct sockaddr_in server,client;
	socklen_t clientsize;
	pid_t     childpid,serverpid;
	char buffer[100];
	int go;

	mainfd=socket(AF_INET,SOCK_STREAM,0);

	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	server.sin_port=htons(port);
	bind(mainfd,(struct sockaddr *)&server,sizeof(server));

	listen(mainfd,pool_size);
	listen_sigchld();
	go=1;
	childpid=-1;
	serverpid=getpid();
	while (serverpid==getpid()) {
		bzero(&client,sizeof(client));
		clientsize=sizeof(client);
		clientfd = accept(mainfd,(struct sockaddr *)&client,&clientsize);
		printf("%d CHILDPID : %d clientfd=%d mainfd=%d errno=%d\n",getpid(),childpid, clientfd, mainfd, errno);
		//When we receive SIGCHLD the access is unlock and return -1, ernno=4 (EINTR)
		if (clientfd>0) {
			if ((childpid = fork()) == 0) {
				close (mainfd);
				go=1;
				while (go) {
					n = recv(clientfd,buffer,sizeof(buffer),0);
					printf("recv %d\n",n);
					if (n>0) {
						buffer[n]=0;
						send(clientfd,buffer,n,0);
					} else {
						go=0;
					}
				}
			}
            printf("%d close clientfd %d\n", getpid(), clientfd);
            close(clientfd);
		}
	}
}
