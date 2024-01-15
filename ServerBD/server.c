#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

#define LENGTH_NAME 31
#define LENGTH_MSG 101
#define LENGTH_SEND 201

#define N 20 
#define M 30
#define L 1000 
#define LUNGTABELA 500
#define NRCLIENIT 10

typedef struct ClientNode 
{
    int data;
    struct ClientNode* prev;
    struct ClientNode* link;
    char ip[16];
    char name[31];
} ClientList;

ClientList *newNode(int sockfd, char* ip) 
{
    ClientList *np = (ClientList *)malloc( sizeof(ClientList) );
    np->data = sockfd;
    np->prev = NULL;
    np->link = NULL;
    strncpy(np->ip, ip, 16);
    strncpy(np->name, "NULL", 5);
    return np;
}
int portNumber = 8888;

int server_sockfd = 0, client_sockfd = 0;
ClientList *root, *now;
int s_addrlen;
int c_addrlen;
struct sockaddr_in server_info, client_info;

void catch_ctrl_c_and_exit(int sig) 
{
    ClientList *tmp;
    while (root != NULL) {
        printf("\nClose socketfd: %d\n", root->data);
        close(root->data); 
        tmp = root;
        root = root->link;
        free(tmp);
    }
    printf("Server oprit\n");
    exit(EXIT_SUCCESS);
}
typedef struct TipMesaj
{
    char mesaj[N]; //CreareCont, Logare,
}TipMesaj;

typedef struct Utilizator
{
    char nume_u[M];
    char parola_u[M];
}Utilizator;

typedef struct BazaDate
{
    char nume_bd[M];
    char nume_u[M];
} BazaDate;

typedef struct UT
{
    char nume_u[M];
} UT;

typedef struct Tabela
{
  char nume_u[M];
  char nume_bd[M];
  char nume_tab[M];
  char camp_INT[M][M];
  int nr_INT;
  char camp_VarChar[M][M];
  int nr_VarChar;
  char camp_date[M][M];
  int nr_Date;
}Tabela;

typedef struct Date ///informatiile din tabele
{
    char nume_u[M];
    char nume_bd[M];
    char nume_tab[M];
    char camp_INT[M][M];
    int nr_INT;
    char camp_VarChar[M][M];
    int nr_VarChar;
    char camp_date[M][M];
    int nr_Date;
}Date;
typedef struct SelectTabela
{
    char nume_u[M];
    char nume_bd[M];
    char nume_tab[M];
}SelectTabela;

typedef struct SelectDateTot
{
    int nr;
    char randulTabela[LUNGTABELA];
    char randulDate[LUNGTABELA][LUNGTABELA];
}SelectDateTot;
typedef struct SelectRandTab
{
    int x;
    char nume_u[M];
    char nume_bd[M];
    char nume_tab[M];
    
}SelectRandTab;
typedef struct ListaTabele
{
    int x;
    char lista[100][LUNGTABELA];
    
}ListaTabele;

