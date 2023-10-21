#include "modem_dial.h"
#include "../fros/fros_common.h"
#include "../common/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uci.h>


int updateModemDialConfig(char *path)
{
    if (path == NULL) {
        return EXIT_FAILURE;
    }
    
    struct uci_context *ctx = uci_alloc_context();
    if (!ctx){
        return EXIT_FAILURE;
    }

    char result[128] = {0};
    char key[32] = {0};
    dialConfig_t defaultConfig;
    memset(&defaultConfig, 0, sizeof(dialConfig_t));

    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.path", result, sizeof(result)))
        defaultConfig.path = strdup(result);
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.custom", result, sizeof(result)))
        defaultConfig.custom = atoi(result)?1:0;
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.apn", result, sizeof(result)))
        defaultConfig.apn = strdup(result);
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.user", result, sizeof(result)))
        defaultConfig.user = strdup(result);
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.passwd", result, sizeof(result)))
        defaultConfig.passwd = strdup(result);
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.auth", result, sizeof(result)))
        defaultConfig.auth = atoi(result);
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.ipv4", result, sizeof(result)))
        defaultConfig.ipv4 = atoi(result)?1:0;
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.ipv6", result, sizeof(result)))
        defaultConfig.ipv6 = atoi(result)?1:0;
    if (UCI_OK == fros_uci_get_value(ctx, "modemdial.global.pincode", result, sizeof(result)))
        defaultConfig.pincode = strdup(result);

    
    if (UCI_OK != fros_uci_add_section(ctx, "modemdial", "modem")){
        uci_free_context(ctx);
        return EXIT_FAILURE;
    }
    
    fros_uci_set_value(ctx, "modemdial.@modem[-1].path", path);
    fros_uci_set_int_value(ctx, "modemdial.@modem[-1].custom", defaultConfig.custom);
    fros_uci_set_value(ctx, "modemdial.@modem[-1].apn", defaultConfig.apn);
    fros_uci_set_value(ctx, "modemdial.@modem[-1].user", defaultConfig.user);
    fros_uci_set_value(ctx, "modemdial.@modem[-1].passwd", defaultConfig.passwd);
    fros_uci_set_int_value(ctx, "modemdial.@modem[-1].auth", defaultConfig.auth);
    fros_uci_set_int_value(ctx, "modemdial.@modem[-1].ipv4", defaultConfig.ipv4);
    fros_uci_set_int_value(ctx, "modemdial.@modem[-1].ipv6", defaultConfig.ipv6);
    fros_uci_set_value(ctx, "modemdial.@modem[-1].pincode", defaultConfig.pincode);

    fros_uci_commit(ctx, "modemdial");

    uci_free_context(ctx);
}



int loadModemDialConfig(char *path, dialConfig_t *dial)
{
    struct uci_context *ctx = uci_alloc_context();
    if (!ctx){
        return EXIT_FAILURE;
    }

    int index = fros_uci_get_section_index_by_str_opt(ctx, DIALPACKAGE_NAME, DIALSECTION_NAME, "path", path);
    if (index < 0){
        printf("not found path:%s, copy defaultConfig config\n", path);
        updateModemDialConfig(path);
        index = fros_uci_get_section_index_by_str_opt(ctx, DIALPACKAGE_NAME, DIALSECTION_NAME, "path", path);
    }

    char result[128] = {0};
    if (UCI_OK == fros_uci_get_array_value(ctx, "modemdial.@modem[%d].path", index, result, sizeof(result)))
        dial->path = strdup(result);
    if (UCI_OK == fros_uci_get_array_value(ctx, "modemdial.@modem[%d].path", index, result, sizeof(result)))
        dial->path = strdup(result);
    if (UCI_OK == fros_uci_get_array_value(ctx, "modemdial.@modem[%d].custom", index, result, sizeof(result)))
        dial->custom = atoi(result)?1:0;
    if (UCI_OK == fros_uci_get_array_value(ctx, "modemdial.@modem[%d].apn", index, result, sizeof(result)))
        dial->apn = strdup(result);
    if (UCI_OK == fros_uci_get_array_value(ctx, "modemdial.@modem[%d].user", index, result, sizeof(result)))
        dial->user = strdup(result);
    if (UCI_OK == fros_uci_get_array_value(ctx, "modemdial.@modem[%d].passwd", index, result, sizeof(result)))
        dial->passwd = strdup(result);
    if (UCI_OK == fros_uci_get_array_value(ctx, "modemdial.@modem[%d].auth", index, result, sizeof(result)))
        dial->auth = atoi(result);
    if (UCI_OK == fros_uci_get_array_value(ctx, "modemdial.@modem[%d].ipv4", index, result, sizeof(result)))
        dial->ipv4 = atoi(result)?1:0;
    if (UCI_OK == fros_uci_get_array_value(ctx, "modemdial.@modem[%d].ipv6", index, result, sizeof(result)))
        dial->ipv6 = atoi(result)?1:0;
    if (UCI_OK == fros_uci_get_array_value(ctx, "modemdial.@modem[%d].pincode", index, result, sizeof(result)))
        dial->pincode = strdup(result);

    uci_free_context(ctx);

    return EXIT_SUCCESS;
}

int main()
{

    dialConfig_t config;
    if(EXIT_SUCCESS == loadModemDialConfig("3-1.1", &config)){
        printf("path: %s\n", config.path);
        printf("custom: %d\n", config.custom);
        printf("apn: %s\n", config.apn);
        printf("user: %s\n", config.user);
        printf("passwd: %s\n", config.passwd);
        printf("auth: %d\n", config.auth);
        printf("ipv4: %d\n", config.ipv4);
        printf("ipv6: %d\n", config.ipv6);
        printf("pincode: %s\n", config.pincode);
    }

    return EXIT_SUCCESS;
}
