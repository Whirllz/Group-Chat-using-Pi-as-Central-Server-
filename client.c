#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#define MAX 100

int main()
{

int sockfd;
struct sockaddr_in servaddr;
char buff[MAX];


sockfd=socket(AF_INET,SOCK_STREAM,0);

bzero(&servaddr,sizeof(servaddr));


servaddr.sin_family=AF_INET;

if(inet_pton(AF_INET,"192.168.225.229",&servaddr.sin_addr)<=0)
printf("E_INET_PTON");

servaddr.sin_port=htons(5555);


if(connect(sockfd,(struct sockaddr *) &servaddr,sizeof(servaddr))<0)
     printf("Error_in_CONNECTING\n");

 bzero(buff,sizeof(buff));
 printf("\nEnter uname: ");
 fgets(buff,sizeof(buff),stdin);
 write(sockfd,buff,strlen(buff));
while(1)
{
	printf("Enter the data to be send: ");
	bzero(buff,sizeof(buff));
	fgets(buff,sizeof(buff),stdin);

	write(sockfd,buff,strlen(buff));
        
        if(strncmp(buff,"exit",4)==0)
        break;
        
	bzero(buff,sizeof(buff));

	read(sockfd,buff,sizeof(buff));

	printf("\n %s \n",buff);

}

close(sockfd);

return 0;
}






