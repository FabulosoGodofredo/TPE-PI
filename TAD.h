#ifndef vuelosADT_h
#define vuelosADT_ h

typedef struct listCDT * listADT;
typedef struct mainAirport * mainAirportADT;
typedef struct subAir * subAirADT;


listADT newList(void);

void freeList(listADT list);

void freerecMain(mainAirportADT firstMain);

void freerecSub(subAirADT firstSub);

int addmainAir(listADT l, char * oaci, char * local, char * iata, char * info, size_t traffic);

static mainAirportADT addmainAirRec(mainAirportADT l, char * oaci, char * local, char * iata, char * info, size_t traffic, int *flag);

int addSubAir(listADT l, char * oaciOr, char * oaciDest, size_t day, size_t type, size_t movement);

int search(mainAirportADT m, char * oaciOr, char * oaciDest, size_t type,size_t movement);

subAirADT addSubAirRec(subAirADT node, char * oaciDest,int type, int *flag);

static void QuerryUNOrec(FILE * f, mainAirportADT l);

int QuerryUNO(listADT list);

#endif
