#ifndef __MODEM_DIAL_H__
#define __MODEM_DIAL_H__


#define DIALTOOL_QUECTEL    "quectel-CM"
#define DIALTOOL_MEIG       "meig-CM"
#define DIALTOOL_FIBOCOM    "fibocom-dial"
#define DIALPACKAGE_NAME    "modemdial"
#define DIALSECTION_NAME    "modem"

typedef struct{
	char *path;
	int custom;
    char *apn;
	char *user;
	char *passwd;
	int auth;
	int ipv4;
	int ipv6;
	char *pincode;
} dialConfig_t;

int loadModemDialConfig(char *path, dialConfig_t *dial);

#endif // !__MODEM_DIAL_H