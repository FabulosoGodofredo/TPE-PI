#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "vuelosADT.h"


int
main()
{
  int error=0;        //indicates if OACI is empty
  int OutMem=0;       //indicates if there is no memory for adding a new Airport
  FILE * pf;
  
  listADT list = newList();
  if (list == NULL) 
  {
    printf("No hay espacio suficiente para el programa\n");
  }
  char aeros[95];
  memset(aeros,0,95);  //Llenamos todo el vector con \0
  
  char local[4];
  char oaci[5];
  char iata[4];
  char info[71];


  pf= fopen("aeropuertos.csv","rt");  //try to open "aeropuertos.csv" in reading mode

  if (pf == NULL) 
  {
    printf("The file aeropuertos.csv could not be read\n");
    return 1;   //ends with error
  }

  else
  {
    fgets(aeros,95,pf);   //reads a line and saves it in an array 
    int Notfirst=0;       // its the TITTLE line, do not add it to the list
    
    while (!feof(pf) && !OutMem) 
    {
      if (Notfirst) 
      {
        strcpy(local,strtok(aeros,";"));
        strcpy(oaci,strtok(NULL,";"));
        if (*oaci != ' ') 
        {
          strcpy(iata,strtok(NULL,";"));
          strtok(NULL,";");
          strcpy(info,strtok(NULL,";"));
        }
        else
          error=1;

          if (!error && !addmainAir(list,oaci,local,iata,info)) 
          {
            printf("The Airport %s could not be added %s\n",local);
            OutMem=1;
          }
      }
        
        memset(aeros,0,95);
        fgets(aeros,95,pf);
        error=0;
        Notfirst=1;
        
    }
  }
  
}
