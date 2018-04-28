#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>

#include<arpa/inet.h>

#define MAX 25

void echo(int a);

int main()
{
struct sockaddr_in serv,cli;

int len,confd,sockfd;

pid_t child;

sockfd=socket(AF_INET,SOCK_STREAM,0);

if(sockfd<0)
printf("E_SOCKET");
else
    printf("SOCKET_BUILD\n");

bzero(&serv,sizeof(serv));

serv.sin_family=AF_INET;
serv.sin_port=htons(5555);
serv.sin_addr.s_addr=htonl(INADDR_ANY);

if(bind(sockfd,(struct sockaddr*)&serv,sizeof(serv)) < 0 )
 printf("E_BIND");
else   printf("BIND");

listen(sockfd,5);

len=sizeof(cli);

while(1)
{

  confd=accept(sockfd,(struct sockaddr*)&cli,&len);

  if(confd < 0)
  printf("E_ACCEPT");
  else{ 
   printf("ACCEPTED");

  if((child=fork())==0)
  {
    close(sockfd);
    echo(confd);
    exit(0);
   }
  else
    close(confd);
 }
}

return 0;
}


void echo(int sockfd)
{

 FILE *f;
  
 int n,i,flag;
 char buff[MAX],uname[MAX],rbuff[50];

 bzero(buff,sizeof(buff));
 
 read(sockfd,buff,sizeof(buff));
 
 strcpy(uname,buff);

 

while(1)
{
 flag=0;
 bzero(buff,sizeof(buff));

 n=read(sockfd,buff,sizeof(buff));
 
 if(n>0)
  printf("\n CLIENT: %s <end>",buff);
 else if(n==0)
  {
    printf("read is returning 0\n");
  }
  else
{
  printf("read failed\n");
 break;
}
  if(strncmp(buff,"exit",4)==0)
 { printf("\n Client wants to exit\n");
   break;
  }
  else
  {
    f=fopen("file.txt","a+");
    fseek(f,0,SEEK_SET);
   printf("Writing back to client : %s\n",buff);
    
   if(fgetc(f)!=EOF)
   {    printf(">0------");
        bzero(rbuff,sizeof(rbuff));

     for(i=3;i<50;i++)
      { fseek(f,-i,SEEK_END);
         printf("in for loop");
       if(fgetc(f)=='\n')
       { fseek(f,1,SEEK_CUR);
         bzero(rbuff,sizeof(rbuff));
        fgets(rbuff,strlen(uname),f);
        if(strcmp(rbuff,uname)!=0)
         {  
            bzero(rbuff,sizeof(rbuff));
            fseek(f,-strlen(uname),SEEK_CUR);
            fgets(rbuff,sizeof(rbuff),f);
            printf("RBUFF====%s",rbuff); 
            
             write(sockfd,rbuff,strlen(rbuff));
             flag=1;    
            break;
      } 
     }  } 
   }
  else{printf("\nN=%d\n",n);}


   fwrite("\n",strlen("\n"),1,f); 
   fwrite(uname,strlen(uname)-1,1,f);
   fwrite(":",strlen(":"),1,f);
   fwrite(buff,strlen(buff),1,f);
   
   fclose(f);
   if(flag!=1)
   write(sockfd,"wait...",strlen("wait..."));
  }

} 
   
}
