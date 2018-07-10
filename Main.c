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
    printf("There is no enough memory to create the list\n");
  }
  char aeros[95];
  memset(aeros,0,95);  //We fill the array with \0
  
  char local[4];
  char oaci[5];
  char iata[4];
  char info[71];


  pf= fopen("aeropuertos.csv","rt");  //try to open "aeropuertos.csv" in reading mode.

  if (pf == NULL) 
  {
    printf("The file aeropuertos.csv could not be read\n");
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

          if (!error && !addmainAir(list,oaci,local,iata,info)) 
          {
            printf("The Airport %s could not be added %s\n",local);
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
}
