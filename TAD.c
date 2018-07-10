#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "vuelosADT.h"

/* List of sub lists were every node on the main list is an Airport from the Airport.cvs file so that all Airports are included
in alphabetical order according to their oaci. The sublists of every node consist of Airports arranged in aplhabetical order 
according to oaci (that may or may not be on the main list) that had a flight takeoff to or landed from the main node´s Airport */

typedef struct listCDT * listADT;

typedef struct mainAirport * mainAirportADT;

typedef struct subAir * subAirADT;

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
list->week = malloc(sizeof(int),7);
if(list->week != NULL)
	{
	for(size_t i=0; i<7;i++)
		list.week[i]=0;
	}
else
	return NULL;
return list;
}

void
freeList(listADT list)                //Free all the saved information about airports and flights.
{
freerecMain(list->firstMain);
free(list);
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

int
addmainAir(listADT l, char * oaci, char * local, char * iata, char * info)       // adds any airport to the Main list.
{
int flag=0;
l->firstMain=addmainAirRec(l->firstMain, oaci, local, iata, info, &flag);
if(flag)
	return 1;
return 0;
}

static mainAirportADT
addmainAirRec(mainAirportADT l, char * oaci, char * local, char * iata, char * info, int *flag)
{
if(l==NULL || strcmp(l->oaci,oaci)>0)
	{
	mainAirportADT aux=calloc(sizeof * aux);
	aux->oaci=malloc(5);
	aux->local=malloc(4);
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

int                                                                                                       //Here we save all the relations between two airports, a relation is made when a flight is made from one airport to another. We do not create a new sublist to any airport that is not in the main list.
addSubAir(listADT l, char * oaciOr, char * oaciDest, size_t day, size_t type, size_t movement,size_t flagday)            //type:aterrizaje=0/despegue=1,  movement:cabotaje=0/internacional=1 flagday tells us if that flight was already added to the week array or not.
{
  int flag=0;
  if (l == NULL || l->firstMain == NULL)
  {
    return 0;
  }
if(flagday)
  (l.week[day])++;

  if(!(search(l->firstMain,oaciOr,oaciDest,type,movement)))
  {
    return 0;
  }
  return 1;
}

int
search(mainAirportADT m, char * oaciOr, char * oaciDest, size_t type,size_t movement) //here we search the airport in the main list, its not necessarly the one where the flight started.
{
  if (m == NULL || (strcmp(m->oaci,oaciOr))>0)
  {
    return 0;
  }
  else if((strcmp(m->oaci,oaciOr))<0)
  {
    return search(m->nextMain,oaciOr,oaciDest,type);
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
    (m->takoffs)++;
    else
    (m->landings)++;
    m->firstSub=addSubAirRec(m->firstSub,oaciDest,type,&flag);
    return flag;
  }
}

subAirADT
addSubAirRec(subAirADT node, char * oaciDest,int type, int *flag)
{
  if (node == NULL || (strcmp(node->oaci,oaciDest))>0)
  {
    subAirADT new = calloc(sizeof(*new));
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



