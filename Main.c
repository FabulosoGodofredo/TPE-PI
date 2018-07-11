#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "TAD.h"
#include "getnum.h"

int
dateToDay(char* str)               //This tell us what day this is.
{
  int d=0, m=0, y=0, count=0;
  d+=((str[0]-'0')*10+(str[1]-'0')-1);
  m+=((str[3]-'0')*10+(str[4]-'0'));
  y+=(2010+(str[9]-'0'));
  int month[]={31,28,31,30,31,30,31,31,30,31,30,31};
  for (size_t i = 1; i < m; i++)
    {
      count+=month[i];
    }
  if(y>2016 || (y==2016 && m>2))
    {
      count+=1;
    }
  count+=((y-2014)*365+d);
  return (count+2)%7;              //The first day of 2014 is Wednesday, so we +2, so 0 es monday, 1 tuesday...
}


int
main(void)
{
  int error=0;        //indicates if OACI is empty
  int OutMem=0;       //indicates if there is no memory for adding a new Airport
  int year=getint("Insert a year between 2014-2018 inclusive: \n");
  while(year<2014 || year>2018)
  {
  year=getint("Please verify that the year is between 2014-2018 inclusive, insert again: \n");
  }
  FILE * pf;

  listADT list = newList();
  if (list == NULL)
  {
    printf("There is no enough memory to create the list\n");
  }
  char aeros[95];
  memset(aeros,0,95);  //We fill the array with \0

  char local[4];
  char oaci[5];
  char iata[4];
  char info[71];


  pf= fopen("aeropuertos_detalle.csv","rt");  //try to open "aeropuertos.csv" in reading mode.

  if (pf == NULL)
  {
    printf("The file aeropuertos_detalle.csv could not be read\n");
    return 1;   //ends with error.
  }

  else
  {
    fgets(aeros,95,pf);   //reads a line and saves it in an array
    int Notfirst=0;       // it is the TITLE which explains in which order the information is written, it is not added it to the list.

    while (!feof(pf) && !OutMem)
    {
      if (Notfirst)
      {
        strcpy(local,strtok(aeros,";"));
        strcpy(oaci,strtok(NULL,";"));
        if (*oaci != ' ')
        {
          strcpy(iata,strtok(NULL,";"));
          strtok(NULL,";");                //we skip the information of the type, type indicates if its Airport or Heliport.
          strcpy(info,strtok(NULL,";"));
        }
        else
          error=1;

          if (!error && !(addmainAir(list,oaci,local,iata,info)))
          {
            printf("The Airport %s could not be added\n",local);
            OutMem=1;
          }
      }

        memset(aeros,0,95);
        fgets(aeros,95,pf);
        error=0;                //as the OACI of the next airport can be in blank, we change the value of the flag back to 0
        Notfirst=1;             //The TITLE was already read so the flag it is no longer useful, so we left it in 1.

    }
  }
  fclose(pf);       //we close the file after we dont need more information from it.

  if (OutMem)
  {
    return 1;     //error
  }

  FILE * fy;
  char flight[66];
  memset(flight,0,66);

  char day[11];
  char movement[14];
  char type[11];
  char oaciOr[5];
  char oaciDest[8];

  fy= fopen("vuelos.csv","rt");

  if (fy == NULL)          //To make sure that there is still more memory to analyze the second file.
  {
    printf("The file vuelos.csv could not be read\n");
    OutMem=1;   //ends with error
  }

  else
  {
    fgets(flight,66,fy);       //here we send all the information needed to complete the sublists.
    int Notfirst=0;
    int Imovement=0;
    int Itype=0;
    int flagday=1;

      while (!feof(fy) && !OutMem)
      {
        if (Notfirst)
        {
          strcpy(day,strtok(flight,";"));
          int Iyear= 1000*(*(day+6)-'0')+100*(*(day+7)-'0')+10*(*(day+8)-'0')+(*(day+9)-'0');
          if (year == Iyear)
          {
            int Iday = dateToDay(day);
            strtok(NULL,";");
            strtok(NULL,";");
            strcpy(movement,strtok(NULL,";"));
            if ((strcmp(movement,"Internacional")) == 0)
            {
                Imovement=1;
            }
            strcpy(type,strtok(NULL,";"));
            if ((strcmp(type,"Despegue"))==0)
            {
                Itype=1;
            }
            strcpy(oaciOr,strtok(NULL,";"));
            strcpy(oaciDest,strtok(NULL,";"));
            if (!isdigit(*(oaciDest+3)))
            {
              if (!addSubAir(list,oaciDest,oaciOr,Iday,!Itype,Imovement,!flagday))
               {
                 printf("The Airport %s could not be added\n",oaciOr);
                 OutMem=1;
               }
            }
            if (!addSubAir(list,oaciOr,oaciDest,Iday,Itype,Imovement,flagday))
             {
               printf("The Airport %s could not be added\n",oaciOr);
               OutMem=1;
             }
        }
      }

        memset(flight,0,66);
        fgets(flight,66,fy);
        Notfirst=1;
        Imovement=0;
        Itype=0;
      }
  }
  fclose(fy);
  if (OutMem) {
    return 1;
  }

QuerryUNO(list);
QuerryDOS(list);
QuerryTRES(list);
QuerryCUATRO(list);
freeList(list);

  return 0;
}
