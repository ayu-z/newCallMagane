#include "modem_dial.h"
#include "../fros/fros_common.h"
#include "../common/common.h"
#include <stdlib.h>
#include <string.h>

int loadModemDialConfig(char *path, dialConfig_t *dial)
{
    struct uci_context *ctx = uci_alloc_context();
    if (!ctx){
        return EXIT_FAILURE;
    }

    int index = fros_uci_get_section_index_by_str_opt(ctx, DIALPACKAGE_NAME, DIALSECTION_NAME, "path", path);
    if (index < 0){
        printf("not found path:%s, copy default config\n", path);
    }

    char result[128] = {0};
    char key[32] = {0};

    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.path", result, sizeof(result)))
        dial->path = strdup(result);
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.custom", result, sizeof(result)))
        dial->custom = atoi(result)?1:0;
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.apn", result, sizeof(result)))
        dial->apn = strdup(result);
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.user", result, sizeof(result)))
        dial->user = strdup(result);
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.passwd", result, sizeof(result)))
        dial->passwd = strdup(result);
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.auth", result, sizeof(result)))
        dial->auth = atoi(result);
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.ipv4", result, sizeof(result)))
        dial->ipv4 = atoi(result)?1:0;
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.ipv6", result, sizeof(result)))
        dial->ipv6 = atoi(result)?1:0;
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.pincode", result, sizeof(result)))
        dial->pincode = atoi(result);

    uci_free_context(ctx);

    return EXIT_SUCCESS;
}


int multiDeviceModemPathMatch(char *node)
{
    char x45HwNode[4][] = {
        {"3-1.4","4-1.4"},
        {"3-1.3","4-1.3"},
        {"3-1.2","4-1.3"},
        {"3-1.1","4-1.1"},
    };

    return 0;
}


int main()
{

    // updateModemDialConfig("11");
    struct uci_context *ctx = uci_alloc_context();
    if (!ctx){
        return EXIT_FAILURE;
    }

    char result[128] = {0};
    fros_uci_get_section_name(ctx, "modemdial", "dial", 1, result);

    uci_free_context(ctx);


    printf("result: %s\n", result);

    return EXIT_SUCCESS;
}