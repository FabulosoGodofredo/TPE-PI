#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "TAD.h"

/* List of sub lists were every node on the main list is an Airport from the Airport.cvs file so that all Airports are included
in alphabetical order according to their oaci. The sublists of every node consist of Airports arranged in aplhabetical order
according to oaci (that may or may not be on the main list) that had a flight takeoff to or landed from the main node´s Airport */


typedef struct subAir{
char * oaci;
size_t takeoffs; //amount of flights that takeoff from this specific subAirport to corresponding mainAirpot
size_t landings; //amount of flights that land on this specific subAirport from corresponding mainAirport
subAirADT nextSub;
}subAir;

typedef struct mainAirport{
char * oaci;
char * local;
char * iata;
char * info; // name of Airport
size_t takeoffs;
size_t landings;
size_t landingsInter;
size_t takeoffsInter;
mainAirportADT nextMain;
subAirADT firstSub;
}mainAirport;

typedef struct listCDT{
int week[7];                //in this array, that every airport list has, keeps the information of how many flights occurs each day of a week during the year asked.
mainAirportADT firstMain;
}listCDT;

listADT                                      //we create a new list with the week array already explained before, and beginning of the list. If there is no memory, it gives back NULL.
newList(void)
{
listADT list=calloc(sizeof(listCDT),1);
return list;
}

static void
freerecSub(subAirADT firstSub)
{
if(firstSub!=NULL)
	{
	freerecSub(firstSub->nextSub);
	free(firstSub->oaci);
	free(firstSub);
	}
return;
}


static void
freerecMain(mainAirportADT firstMain)
{
if(firstMain!=NULL)
{
freerecSub(firstMain->firstSub);
freerecMain(firstMain->nextMain);
free(firstMain->oaci);
free(firstMain->local);
free(firstMain->iata);
free(firstMain->info);
free(firstMain);
}
return;
}

void
freeList(listADT list)                //Free all the saved information about airports and flights.
{
freerecMain(list->firstMain);
free(list);
}


static mainAirportADT
addmainAirRec(mainAirportADT l, char * oaci, char * local, char * iata, char * info, int *flag)
{
if(l==NULL || strcmp(l->oaci,oaci)>0)
	{
	mainAirportADT aux=malloc(sizeof(*aux));
	aux->oaci=malloc(strlen(oaci)+1);
	aux->local=malloc(strlen(local)+1);
	aux->iata=malloc(strlen(iata)+1);
	aux->info=malloc(strlen(info)+1);
	if (aux!=NULL && oaci!=NULL && local!=NULL && iata!=NULL && info!=NULL)
		{
		*flag=1;
		strcpy(aux->oaci,oaci);
		strcpy(aux->local,local);
		strcpy(aux->iata,iata);
		strcpy(aux->info,info);
		aux->nextMain=l;
		return aux;
		}
	}
else
	l->nextMain=addmainAirRec(l->nextMain, oaci, local, iata, info, flag);
return l;
}


int
addmainAir(listADT l, char * oaci, char * local, char * iata, char * info)       // adds any airport to the Main list.
{
int flag=0;
l->firstMain=addmainAirRec(l->firstMain, oaci, local, iata, info, &flag);
if(flag)
	return 1;
return 0;
}


static subAirADT
addSubAirRec(subAirADT node, char * oaciDest,int type, int *flag)
{
  if (node == NULL || (strcmp(node->oaci,oaciDest))>0)
  {
    subAirADT new = malloc(sizeof(*new));
    new->oaci=malloc(4);
    if (new!=NULL && new->oaci!=NULL)
    {
      strcpy(new->oaci,oaciDest);
      *flag=1;
      if (type)          //is takeoff
      {
        (new->takeoffs)++;
      }
      else
        (new->landings)++;
      return new;
    }
    return node;
  }
  else if ((strcmp(node->oaci,oaciDest))==0)
  {
    *flag=1;
    if (type)          //is takeoff
    {
      (node->takeoffs)++;
    }
    else
      (node->landings)++;
    return node;
  }
  else
  {
    node->nextSub = addSubAirRec(node->nextSub,oaciDest,type,flag);
    return node;
  }
}


static int
search(mainAirportADT m, char * oaciOr, char * oaciDest, size_t type,size_t movement) //here we search the airport in the main list, its not necessarly the one where the flight started.
{
  if (m == NULL || (strcmp(m->oaci,oaciOr))>0)
  {
    return 0;
  }
  else if((strcmp(m->oaci,oaciOr))<0)
  {
    return search(m->nextMain,oaciOr,oaciDest,type,movement);
  }
  else
  {
    int flag=0;
    if (movement==1)
    {
      if (type)
      {
        (m->takeoffsInter)++;
      }
      else
        (m->landingsInter)++;
    }
    if(type)
    (m->takeoffs)++;
    else
    (m->landings)++;
    m->firstSub=addSubAirRec(m->firstSub,oaciDest,type,&flag);
    return flag;
  }
}


