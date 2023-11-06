#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define N 12 //dim tip mesaj
#define M 60 //dim mesaj


int client_sock;
socklen_t addr_size;
int server_sock, client_sock;
struct sockaddr_in server_addr, client_addr;
int n;
void makeServer()
{
   char *ip = "127.0.0.1";
  int port = 5576;


  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("[-]Bind error");
    exit(1);
  }
  printf("[+]Bind to the port number: %d\n", port);

  listen(server_sock, 5);
  printf("Listening...\n");
}
void coenctare_client()
{
   addr_size = sizeof(client_addr);
  client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);

  if(client_sock)
    printf("[+]Client connectat.\n");
  else
    printf("[+]Eroare de conectare client.\n");
}
void primeste_mesaj(char **tip, char **mesaj) 
{
    *tip = (char*)malloc(N * sizeof(char));
    bzero(*tip, N);
    recv(client_sock, *tip, N, 0); 

    *mesaj = (char*)malloc(M * sizeof(char)); 
    bzero(*mesaj, M);
    recv(client_sock, *mesaj, M, 0);  
}
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
  
    send(client_sock, t2, N, 0);//CreeareCont //Logare
    send(client_sock, m2, M, 0);   

    free(m2);
    free(t2);  
    
    
}
int main()
{

  makeServer();
  coenctare_client();
  
  while(1)
  {
    
    char *mesaj, *tip;
    primeste_mesaj(&tip, &mesaj);
    printf("Client: %s\n", tip); 
    printf("Client: %s\n", mesaj);
    
    
    char msj2[M],tip2[N];
    bzero(msj2, M);
    strcpy(msj2, "Ana are mere");
  
    bzero(tip2, N);
    strcpy(tip2, "Raspuns");

   

    trimite_mesaj(msj2,tip2);
    break;

  }
  close(client_sock);
  return 0;
}




 // Afis Intefata principala
          //log in
          //Creeare cont 

    //CREEARE CONT
    //char username[30],parola[30];
    //adauga_util_BD(username,parola);
    //creeaza_fis_BD(username)
    //intoarce la LOG IN


    //LOG IN
    //char username[30],parola[30];
    //verifica_Util(username,parola);
    //Afis meniu principal:
        // 1 creeaza una noua
        // 2 modifica baza
        // 3 sterge baza

    //Op 1: 
        //char numeBD[20];
        //numeBD=citeste_numeBD();
        //actualizeaza util_fis_BD(numeBD,"creeaza"); //fiecare util are un fis cu numele bazelor sale de date
    //Op 2:
        //char numeBD[20];
        //numeBD=citeste_numeBD();
        //Alege ce operatii vrei sa faci: ...
    //Op 3: 
        //char numeBD[20];
        //numeBD=citeste_numeBD();
        //actualizeaza util_fis_BD(numeBD,"sterge"); //fiecare util are un fis cu numele bazelor sale de date
    
    //close(client_sock);
    //printf("[+]Client disconnected.\n\n");