void primeste_mesaj(int sock,TipMesaj *tipm) 
{
    strcpy(tipm->mesaj,"");
    recv(sock, tipm, sizeof(TipMesaj), 0);
}
void primeste_U(int sock,UT *u)
{
    recv(sock, u, sizeof(UT), 0);
}
void primeste_Utilizator(int sock,Utilizator *u)
{
    Utilizator ut;
    recv(sock, &ut, sizeof(Utilizator), 0);
    strcpy(u->nume_u,ut.nume_u);
    strcpy(u->parola_u,ut.parola_u);

}
void primeste_Baza(int sock,BazaDate *bd)
{
    BazaDate b;
    recv(sock, &b, sizeof(BazaDate), 0);
    strcpy(bd->nume_bd,b.nume_bd);
    strcpy(bd->nume_u,b.nume_u);
}
void primeste_Tabela(int sock,Tabela *tab)
{
    Tabela tb;
    recv(sock, &tb, sizeof(Tabela), 0);
    strcpy(tab->nume_u,tb.nume_u);
    strcpy(tab->nume_bd,tb.nume_bd);
    strcpy(tab->nume_tab,tb.nume_tab);
    tab->nr_INT=tb.nr_INT;
    tab->nr_VarChar=tb.nr_VarChar;
    tab->nr_Date=tb.nr_Date;

    for(int i=0;i<tab->nr_INT;i++)
        strcpy(tab->camp_INT[i],tb.camp_INT[i]);
    for(int i=0;i<tab->nr_VarChar;i++)
        strcpy(tab->camp_VarChar[i],tb.camp_VarChar[i]);
    for(int i=0;i<tab->nr_Date;i++)
        strcpy(tab->camp_date[i],tb.camp_date[i]);
    
}
void primeste_Date(int sock, Date *dt)
{
    Date dat;
    recv(sock, &dat, sizeof(Date), 0);
    strcpy(dt->nume_u,dat.nume_u);
    strcpy(dt->nume_bd,dat.nume_bd);
    strcpy(dt->nume_tab,dat.nume_tab);
    dt->nr_INT=dat.nr_INT;
    dt->nr_VarChar=dat.nr_VarChar;
    dt->nr_Date=dat.nr_Date;

    for(int i=0;i<dt->nr_INT;i++)
        strcpy(dt->camp_INT[i],dat.camp_INT[i]);
    for(int i=0;i<dt->nr_VarChar;i++)
        strcpy(dt->camp_VarChar[i],dat.camp_VarChar[i]);
    for(int i=0;i<dt->nr_Date;i++)
        strcpy(dt->camp_date[i],dat.camp_date[i]);
}
void primeste_SelectTabela(int sock, SelectTabela *stab)
{
    SelectTabela stb;
    recv(sock, &stb, sizeof(SelectTabela), 0);
    strcpy(stab->nume_u,stb.nume_u);
    strcpy(stab->nume_bd,stb.nume_bd);
    strcpy(stab->nume_tab,stb.nume_tab);
}
void primesteRandDeSters(int sock, SelectRandTab *srt)
{
    SelectRandTab s;
    recv(sock, &s, sizeof(SelectRandTab), 0);
    srt->x=s.x;
    strcpy(srt->nume_u,s.nume_u);
    strcpy(srt->nume_bd,s.nume_bd);
    strcpy(srt->nume_tab,s.nume_tab);
}
void primeste_DateTot(int sock,SelectDateTot *st)
{
    SelectDateTot stv;
    recv(sock,&stv,sizeof(SelectDateTot),0);
    st->nr=stv.nr;
    for(int i=0;i<stv.nr;i++)
        strcpy(st->randulDate[i],stv.randulDate[i]);
    
    strcpy(st->randulTabela,stv.randulTabela);

}
void trimite_mesaj(int soc,TipMesaj tp) 
{
    printf("%s\n",tp.mesaj);
    send(soc, &tp, sizeof(TipMesaj), 0);
}
void trimite_Date(SelectDateTot std,int soc)
{
    send(soc, &std, sizeof(SelectDateTot), 0);
}
void trimite_Tabela(Tabela tab,int soc)
{
    send(soc, &tab, sizeof(Tabela), 0);
}
int trimite_Lista_Tabele(int soc,ListaTabele lis)
{
    send(soc, &lis, sizeof(ListaTabele), 0);
}
int adauga_Utilizatori(Utilizator ut)    
{
    int fid = open("Utilizatori.txt", O_RDONLY);
    if (fid == -1) {
        printf("eroare open");
        return -1;
    }
    ssize_t bytes_read;
    char line[70];
    int cnt=0;
    char ch;
    while ((bytes_read=read(fid,&ch,1))>0)
    {
        if(ch=='\n')
        {
            line[cnt]='\0';
            cnt=0;
            char copy[70];
            strcpy(copy,line);
            char *nume=strtok(copy," ");
            char *parola=strtok(NULL," ");
            if(strcmp(nume,ut.nume_u)==0)
                return 0;
            strcpy(line,"");
        }
        else
            line[cnt++]=ch;
    }
    //creare director_u
    char nume_dir[40]="Dir_";
    strcat(nume_dir,ut.nume_u);
   if (mkdir(nume_dir, 0777) == 0) {
        printf("Director creat cu succes.\n");
    } else {
        printf("Eroare creare director");
    }
    // adaugare in baza de date  
    close(fid);
    char sir[70]="";
    strcpy(sir,ut.nume_u);
    strcat(sir," ");
    strcat(sir,ut.parola_u);
    strcat(sir,"\n\0");
    fid = open("Utilizatori.txt", O_WRONLY|O_APPEND);
    write(fid,sir,strlen(sir));
    close(fid);
    return 1;
}
int verifica_Utilizator(Utilizator ut)
{
    int fid = open("Utilizatori.txt", O_RDONLY);
    if (fid == -1) {
        printf("eroare open");
        return -1;
    }

    ssize_t bytes_read;
    char line[70];
    int cnt=0;
    char ch;
    while ((bytes_read=read(fid,&ch,1))>0)
    {
        if(ch=='\n')
        {
            line[cnt]='\0';
            cnt=0;
            char copy[70];
            strcpy(copy,line);
            char *nume=strtok(copy," ");
            char *parola=strtok(NULL," ");
            if(strcmp(nume,ut.nume_u)==0&&strcmp(parola,ut.parola_u)==0)
                return 1;
            strcpy(copy,line);
            strcpy(line,"");
        }
        else
            line[cnt++]=ch;
    }
    close(fid);
    return 0;
}
int creeazaBd(BazaDate bd)
{
    DIR  *dir;
    struct dirent *entry;

    char sir[50]="Dir_";
    strcat(sir,bd.nume_u);

    dir=opendir(sir);

    if (dir == NULL) 
    {
        printf("Eroare deschidere director\n");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) 
    {
        // Skip special entries "." and ".."
        if (entry->d_name[0] != '.') 
        {
            if(strcmp(entry->d_name,bd.nume_bd)==0)
                return 0;
        }
    }

     //creare director_bd
    char nume_dir[70]="Dir_";
    strcat(nume_dir,bd.nume_u);
    strcat(nume_dir,"/");
    strcat(nume_dir,"Dir_BD_");
    strcat(nume_dir,bd.nume_bd);
    
    int ok=0;
    if (mkdir(nume_dir, 0777) == 0) 
    {
        printf("Director creat cu succes.\n");
    } 
    else
    {
        printf("Eroare creare director\n");
        return 3;
    }

    DIR *dir2;
    dir2=opendir(nume_dir);

    if (dir == NULL) 
    {
        printf("Eroare deschidere director\n");
        return 0;
    }
    //creaza fis bd in dir
    char fis[80]="";
    strcat(fis,nume_dir); 
    strcat(fis,"/");
    strcat(fis,"BD_");
    strcat(fis,bd.nume_bd);
    strcat(fis,".txt");
    printf("%s\n",fis);
    int fid=open(fis,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    close(fid);


    int dir_fd = dirfd(dir);
    close(dir_fd);
    return 1;
}
int creeazaTabela(Tabela tb)
{
    char sir[70]="Dir_";
    strcat(sir,tb.nume_u);
    strcat(sir,"/");
    strcat(sir,"Dir_BD_");
    strcat(sir,tb.nume_bd);
    strcat(sir,"/");
    strcat(sir,"BD_");
    strcat(sir,tb.nume_bd);
    strcat(sir,".txt");

    // verifica daca tabela exista deja
    //*******************************
    int fd=open(sir,O_RDONLY);
    if(fd==-1)
    {
        printf("Eroare deschidere fisier\n"); 
        return -1;/// daca baza de data exista
    }

    int bytesRead;
    char ch;
    char linie[LUNGTABELA];
    int cnt=0;
    int ok=0;
    while ((bytesRead=read(fd,&ch,1))>0)
    {
        if(ch=='\n')
        {
            linie[cnt]='\0';
            cnt=0;
            char copy[LUNGTABELA];
            strcpy(copy,linie);
            char *nume_tab=strtok(copy,"{"); 
            if(strcmp(nume_tab,tb.nume_tab)==0)
            {
               return 0; // tabela deja exista
            }
             strcpy(linie,"");
        }
        else
            linie[cnt++]=ch;
    }
    lseek(fd,0,SEEK_SET);
    close(fd);
    //*********************************

    //adauga tabela
    int fid=open(sir,O_WRONLY|O_APPEND);
   
    if(fid==-1)
    {
        printf("Eroare deschidere fisier\n");
        return -1;
    }
    char sir2[L]="";
    strcat(sir2,tb.nume_tab);
    strcat(sir2,"{");
    for(int i=0;i<tb.nr_INT;i++)
    {
        strcat(sir2,tb.camp_INT[i]);
        strcat(sir2," INT ");
    }
    for(int i=0;i<tb.nr_VarChar;i++)
    {
        strcat(sir2,tb.camp_VarChar[i]);
        strcat(sir2," VARCHAR ");
    }
    for(int i=0;i<tb.nr_Date;i++)
    {
        strcat(sir2,tb.camp_date[i]);
        strcat(sir2," DATE ");
    }
    strcat(sir2,"}\n");
    write(fid,sir2,strlen(sir2));
    close(fid);

    //creaza fis tabela in dir
    char fis[80]="Dir_";
    strcat(fis,tb.nume_u);
    strcat(fis,"/");
    strcat(fis,"Dir_BD_");
    strcat(fis,tb.nume_bd);
    strcat(fis,"/");
    strcat(fis,"Tabela_");
    strcat(fis,tb.nume_tab);
    strcat(fis,".txt");
   
    printf("%s",fis);
    int fidd=open(fis,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    close(fidd);
    return 1;
}
int adaugaInTabela(Date dt)
{
     char sir[70]="Dir_";
    strcat(sir,dt.nume_u);
    strcat(sir,"/");
    strcat(sir,"Dir_BD_");
    strcat(sir,dt.nume_bd);
    strcat(sir,"/");
    strcat(sir,"BD_");
    strcat(sir,dt.nume_bd);
    strcat(sir,".txt");

    int fid=open(sir,O_RDONLY);
   
    if(fid==-1)
    {
        printf("Eroare deschidere fisier.Baza nu exista\n");
        return 0;
    }

    int bytesRead;
    char ch;
    char linie[LUNGTABELA];
    int cnt=0;
    int ok=0;
    while ((bytesRead=read(fid,&ch,1))>0)
    {
        if(ch=='\n')
        {
            linie[cnt]='\0';
            cnt=0;
            char copy[LUNGTABELA];
            strcpy(copy,linie);
            char *nume_tab=strtok(copy,"{"); 
            if(strcmp(nume_tab,dt.nume_tab)==0)
            {
                ok=1;
                break;
            }
             strcpy(linie,"");
        }
        else
            linie[cnt++]=ch;
    }
    close(fid);
    if(ok==1)
    {
        char sir2[70]="Dir_";
        strcat(sir2,dt.nume_u);
        strcat(sir2,"/");
        strcat(sir2,"Dir_BD_");
        strcat(sir2,dt.nume_bd);
        strcat(sir2,"/");
        strcat(sir2,"Tabela_");
        strcat(sir2,dt.nume_tab);
        strcat(sir2,".txt");
        printf("%s\n",sir2);
        int fd=open(sir2,O_WRONLY|O_APPEND);
   
        if(fd==-1)
        {
            printf("Eroare deschidere fisier.Tabela nu exista\n");
            return 0;
        }
        char randul[LUNGTABELA]="";
        for(int i=0;i<dt.nr_INT;i++)
        {
            strcat(randul,dt.camp_INT[i]);
            strcat(randul,"  INT  ");
        }
        for(int i=0;i<dt.nr_VarChar;i++)
        {
            strcat(randul,dt.camp_VarChar[i]);
             strcat(randul,"  VARCHAR  ");
        }
        for(int i=0;i<dt.nr_Date;i++)
        {
            strcat(randul,dt.camp_date[i]);
             strcat(randul,"  DATE  ");
        }
        strcat(randul,"\n");
        write(fd,randul,strlen(randul));
        printf("%s\n",randul);
        close(fd);

    }
    return 1; 
}
int selectTabela(SelectTabela stb,int soc)
{
    char sir[70]="Dir_";
    strcat(sir,stb.nume_u);
    strcat(sir,"/");
    strcat(sir,"Dir_BD_");
    strcat(sir,stb.nume_bd);
    strcat(sir,"/");
    strcat(sir,"BD_");
    strcat(sir,stb.nume_bd);
    strcat(sir,".txt");

    int fid=open(sir,O_RDONLY);
    printf("%s\n",sir);
    if(fid==-1)
    {
        printf("Eroare deschidere fisier\n");
        return 0;
    }
    int bytesRead;
    char ch;
    char linie[LUNGTABELA];
    int cnt=0;
    int ok=0;
    while ((bytesRead=read(fid,&ch,1))>0)
    {
        if(ch=='\n')
        {
            linie[cnt]='\0';
            cnt=0;
            char copy[LUNGTABELA];
            strcpy(copy,linie);
            char *nume_tab=strtok(copy,"{"); 
            if(strcmp(nume_tab,stb.nume_tab)==0)
            {
                ok=1;
                break;
            }
             strcpy(linie,"");
        }
        else
            linie[cnt++]=ch;
    }
    close(fid);

    if(ok==1)
    {
       printf("Ok\n");
       SelectDateTot dat;
       strcpy(dat.randulTabela,"");
       strcpy(dat.randulTabela,linie);

       char copy[LUNGTABELA];
       strcpy(copy,linie);
       char *nume_tab=strtok(copy,"{"); 

       char sir2[70]="Dir_";
        strcat(sir2,stb.nume_u);
        strcat(sir2,"/");
        strcat(sir2,"Dir_BD_");
        strcat(sir2,stb.nume_bd);
        strcat(sir2,"/");
        strcat(sir2,"Tabela_");
        strcat(sir2,nume_tab);
        strcat(sir2,".txt");

        int fid2=open(sir2,O_RDONLY);
        printf("%s\n",sir2);
        if(fid2==-1)
        {
            printf("eroare deschidere fisier\n");
            return -1;
        }
        int bytesRead2;
        char ch2;
        char linie2[LUNGTABELA];
        int cnt2=0;
        int ok2=0;
        int index=0;
        while ((bytesRead2=read(fid2,&ch2,1))>0)
        {
            if(ch2=='\n')
            {
                linie2[cnt2]='\0';
                strcpy(dat.randulDate[index++],linie2);
                cnt2=0;
                char copy[LUNGTABELA];
                strcpy(linie,"");
            }
            else
                linie2[cnt2++]=ch2;
        }
        close(fid2);
        dat.nr=index;
        trimite_Date(dat,soc);
        return 1;
    }
    else
    {
        return 0;
    }
    return 1;
}
int stergeRandTabela(SelectRandTab st,int soc)
{
     char sir[70]="Dir_";
    strcat(sir,st.nume_u);
    strcat(sir,"/");
    strcat(sir,"Dir_BD_");
    strcat(sir,st.nume_bd);
    strcat(sir,"/");
    strcat(sir,"Tabela_");
    strcat(sir,st.nume_tab);
    strcat(sir,".txt");

    int fid=open(sir,O_RDONLY);
   
    if(fid==-1)
    {
        printf("eroare deschidere fisier\n");
        return -1;
    }
    int bytesRead;
    char ch;
    char linie[LUNGTABELA];
    char date[LUNGTABELA][LUNGTABELA];
    int cnt=0;
    int index=0;
    while ((bytesRead=read(fid,&ch,1))>0)
    {
        if(ch=='\n')
        {
            linie[cnt]='\0';
            strcpy(date[index++],linie);
            cnt=0;
        }
        else
            linie[cnt++]=ch;
    }
    for(int i=st.x-1;i<index-1;i++)
    {
        strcpy(date[i],date[i+1]);
    }
    index--;
    lseek(fid,0,SEEK_SET);
    close(fid);
     for(int i=0;i<index;i++)
            printf("%s\n",date[i]);
    

    int fd=open(sir,O_WRONLY|O_TRUNC);
    for(int i=0;i<index;i++)
    {
        strcat(date[i],"\n");
        write(fd,date[i],strlen(date[i]));
    }
    close(fd);
    return 1;
}
int extrageDateTabela(Tabela *tab,SelectTabela t)
{
    char sir[70]="Dir_";
    strcat(sir,t.nume_u);
    strcat(sir,"/");
    strcat(sir,"Dir_BD_");
    strcat(sir,t.nume_bd);
    strcat(sir,"/");
    strcat(sir,"BD_");
    strcat(sir,t.nume_bd);
    strcat(sir,".txt");

    int fid=open(sir,O_RDONLY);
   
    if(fid==-1)
    {
        printf("eroare deschidere fisier\n");
        return -1;
    }

    int bytesRead;
    char linie[LUNGTABELA];
    char date[LUNGTABELA][LUNGTABELA];
    int cnt=0;
    int index=0;
    char ch;
    int ok=0;
    while ((bytesRead=read(fid,&ch,1))>0)
    {
        if(ch=='\n')
        {
            linie[cnt]='\0';
            char *gasit=strstr(linie,t.nume_tab);
            if(gasit != NULL)
            {
                ok=1;
                break;
            }
            cnt=0;
        }
        else
            linie[cnt++]=ch;
    }
    if(ok==1)
    {
        printf("%s\n",linie);
        char copy[LUNGTABELA]="";
        strcpy(copy,linie);

        strcpy(tab->nume_u,t.nume_u);
        strcpy(tab->nume_bd,t.nume_bd);
        strcpy(tab->nume_tab,t.nume_tab);
        tab->nr_INT=0;
        tab->nr_Date=0;
        tab->nr_VarChar=0;

        char *str=strtok(copy,"{} ");
        char prev[M]="";
        char prev2[M]=""; 
        while(str!=NULL)
        {
            if(strcmp(str,"INT")==0)
            {
                 strcpy(tab->camp_INT[tab->nr_INT],prev);
                 tab->nr_INT++;
            }
               
            if(strcmp(str,"VARCHAR")==0)
            {
                strcpy(tab->camp_VarChar[tab->nr_VarChar],prev);
                tab->nr_VarChar++;
            }
                
            if(strcmp(str,"DATE")==0)
            {
                strcpy(tab->camp_date[tab->nr_Date],prev);
                tab->nr_Date++;
            }
            strcpy(prev,"\0");
            strcpy(prev,str);
            str=strtok(NULL,"{} ");
        }
        return 1;
    }
    else
    {
        printf("Tabela nu exista\n");
        return 0;
    }

}
int actualizaeazaTabela(SelectDateTot sdate,SelectTabela stab)
{
    
    char sir[70]="Dir_";
    strcat(sir,stab.nume_u);
    strcat(sir,"/");
    strcat(sir,"Dir_BD_");
    strcat(sir,stab.nume_bd);
    strcat(sir,"/");
    strcat(sir,"Tabela_");
    strcat(sir,stab.nume_tab);
    strcat(sir,".txt");

    int fid=open(sir,O_WRONLY|O_TRUNC);
   
    if(fid==-1)
    {
        printf("eroare deschidere fisier\n");
        return -1;
    }
    for(int i=0;i<sdate.nr;i++)
    {
        strcat(sdate.randulDate[i],"\n");
        write(fid,sdate.randulDate[i],strlen(sdate.randulDate[i]));
    }
    close(fid);
    printf("Date actualizate cu succes\n");
    return 1;

}
void cauta_tabele(BazaDate stab,ListaTabele *lis)
{
    ListaTabele lls;
    char sir[70]="Dir_";
    strcat(sir,stab.nume_u);
    strcat(sir,"/");
    strcat(sir,"Dir_BD_");
    strcat(sir,stab.nume_bd);

     DIR *dp;
    struct dirent *entry;

    dp = opendir(sir);

    if (dp == NULL)
    {
        perror("Eroare la deschiderea directorului");
        exit(EXIT_FAILURE);
    }
    int index=0;
    while ((entry = readdir(dp)) != NULL) 
    {    
        if (strncmp(entry->d_name, "Tabela_", 7) == 0) 
        {
            //printf("Fisier gasit: %s\n", entry->d_name);
            strcpy(lls.lista[index++],entry->d_name);
        }
    }
    lls.x=index;
    lis->x=index;
    for(int i=0;i<index;i++)
    {
        strcpy(lis->lista[i],lls.lista[i]);
    }

    closedir(dp);
}
int cauta_BD(UT u,ListaTabele *lis)
{
    ListaTabele lls;
    printf("%s\n",u.nume_u);
    char sir[70]="Dir_";
    strcat(sir,u.nume_u);

    DIR *dp;
    struct dirent *entry;

    dp = opendir(sir);

    if (dp == NULL)
    {
        perror("Eroare la deschiderea directorului");
        exit(EXIT_FAILURE);
    }
    int index=0;
    while ((entry = readdir(dp)) != NULL) 
    {    
        if (strncmp(entry->d_name, "Dir_BD_", 7) == 0) 
        {
            //printf("Fisier gasit: %s\n", entry->d_name);
            strcpy(lls.lista[index++],entry->d_name);
        }
    }
    lls.x=index;
    lis->x=index;
    for(int i=0;i<index;i++)
    {
        strcpy(lis->lista[i],lls.lista[i]);
    }

    closedir(dp);
}
void sterge_Baza(BazaDate stab)
{
    char sir[70]="Dir_";
    strcat(sir,stab.nume_u);
    strcat(sir,"/");
    strcat(sir,"Dir_BD_");
    strcat(sir,stab.nume_bd);

    if (rmdir(sir) == 0)
    {
        printf("Directorul a fost șters cu succes.\n");
    } 
    else 
    {
        perror("Eroare la ștergerea directorului");
    }
}
void sterge_Tabela(SelectTabela stab)
{
    ListaTabele lls;
    char sir[70]="Dir_";
    strcat(sir,stab.nume_u);
    strcat(sir,"/");
    strcat(sir,"Dir_BD_");
    strcat(sir,stab.nume_bd);
    strcat(sir,"/");
    strcat(sir,"Tabela_");
    strcat(sir,stab.nume_tab);
    strcat(sir,".txt");


    if(remove(sir) == 0)
    {
        printf("Tabela a fost ștearsa cu succes.\n");
    }
    else
    {
        perror("Eroare la ștergerea tabelei\n");
    }
}


pthread_mutex_t logMutex = PTHREAD_MUTEX_INITIALIZER;

void logOperation(const char* operationType, const char* sir)
{
    char timestamp[20];
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    pthread_mutex_lock(&logMutex);
   
    FILE* logFile = fopen("log.txt", "a");
    if (logFile == NULL) 
    {
        perror("Eroare la deschiderea fișierului de log");
        exit(EXIT_FAILURE);
    }

    if (sir != NULL) 
    {
        fprintf(logFile, "%s, %s, %s\n", timestamp, operationType, sir);
    }
    else 
    {
        fprintf(logFile, "%s, %s\n", timestamp, operationType);
    }

    fclose(logFile);
    pthread_mutex_unlock(&logMutex);
}


void client_handler(void *p_client) 
{
    int leave_flag = 0;
    char nickname[LENGTH_NAME] = {};
    char recv_buffer[LENGTH_MSG] = {};
    char send_buffer[LENGTH_SEND] = {};
    ClientList *np = (ClientList *)p_client;

    strcpy(np->name, "***");
    leave_flag=0;
    while (1) 
    {
        if (leave_flag) 
        {
            break;
        }
        TipMesaj tp;
        primeste_mesaj(np->data,&tp);
         printf("%s\n",tp.mesaj);
        if(strcmp(tp.mesaj,"nimic")==0)
        {
            break;
        }
       
        if(strcmp(tp.mesaj,"CreareCont")==0)
        {
          Utilizator u;
          
          primeste_Utilizator(np->data,&u);
          if(adauga_Utilizatori(u)==0)
          {
                TipMesaj ms;
                strcpy(ms.mesaj,"esec, userul exista");
                logOperation("CreareCont",ms.mesaj);
                trimite_mesaj(np->data,ms);
                printf("Userul deja exista\n");
          }
          else
          {
                TipMesaj ms;
                strcpy(ms.mesaj,"succes");
                trimite_mesaj(np->data,ms);
                logOperation("CreareCont",ms.mesaj);
                printf("Cont creat cu succes\n");
          }
        }
        if(strcmp(tp.mesaj,"Logare")==0)
        {
          Utilizator u;
          primeste_Utilizator(np->data,&u);
          printf("%s\n",u.nume_u);
          printf("%s\n",u.parola_u);
          if(verifica_Utilizator(u)==1)
          {
              printf("User gasit\n");
              TipMesaj msj;
              strcpy(msj.mesaj,"succes");
              logOperation("Logare",msj.mesaj);
              trimite_mesaj(np->data,msj);
          }
          else
          {
              printf("User negasit\n");
              TipMesaj msj;
              strcpy(msj.mesaj,"eroare");
              trimite_mesaj(np->data,msj);
          }
        }
        if(strcmp(tp.mesaj,"CreareBD")==0)
        {
            BazaDate bd;
            primeste_Baza(np->data,&bd);
            int aux=creeazaBd(bd);
            if(aux==3)
            {
                logOperation("CreareBD","Baza de data exista deja");
                printf("Baza de data exista deja\n");
            }
            else if(aux==0)
            {
                logOperation("CreareBD","Eroare creare Baza de date");
                printf("Eroare creare Baza de date\n");
            }
            else
            {
                logOperation("CreareBD","Baza de date creata cu succes");
                 printf("Baza de date creata cu succes\n");
            }

        }
        if(strcmp(tp.mesaj,"CreareTabela")==0)
        {
            Tabela tb;
            primeste_Tabela(np->data,&tb);
            int aux=creeazaTabela(tb);
            if(aux==-1)
            {
                logOperation("CreareTabela","Baza de date nu exista");
                printf("Baza de date nu exista\n");
            }
            else if(aux==0)
            {
                logOperation("CreareTabela","Tabela exista deja");
                printf("Tabela exista deja\n");
            }
            else
            {
                logOperation("CreareTabela","Baza de date nu exista");
                printf("Tabela creata cu succes\n");
            }
        }
        if(strcmp(tp.mesaj,"InserareDate")==0)
        {
            SelectTabela tab;
            primeste_SelectTabela(np->data,&tab);

            Tabela tb;
            extrageDateTabela(&tb,tab);
            trimite_Tabela(tb,np->data);

            Date dt;
            primeste_Date(np->data,&dt);
            int aux=adaugaInTabela(dt);
            if(aux)
            {
                logOperation("InserareDate","Date adaugate cu succes");
                printf("Date adaugate cu succes\n");
            }
        }
        if(strcmp(tp.mesaj,"SelectareDate")==0)
        {
            SelectTabela stb;
            primeste_SelectTabela(np->data,&stb);
            int aux=selectTabela(stb,np->data);
            if(aux)
            {
                logOperation("SelectareDate","Date selectate cu succes");
                printf("Date selectate cu succes\n");
            }
        }
        if(strcmp(tp.mesaj,"StergeRand")==0)
        {
            SelectRandTab st;
            primesteRandDeSters(np->data,&st);
            printf("%d\n",st.x);
            stergeRandTabela(st,np->data);

            logOperation("StergeRand","Rand sters cu succes");
        }
        if(strcmp(tp.mesaj,"UpdateDate")==0)
        {
            SelectTabela stb;
            primeste_SelectTabela(np->data,&stb);
            int aux=selectTabela(stb,np->data);
            if(aux)
            {
                printf("Date selectate cu succes\n");
                printf("------------------------\n");
                printf("Primeste date:\n");    
                SelectDateTot stdate;
                primeste_DateTot(np->data,&stdate);
                actualizaeazaTabela(stdate,stb);

                logOperation("UpdateDate","Data modificate cu succes");
                break;
            }

        }
        if(strcmp(tp.mesaj,"AfisTabele")==0)
        {
            BazaDate bd;
            primeste_Baza(np->data,&bd);
            ListaTabele lis;
            cauta_tabele(bd,&lis);
            
            int aux=trimite_Lista_Tabele(np->data,lis);
        }
        if(strcmp(tp.mesaj,"AfisBd")==0)
        {
            UT uu;
            primeste_U(np->data,&uu);
            ListaTabele lis;
            cauta_BD(uu,&lis);
            
            int aux=trimite_Lista_Tabele(np->data,lis);
        }
        if(strcmp(tp.mesaj,"StergeBD")==0)
        {
            BazaDate bd;
            primeste_Baza(np->data,&bd);
            
            sterge_Baza(bd);
        }
        if(strcmp(tp.mesaj,"StergeTabel")==0)
        {
            SelectTabela tb;
            primeste_SelectTabela(np->data,&tb);
            sterge_Tabela(tb);
        }
        
        if(strcmp(tp.mesaj,"Delogare")==0)
        {
            leave_flag=1;
            printf("Clientul %d  s-a delogat\n",np->data);
            logOperation("Delogare","Client delogat");
        }
        
    }
    close(np->data);
    if (np == now)
    { 
        now = np->prev;
        now->link = NULL;
    } 
    else 
    { 
        np->prev->link = np->link;
        np->link->prev = np->prev;
    }
    free(np);
}

void makeServer()
{
    server_sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (server_sockfd == -1) {
        printf("Eroare creare socket.");
        exit(EXIT_FAILURE);
    }

    int s_addrlen = sizeof(server_info);
    int c_addrlen = sizeof(client_info);
    memset(&server_info, 0, s_addrlen);
    memset(&client_info, 0, c_addrlen);
    server_info.sin_family = PF_INET;
    server_info.sin_addr.s_addr = INADDR_ANY;
    char stringValue[] = "8888";
    int intValue = atoi(stringValue);
    
    server_info.sin_port = htons((unsigned short)intValue);
    printf("%d\n",ntohs(server_info.sin_port));

    bind(server_sockfd, (struct sockaddr *)&server_info, s_addrlen);
    listen(server_sockfd, 5);

    getsockname(server_sockfd, (struct sockaddr*) &server_info, (socklen_t*) &s_addrlen);
    printf("Start Server : %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));
}

int main()
{
    signal(SIGINT, catch_ctrl_c_and_exit);
    makeServer();

    root = newNode(server_sockfd, inet_ntoa(server_info.sin_addr));
    now = root;
    while (1) 
    {
        client_sockfd = accept(server_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);

        getpeername(client_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
        printf("Client %s:%d adaugat.\n", inet_ntoa(client_info.sin_addr), client_info.sin_port);

        ClientList *c = newNode(client_sockfd, inet_ntoa(client_info.sin_addr));
        c->prev = now;
        now->link = c;
        now = c;

        pthread_t id;
        if (pthread_create(&id, NULL, (void *)client_handler, (void *)c) != 0) 
        {
            printf("Eroare pthread !\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("***********************\n");
        }
    }
    return 0;
}