int                                                                                                       //Here we save all the relations between two airports, a relation is made when a flight is made from one airport to another. We do not create a new sublist to any airport that is not in the main list.
addSubAir(listADT l, char * oaciOr, char * oaciDest, size_t day, size_t type, size_t movement,int flagday)            //type:aterrizaje=0/despegue=1,  movement:cabotaje=0/internacional=1 flagday tells us if that flight was already added to the week array or not.
{
  if (l == NULL || l->firstMain == NULL)
  {
    return 0;
  }
if(flagday)
  (*l).week[day]++;

  if(!(search(l->firstMain,oaciOr,oaciDest,type,movement)))
  {
    return 0;
  }
  return 1;
}

static void
QuerryUNOrec(FILE *f, mainAirportADT l)
{
if(l!=NULL)
{
	int suma[1]={l->takeoffs+l->landings};
	if(suma!=0)
		{
		fwrite(l->oaci, 1, sizeof(l->oaci), f);
		fwrite(";",1,sizeof(char),f);
		fwrite(l->local, 1, sizeof(l->local), f);
		fwrite(";",1,sizeof(char),f);
		fwrite(l->info, 1, sizeof(l->info), f);
		fwrite(";",1,sizeof(char),f);
		fwrite(suma, sizeof(int), 1, f);
		fwrite("\n",1,sizeof(char),f);
		QuerryUNOrec(f,l->nextMain);
		}
}
return;
}

int
QuerryUNO(listADT list)
{
FILE * Querry1;
Querry1 = fopen("desktop/TPE/movs_aeropuerto.csv","wt");
if (Querry1==NULL)
{
	fclose(Querry1);
	return 0;
}
QuerryUNOrec(Querry1,list->firstMain);
fclose(Querry1);
return 1;
}

static void
QuerryDOSrec(FILE *f, mainAirportADT l)
{
if(l!=NULL)
{
	int suma[1]={l->takeoffsInter+l->landingsInter};
	if(suma!=0)
	{
		fwrite(l->oaci, 1, sizeof(l->oaci), f);
		fwrite(";",1,sizeof(char),f);
		fwrite(l->iata, 1, sizeof(l->iata), f);
		fwrite(";",1,sizeof(char),f);
		int tk[2]={l->takeoffsInter,l->landingsInter};
		fwrite(tk, sizeof(int), 1, f);
		fwrite(";",1,sizeof(char),f);
		fwrite(tk+1, sizeof(int), 1, f);
		fwrite(";",1,sizeof(char),f);
		fwrite(suma, sizeof(int), 1, f);
		fwrite("\n",1,sizeof(char),f);
		QuerryDOSrec(f,l->nextMain);
	}
}
return;
}

int
QuerryDOS(listADT list)
{
FILE * Querry2;
Querry2 = fopen("desktop/TPE/movs_internacional.csv","wt");
if (Querry2==NULL)
{
	fclose(Querry2);
	return 0;
}
QuerryDOSrec(Querry2,list->firstMain);
fclose(Querry2);
return 1;
}

int
QuerryTRES(listADT list)
{
FILE * Querry3;
Querry3 = fopen("desktop/TPE/semanal.csv","wt");
if (Querry3==NULL)
{
	fclose(Querry3);
	return 0;
}
char *vec[] = {"Lunes","Martes","Miercoles","Jueves","Viernes","Sabado","Domingo"};
for (int i=0; i < 7; i++)
{
	fwrite(vec[i], 1, sizeof(*vec[i]), Querry3);
	fwrite(";",1,sizeof(char),Querry3);
	fwrite((list->week)+i,sizeof(int),1,Querry3);
	fwrite("\n",1,sizeof(char),Querry3);
}
fclose(Querry3);
return 1;
}

static void
QuerryCUATROrecrec(FILE *f, mainAirportADT l, subAirADT s)
{
if(s!=NULL)
{
	fwrite(l->oaci, 1, sizeof(l->oaci), f);
	fwrite(";",1,sizeof(char),f);
	fwrite(s->oaci, 1, sizeof(l->iata), f);
	fwrite(";",1,sizeof(char),f);
	int tk[2]={s->takeoffs,s->landings};
	fwrite(tk, sizeof(int), 1, f);
	fwrite(";",1,sizeof(char),f);
	fwrite(tk+1, sizeof(int), 1, f);
	fwrite("\n",1,sizeof(char),f);
	QuerryCUATROrecrec(f,l,s->nextSub);
}
return;
}

static void
QuerryCUATROrec(FILE *f, mainAirportADT l)
{
if(l!=NULL)
{
		QuerryCUATROrecrec(f,l,l->firstSub);
		QuerryCUATROrec(f,l->nextMain);
}
return;
}

int
QuerryCUATRO(listADT list)
{
FILE * Querry4;
Querry4 = fopen("desktop/TPE/aerop_detalle.csv","wt");
if (Querry4==NULL)
{
	fclose(Querry4);
	return 0;
}
QuerryCUATROrec(Querry4,list->firstMain);
fclose(Querry4);
return 1;
}

