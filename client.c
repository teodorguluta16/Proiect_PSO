#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define N 12 //dimensiune tip mesaj
#define M 60 // dimensiune mesaj

int sock;
void trimite_mesaj(char *msj,char *tip)
{
    char *m2=(char*)malloc(M * sizeof(char));
    char *t2=(char*)malloc(N * sizeof(char));
    strcpy(m2,msj);
    strcpy(t2,tip);
   
    if(strlen(t2)<N)
    {
      int i=strlen(t2);
      for(;i<N;i++)
      {
        strcat(t2,"+");
      }
      t2[strlen(t2)]='\0';
    }
    
    if(strlen(m2)<M)
    {
      int i;
      for(int i=strlen(m2);i<M;i++)
            strcat(m2,"+");
      m2[strlen(m2)]='\0';
      
    }
    send(sock, t2, strlen(t2), 0);//CreeareCont //Logare
    send(sock, m2, strlen(m2), 0);   

    free(m2);
    free(t2);
}
void primeste_mesaj(char **tip, char **mesaj) 
{
    *tip = (char*)malloc(N * sizeof(char));
    bzero(*tip, N);
    recv(sock, *tip, N, 0);

    printf("%ld\n", strlen(*tip)); 
    

    *mesaj = (char*)malloc(M * sizeof(char)); 
    bzero(*mesaj, M);
    recv(sock, *mesaj, M, 0); 
}
void conectare()
{
  char *ip = "127.0.0.1";
  int port = 5576;
  struct sockaddr_in addr;
  socklen_t addr_size;
  int n;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");

  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(ip);


  if(!connect(sock, (struct sockaddr*)&addr, sizeof(addr)))
  {
    printf("%d",port);
    printf("Connected to the server.\n");
  }
  else
  {
    printf("Eroare de conectare.\n");
    exit(0);
  }
    
}
int main()
{

  conectare();

  while(1)
  {
      char mesaj[M],tip[N];
      bzero(mesaj, M);
      strcpy(mesaj, "HELLO, THIS IS CLIENT.");
  
      bzero(tip, N);
      strcpy(tip, "CreareCont");
  
    
      trimite_mesaj(mesaj,tip);
      bzero(mesaj, M);

       char *msj2, *tip2;
      primeste_mesaj(&tip2, &msj2);

      printf("Server: %s\n", msj2);
      printf("Server: %s\n", tip2);
      break;
  }
 
  close(sock);
  printf("Disconnected from the server.\n");

  return 0;

}
