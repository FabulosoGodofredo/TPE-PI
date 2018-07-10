#ifndef TAD_h
#define TAD_h

typedef struct listCDT * listADT;
typedef struct mainAirport * mainAirportADT;
typedef struct subAir * subAirADT;


listADT newList(void);

void freeList(listADT list);

int addmainAir(listADT l, char * oaci, char * local, char * iata, char * info);

int addSubAir(listADT l, char * oaciOr, char * oaciDest, size_t day, size_t type, size_t movement, int flagday);

int QuerryUNO(listADT list);

int QuerryDOS(listADT list);

int QuerryTRES(listADT list);

int QuerryCUATRO(listADT list);

#endif

