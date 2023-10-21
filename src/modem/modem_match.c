#include "modem_match.h"
#include "../common/common.h"
#include "../common/hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HASH_LIMIT 0x41a7

typedef void (*modemEventHandle)(void *);

typedef struct {
    int idVendor;
    int idProduct;
} UID_t;


typedef struct {
    char *name;
    int ATidx;
    modemEventHandle init;
    modemEventHandle dial;
} Modem_t;

typedef struct {
    UID_t uid;
    Modem_t modem;
} ModemInfo_t;

unsigned int customKeyInsertHandle(void *key) {
    UID_t *hashKey = (UID_t *)key;

    return (unsigned int)((((unsigned long)hashKey->idVendor << 16) | hashKey->idProduct)%MAX_HASH_LIMIT);
}

unsigned int customKeyCompareHandle(void *key1, void *key2) {
    UID_t *uid1 = (UID_t *)key1;
    UID_t *uid2 = (UID_t *)key2;

    return (uid1->idVendor == uid2->idVendor && uid1->idProduct == uid2->idProduct);
}

void modemInit(void *data) 
{
    printf("modemEventHandle init: %s\n", (char *)data);
}

void modemDial(void *data) 
{
    printf("modemEventHandle dial: %s\n", (char *)data);
}

static ModemInfo_t modemMatchTable[] = {
    
    {0x2c7c, 0x0800, VENDOR_QUECTEL(RM500Q), 3, modemInit, modemDial},
    {0x2c7c, 0x0900, VENDOR_QUECTEL(RM500U), 3, modemInit, modemDial},
    {0x2c7c, 0x0125, VENDOR_QUECTEL(EC20), 3, modemInit, modemDial},
    {0x2c7c, 0x0620, VENDOR_QUECTEL(EM1x0RG), 3, modemInit, modemDial},
    {0x2c7c, 0x030a, VENDOR_QUECTEL(EM05G), 3, modemInit, modemDial},
    {0x2c7c, 0x030e, VENDOR_QUECTEL(EM05G), 3, modemInit, modemDial},
    {0x12d1, 0x1506, VENDOR_QUECTEL(EM350), 3, modemInit, modemDial},
    {0x2dee, 0x4d22, VENDOR_MEIG(SRM-Series), 3, modemInit, modemDial},
    {0x2dee, 0x4d58, VENDOR_MEIG(SLM770), 2, modemInit, modemDial},
    {0x2dee, 0x4d52, VENDOR_MEIG(SRM821), 1, modemInit, modemDial},
    {0x05c6, 0xf601, VENDOR_MEIG(SLM750), 3, modemInit, modemDial},
    {0x3c93, 0xffff, VENDOR_MEIG(SRM810), 3, modemInit, modemDial},
    {0x2cb7, 0x0104, VENDOR_FIBOCOM(FM150), 3, modemInit, modemDial},
    {0x1508, 0x1001, VENDOR_FIBOCOM(NL668), 3, modemInit, modemDial},
    {0x12d1, 0x15c3, "HuaWei MH5000", 3, modemInit, modemDial},
    {0x3466, 0x3301, "TD Tech MT5710", 1, modemInit, modemDial},
    {0x1286, 0x4e3c, "YuGe CLM920", 3, modemInit, modemDial},
    {0x1e2d, 0x00b3, "Thales T99W175", 3, modemInit, modemDial},
    {0x1e0e, 0x9001, "SimCOM SIM8200", 3, modemInit, modemDial},
    {0x1782, 0x4038, "Haixing MNR01", 3, modemInit, modemDial},
    {0x05c6, 0x9091, "Yidong F03X", 3, modemInit, modemDial},
    {0x05c6, 0x90db, "yiseacom", 3, modemInit, modemDial},
    {0x05c6, 0x90d5, "FOXCON", 3, modemInit, modemDial},
    {0x2949, 0x8802, "YouFANG N510M", 3, modemInit, modemDial},
    {0x2cb7, 0x0a05, "YanFei Fibcom", 3, modemInit, modemDial},
};


void printfModemMatchTable(hashTable_t* hashTable)
{
    Modem_t *res;

    for (size_t i = 0; i < countKeysHashTable(hashTable); i++){
        res = (Modem_t *)searchKeyHashTable(hashTable, &modemMatchTable[i].uid);
        if (res != NULL) {
            printf("idx: %02ld Modem Name: %s - AT COM:%d\n", i, res->name, res->ATidx);

        } 
    }
}

int main() 
{
    hashTable_t* hashTable = createHashTable(customKeyInsertHandle, customKeyCompareHandle);

    for (int i = 0; i < sizeof(modemMatchTable) / sizeof(modemMatchTable[0]); i++) {
        insertKeyHashTable(hashTable, &modemMatchTable[i].uid, &modemMatchTable[i].modem);
    }

    printfModemMatchTable(hashTable);

    destroyHashTable(hashTable);

    return EXIT_SUCCESS;
}
