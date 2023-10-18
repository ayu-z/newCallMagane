#include "../fros/fros_common.h"
#include <string.h>
#include <stdlib.h>

int updateSystemNetworks()
{
    char buf[1024] = {0};

    struct uci_context *ctx = uci_alloc_context();
    if (!ctx){
        return EXIT_FAILURE;
    }


        if (UCI_OK == fros_uci_get_value(ctx, "network.modem.ifname", buf, sizeof(buf)))
		    printf("ifname:%s\n", strdup(buf));
    

    uci_free_context(ctx);


    return EXIT_SUCCESS;
}

int updateSystemLeds()
{

    return EXIT_SUCCESS;
}

int updateSystemFirewall()
{

    return EXIT_SUCCESS;
}



int main()
{
    updateSystemNetworks();
}