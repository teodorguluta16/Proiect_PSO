#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define N 20 //dimensiune tip mesaj
#define M 30 // dimensiune mesaj
#define LUNGTABELA 500

int sockfd = 0;
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

typedef struct Tabela /// campurile tabelei
{
  char nume_u[M];
  char nume_bd[M];
  char nume_tab[M];
  char camp_INT[M][M];
  int nr_INT;
  char camp_VarChar[M][M];
  int nr_VarChar;
  char camp_date[M][M];
  int nr_Date;///data 
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

void primeste_mesaj(TipMesaj *tipm) 
{
    TipMesaj t;
    recv(sockfd, &t, sizeof(TipMesaj), 0);
    strcpy(tipm->mesaj,t.mesaj);
}
void primeste_Utilizator(Utilizator *u)
{
    Utilizator ut;
    recv(sockfd, &ut, sizeof(Utilizator), 0);
    strcpy(u->nume_u,ut.nume_u);
    strcpy(u->parola_u,ut.parola_u);
}
void primeste_Baza(BazaDate *bd)
{
    BazaDate b;
    recv(sockfd, &b, sizeof(BazaDate), 0);
    strcpy(bd->nume_bd,b.nume_bd);
    strcpy(bd->nume_u,b.nume_u);
}
void primeste_Tabela(Tabela *tab)
{
    Tabela tb;
    recv(sockfd, &tb, sizeof(Tabela), 0);
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
void primeste_Date(SelectDateTot *st)
{
    SelectDateTot stv;
    recv(sockfd,&stv,sizeof(SelectDateTot),0);
    st->nr=stv.nr;
    for(int i=0;i<stv.nr;i++)
        strcpy(st->randulDate[i],stv.randulDate[i]);
    
    strcpy(st->randulTabela,stv.randulTabela);

}
void primeste_Lista_Tabele(ListaTabele *tipm) 
{
    recv(sockfd, tipm, sizeof(ListaTabele), 0);
}
void trimiteDateTot(SelectDateTot st)
{
    send(sockfd,&st,sizeof(SelectDateTot),0);
}

void trimite_mesaj(TipMesaj tp) 
{
    send(sockfd, &tp, sizeof(TipMesaj), 0);
}
void trimite_Utilizator(Utilizator u)
{
    send(sockfd, &u, sizeof(Utilizator), 0);
}
void trimite_Baza(BazaDate bd)
{
    send(sockfd, &bd, sizeof(BazaDate), 0);
}
void trimite_Tabela(Tabela tab)
{
    send(sockfd, &tab, sizeof(Tabela), 0);
}
void trimite_insertTabela(Date dt)
{
    send(sockfd,&dt,sizeof(Date),0);
}
void trimite_SelectTabela(SelectTabela stab)
{
    send(sockfd,&stab,sizeof(SelectTabela),0);
}
void trimite_nrRand(SelectRandTab s)
{
    send(sockfd,&s,sizeof(SelectRandTab),0);
}
void trimite_u(UT u)
{
    send(sockfd,&u,sizeof(UT),0);
}
void conectare()
{
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1) {
        printf("Eroare creare socket.");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in server_info, client_info;
    int s_addrlen = sizeof(server_info);
    int c_addrlen = sizeof(client_info);
    memset(&server_info, 0, s_addrlen);
    memset(&client_info, 0, c_addrlen);
    server_info.sin_family = PF_INET;
    server_info.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_info.sin_port = htons(8888);

    
    int err = connect(sockfd, (struct sockaddr *)&server_info, s_addrlen);
    if (err == -1) {
        printf("Eroare conectare server!\n");
        exit(EXIT_FAILURE);
    }
    
    getsockname(sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
    getpeername(sockfd, (struct sockaddr*) &server_info, (socklen_t*) &s_addrlen);
    printf("Conectare la Server: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));
    printf("Client: %s:%d\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));
    
}
int main_menu()
{

    printf("Alegeti una dintre aceste optiuni:\n\n");

    printf("1-Creaza o baza de date\n");

    printf("2-Adauga o tabela la o baza de date\n");

    printf("3-Insereaza date intr-o tabela\n");

    printf("4-Sterge date dintr-o tabela\n");

    printf("5-Modifica date dintr-o tabela\n");

    printf("6-Afiseaza datele dintr-o tabela\n");

    printf("7-Afiseaza tabelele dintr-o baza:\n");

    printf("8-Afiseaza bazele de date:\n");

    printf("9-Sterge o baza de date\n");

    printf("10-Sterge un tabel\n");

    printf("11-Iesire\n");


    int o;

    scanf("%d", &o);

    return o;

}
 
int main()
{
   conectare();
    //CreareCont Logare CreareBD CreareTabela InserareDate, SelectareDate,StergeRand,UpdateDate
    printf("Alegeti login sau register\n1-login\n2-register\n");
    int opt;
    scanf("%d", &opt);
    if(!(opt == 1 || opt == 2))
    {
        printf("Trebuie sa alegeti 1 sau 2!\n");
        main();
    }
    char temp1[20], temp2[20];
    printf("Username:");
    scanf("%s", temp1);
    printf("Parola:");
    scanf("%s", temp2);
    
    Utilizator u;
    strcpy(u.nume_u, temp1);
    strcpy(u.parola_u, temp2);

    TipMesaj tip_log;
    if(opt==1)
    {
        strcpy(tip_log.mesaj, "Logare");
    }
    else if(opt==2)
    {
        strcpy(tip_log.mesaj, "CreareCont");
    }
    trimite_mesaj(tip_log);
    trimite_Utilizator(u);
    system("clear");
    TipMesaj tp;
    primeste_mesaj(&tp);
    
    if(strcmp(tp.mesaj,"succes")!=0)
    {
        system("clear");
        printf("%s\n",tp.mesaj);
        printf("Logarea nu a fost cu succes!\n\n");
        main();
    }
    else
    {
         printf("Logare cu succes!\n\n");
    }
    while(1)
    {
      
        int o_main=main_menu();
        while (!(o_main==1 || o_main==2 || o_main==3 || o_main==4 || o_main==5 || o_main==6 || o_main==7||o_main==8||o_main==9 || o_main==10|| o_main==11))
        {
            system("clear");

            printf("Trebuie sa alegeti una dintre optiuni\n");

            o_main=main_menu();
        }
  
  
        switch (o_main)
        {
            case 1:
                TipMesaj msj;
                strcpy(msj.mesaj,"CreareBD");
                trimite_mesaj(msj);
                printf("Introduceti numele BD(maxim 10 caractere):");
                char temp_bd[10];
                scanf("%s", temp_bd);
                BazaDate bd;
                strcpy(bd.nume_bd,temp_bd);
                strcpy(bd.nume_u,u.nume_u);
                trimite_Baza(bd);
                break;
            
            case 2:
                TipMesaj msj2;
                strcpy(msj2.mesaj,"CreareTabela");
                trimite_mesaj(msj2);
                printf("Numele tabelei(maxim 10 caractere):");
                char temp_t[10];
                scanf("%s", temp_t);
                Tabela t;
                strcpy(t.nume_tab,temp_t);
                printf("Numele bazei de date care o contine:");
                char baza_t[10];
                scanf("%s",baza_t);
                strcpy(t.nume_bd,baza_t);
                printf("Numarul de campuri int:");
                int n_int;
                scanf("%d",&n_int);
                t.nr_INT=n_int;
                printf("Numarul de campuri VARCHAR:");
                int n_char;
                scanf("%d",&n_char);
                t.nr_VarChar=n_char;
                printf("Numarul de campuri DATE:");
                int n_date;
                scanf("%d",&n_date);
                t.nr_Date=n_date;
                strcpy(t.nume_u, u.nume_u);
                trimite_Tabela(t);
                break;
            
            case 3:
                Date d;
                printf("Introduceti bazei de date: ");
                char tab_date_baza[N];
                scanf("%s", tab_date_baza);

                printf("Introduceti numele tabelei:");
                char tab_date_insert[N];
                scanf("%s", tab_date_insert);

                TipMesaj mesaj_insert;
                strcpy(mesaj_insert.mesaj, "InserareDate");// trimite ca vrei sa inserezi
                trimite_mesaj(mesaj_insert);

                // trimite numele tabelei in care vrei sa inserezi din care baza de date si din care utilizator
                SelectTabela tabe;
                strcpy(tabe.nume_u,u.nume_u);
                strcpy(tabe.nume_bd,tab_date_baza);
                strcpy(tabe.nume_tab,tab_date_insert);
                trimite_SelectTabela(tabe);
                //*************************************************************

                //primeste datele Tabelei selectate
                Tabela tabela_insert;
                primeste_Tabela(&tabela_insert);

                  printf("INT %d\n",tabela_insert.nr_INT);
                  printf("CHAR %d\n",tabela_insert.nr_VarChar);
                  printf("DATE %d\n",tabela_insert.nr_Date);

                  for(int i=0;i<tabela_insert.nr_INT;i++)
                      printf("%s\n",tabela_insert.camp_INT[i]);
                  for(int i=0;i<tabela_insert.nr_VarChar;i++)
                      printf("%s\n",tabela_insert.camp_VarChar[i]);
                  for(int i=0;i<tabela_insert.nr_Date;i++)
                    printf("%s\n",tabela_insert.camp_date[i]);
        
                //**************************************************************

                if(tabela_insert.nr_INT) 
                {
                    printf("Introduceti datele de tip int:\n");
                    char insert_int[M];    
                    for(int i=0;i<tabela_insert.nr_INT;i++)
                    {
                        scanf("%s", insert_int);
                        strcpy( d.camp_INT[i],insert_int);
                    }
                }
                if(tabela_insert.nr_VarChar)
                {
                    printf("Introduceti datele de tip char:\n");
                    char insert_char[M];
                    for(int i=0;i<tabela_insert.nr_VarChar;i++)
                    {
                        scanf("%s", insert_char);
                        strcpy(d.camp_VarChar[i],insert_char);

                    }
    
                }
                 if(tabela_insert.nr_Date)
                {
                    printf("Introduceti datele de tip Date:\n");
                    char insert_Date[10];
                    for(int i=0;i<tabela_insert.nr_Date;i++)
                    {
                        scanf("%s", insert_Date);
                        strcpy(d.camp_date[i],insert_Date);

                    }
                }
                strcpy(d.nume_bd,tabela_insert.nume_bd);
                strcpy(d.nume_u,tabela_insert.nume_u);
                strcpy(d.nume_tab,tabela_insert.nume_tab);
                d.nr_Date=tabela_insert.nr_Date;
                d.nr_INT=tabela_insert.nr_INT;
                d.nr_VarChar=tabela_insert.nr_VarChar;
                trimite_insertTabela(d);
                break;
            case 4:
                Date dd;
                printf("Introduceti numele bazei de date:");
                char tab_date_delete1[N];
                scanf("%s", tab_date_delete1);


                printf("Introduceti numele tabelei:");
                char tab_date_delete2[N];
                scanf("%s", tab_date_delete2);

                // trimite numele tabelei in care vrei sa stergi din care baza de date si din care utilizator
                SelectTabela tabe_delete;
                strcpy(tabe_delete.nume_u,u.nume_u);
                strcpy(tabe_delete.nume_bd,tab_date_delete1);
                strcpy(tabe_delete.nume_tab,tab_date_delete2);

                TipMesaj mesaj_delete;
                strcpy(mesaj_delete.mesaj, "SelectareDate");// trimite ca vrei sa stergi
                trimite_mesaj(mesaj_delete);
                trimite_SelectTabela(tabe_delete);
                //*********************

                // primeste datele Tabelei selectate
                SelectDateTot tabela_delete;
                primeste_Date(&tabela_delete);

                printf("Total: %d\n",tabela_delete.nr);
                for(int i=0;i<tabela_delete.nr;i++)
                    printf("%s\n",tabela_delete.randulDate[i]);

                //**********************

                // aici fac stergerea efectiva
                printf("\nIntroduceti Randul de sters: ");
                int var;
                scanf("%d", &var);
                SelectRandTab s;
                s.x=var;
                strcpy(s.nume_u,"alex");
                strcpy(s.nume_bd,tab_date_delete1);
                strcpy(s.nume_tab,tab_date_delete2);

                strcpy(mesaj_delete.mesaj, "StergeRand");// trimite ca vrei sa stergi
                trimite_mesaj(mesaj_delete);
                trimite_nrRand(s);


                break;
            case 5:
                //Update
                TipMesaj msju;
                strcpy(msju.mesaj,"UpdateDate");
                trimite_mesaj(msju);

                //*********************************

                printf("Introduceti numele bazei pentru UPDATE:");
                char baza_date_update[N];
                scanf("%s", baza_date_update);

                printf("Introduceti numele tabelei pentru UPDATE:");
                char tab_date_update[N];
                scanf("%s", tab_date_update);

                SelectTabela tb;

                strcpy(tb.nume_u,u.nume_u);
                strcpy(tb.nume_bd,baza_date_update);
                strcpy(tb.nume_tab,tab_date_update);
                trimite_SelectTabela(tb);

                SelectDateTot sf;
                primeste_Date(&sf);
                //for(int i=0;i<sf.nr;i++)
                  //  printf("%s\n",sf.randulDate[i]);

                //*********************************

                //Modifica datele
                //*****************

                printf("Introdu numarul randului de modificat: ");
                int xrand;
                scanf("%d",&xrand);
                xrand--;

                printf("Randul curent: %s\n",sf.randulDate[xrand]);

                printf("Introdu numarul coloanei: ");
               
                int col=0;
                scanf("%d",&col);

                printf("Introdu noua inregistrare: ");
                char inreg[25];
                scanf("%s",inreg);

                int nrr=strlen(sf.randulDate[xrand]);
                int contor=0;
                int pozitie=0;
                for(int j=0;j<nrr;j++)
                    if(sf.randulDate[xrand][j]==' ')
                    {
                        contor++;
                        j++;
                        if(contor==col-1)
                        {
                            pozitie=j;
                            break;
                        }
                    }
                pozitie=pozitie+1;
                strncpy( sf.randulDate[xrand]+ pozitie, inreg, strlen(inreg));
                printf("Randul modificat: %s\n",sf.randulDate[xrand]);
                //****************
                
                
                //trimite toate datele inapoi
                trimiteDateTot(sf);
                //*************************
                break;
            case 6:

                SelectTabela ss;
                printf("Introduceti numele bazei de date: ");
                scanf("%s", ss.nume_bd);
                printf("Introduceti numele tabelei: ");
                scanf("%s", ss.nume_tab);
                strcpy(ss.nume_u, u.nume_u);


                TipMesaj mesaj_s;
                strcpy(mesaj_s.mesaj, "SelectareDate");
                trimite_mesaj(mesaj_s);

                trimite_SelectTabela(ss);

                SelectDateTot sff;
                primeste_Date(&sff);
                for(int i=0;i<sff.nr;i++)
                    printf("%s\n",sff.randulDate[i]);    
                break;
            case 7:
                // afis tabele
                TipMesaj mesaj_sss;
                strcpy(mesaj_sss.mesaj, "AfisTabele");
                trimite_mesaj(mesaj_sss);

                char numebaza[20];
                BazaDate bzz;
                printf("Introdu numele bazei de date cautate: ");
                scanf("%s",bzz.nume_bd);
                strcpy(bzz.nume_u,u.nume_u);

                trimite_Baza(bzz);

                ListaTabele lis;
                primeste_Lista_Tabele(&lis);

                for(int i=0;i<lis.x;i++)
                {
                    printf("%s\n",lis.lista[i]);
                }
                break;
            case 8:
                TipMesaj mesaj_ssss;
                strcpy(mesaj_ssss.mesaj, "AfisBd");
                trimite_mesaj(mesaj_ssss);

                UT an;
                strcpy(an.nume_u,u.nume_u);

                trimite_u(an);

                ListaTabele liis;
                primeste_Lista_Tabele(&liis);

                for(int i=0;i<liis.x;i++)
                {
                    printf("%s\n",liis.lista[i]);
                }
                printf("\n\n");
                break;
            case 9:
                TipMesaj mesaj_sss3;
                strcpy(mesaj_sss3.mesaj, "StergeBD");
                trimite_mesaj(mesaj_sss3);

                BazaDate bzz2;
                printf("Introdu numele bazei de date de sters: ");
                scanf("%s",bzz2.nume_bd);
                strcpy(bzz2.nume_u,u.nume_u);

                trimite_Baza(bzz2);
                break;
            case 10:
                TipMesaj mesaj_sss4;
                strcpy(mesaj_sss4.mesaj, "StergeTabel");
                trimite_mesaj(mesaj_sss4);

                SelectTabela sters_tabel;
                printf("Introdu numele bazei de date: ");
                scanf("%s", sters_tabel.nume_bd);
                printf("Introdu numele tabelei: ");
                scanf("%s", sters_tabel.nume_tab);
                strcpy(sters_tabel.nume_u, u.nume_u);

                trimite_SelectTabela(sters_tabel);

                break;
            case 11:
                TipMesaj mesaj_sssss;
                strcpy(mesaj_sssss.mesaj, "Delogare");
                trimite_mesaj(mesaj_sssss);
                return 0;
                break;
            default:
                printf("Alege una dintre optiuni!\n");
                break;
        }

    }
    TipMesaj tpi;
    strcpy(tpi.mesaj,"nimic");
    trimite_mesaj(tpi);
    close(sockfd);
    printf("Disconnected from the server.\n");

  return 0;

}
