#include "../fros/fros_common.h"
#include "../common/common.h"
#include <string.h>
#include <stdlib.h>
#include <uci.h>

int updateSystemNetworks(const char *modem, char *interfaceName)
{
    if (interfaceName == NULL){
        return EXIT_FAILURE;       
    }

    if (modem == NULL) {
        modem = strdup("modem");
    }

    char result[MAX_STRING_LENGTH] = {0};
    char *ifname = NULL;

    struct uci_context *ctx = uci_alloc_context();
    if (!ctx){
        return EXIT_FAILURE;
    }

    char key[64] = {0};

    sprintf(key, "network.%s.ifname", modem);

    if (UCI_OK == fros_uci_get_value(ctx, key, result, sizeof(result))){
        ifname = strdup(result);
        printf("ifname:%s\n", ifname);
    }else{
        goto EXIT_ERR;
    }
    
    if(strcmp(ifname, interfaceName) != 0){
        if (UCI_OK != fros_uci_set_value(ctx, key, interfaceName)){
            goto EXIT_ERR;
        }
        fros_uci_commit(ctx, "network");
        system("/etc/init.d/network reload");
    }


    uci_free_context(ctx);
    return EXIT_SUCCESS;

EXIT_ERR:
    uci_free_context(ctx);
    return EXIT_FAILURE;
}

int updateSystemLeds(char *interfaceName)
{
    if (interfaceName == NULL){
        return EXIT_FAILURE;       
    }

    char result[MAX_STRING_LENGTH] = {0};
    char *dev = NULL;

    struct uci_context *ctx = uci_alloc_context();
    if (!ctx){
        return EXIT_FAILURE;
    }


    if (UCI_OK == fros_uci_get_value(ctx, "system.modem.dev", result, sizeof(result))){
        dev = strdup(result);
        printf("modem led dev:%s\n", dev);
    }else{
        goto EXIT_ERR;
    }
    
    if(strcmp(dev, interfaceName) != 0){
        if (UCI_OK != fros_uci_set_value(ctx, "system.modem.dev", interfaceName)){
            goto EXIT_ERR;
        }
        fros_uci_commit(ctx, "system");
        system("/etc/init.d/led reload");
    }


    uci_free_context(ctx);
    return EXIT_SUCCESS;

EXIT_ERR:
    uci_free_context(ctx);
    return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

/*
int updateSystemFirewall()
{

    return EXIT_SUCCESS;
}
*/
