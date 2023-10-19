#ifndef __MODEM_DETECT_H__
#define __MODEM_DETECT_H__


typedef struct{
    int idVendor;
    int idProduct;
    char *interface;
    char *atCom;
    char *driver;
    char *path;
} modemHwInfo_t;



#endif // !__MODEM_DETEH__


