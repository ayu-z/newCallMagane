#ifndef __MODEM_DIAL_H__
#define __MODEM_DIAL_H__


#define DIALTOOL_QUECTEL    "quectel-CM"
#define DIALTOOL_MEIG       "meig-CM"
#define DIALTOOL_FIBOCOM    "fibocom-dial"
#define DIALPACKAGE_NAME    "modemdial"
#define DIALSECTION_NAME    "dial"

typedef struct{
	char *path;
	int custom;
    char *apn;
	char *user;
	char *passwd;
	int auth;
	int ipv4;
	int ipv6;
	int pincode;
} dialConfig_t;

enum modemch{
	UNKNOWN_CH,
	MODEM_CH1,
	MODEM_CH2,
	MODEM_CH3,
	MODEM_CH4,
};

#endif // !__MODEM_DIAL_H