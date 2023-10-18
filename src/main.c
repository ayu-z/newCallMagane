#include "common/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

typedef struct{
    int idVendor;
    int idProduct;
    char *interface;
    char *atCom;
    char *driver;
    char *path;
} modemHwInfo_t;

int main() {
    glob_t glob_result;
    
    if (glob("/sys/class/net/*", GLOB_ERR, NULL, &glob_result) != 0) {
        perror("Error in glob");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
        const char *path = glob_result.gl_pathv[i];
        char driver[MAX_STRING_LENGTH/8];
        char ifcPath[MAX_STRING_LENGTH/4];
        char atCom[MAX_STRING_LENGTH/8];
        char *ifc ;
        int err;

        if (strstr(path, "wwan") || strstr(path, "usb") || strstr(path, "eth")) {
            ifc = extractSegment(path, "/", -1);
            err = getInterfaceDriver(strdup(ifc), driver);
            if (err == EXIT_FAILURE) {
                perror("Error: DRIVER= not found\n");
                continue;
            }
        }else {
            continue;
        }

        err = interfaceIsModem(driver);
        if(!err){
            continue;
        }

        err = getIntrefacePath(strdup(ifc), ifcPath);
        if( err == EXIT_FAILURE){
            continue;
        }

        char *base = extractSegmentRange(ifcPath, "/", 1, -4);
        char *node = strdup(extractSegment(ifcPath, "/", -4));
        err = getAtComIndex(node, 3, atCom);
        if( err == EXIT_FAILURE){
            continue;
        }

        char uidPath[MAX_STRING_LENGTH/2];
        snprintf(uidPath, sizeof(uidPath), "/%s/%s", base, "idVendor");
        FILE *uidFp = fopen(uidPath, "r");
        if (uidFp == NULL) {
            perror("Error opening file");
            continue;
        }
        char idVendor[10];
        if (fscanf(uidFp, "%s", idVendor) != 1) {
            perror("Error reading");
            fclose(uidFp);
            continue;
        }
        fclose(uidFp);

        snprintf(uidPath, sizeof(uidPath), "/%s/%s", base, "idProduct");
        uidFp = fopen(uidPath, "r");
        if (uidFp == NULL) {
            perror("Error opening file");
            continue;
        }
        char idProduct[10];
        if (fscanf(uidFp, "%s", idProduct) != 1) {
            perror("Error reading");
            fclose(uidFp);
            continue;
        }
        fclose(uidFp);

        modemHwInfo_t modemHwInfo = {
            .idVendor = strtol(idVendor, NULL, 16),
            .idProduct = strtol(idProduct, NULL, 16),
            .interface = strdup(ifc),
            .atCom = strdup(atCom),
            .driver = strdup(driver),
            .path = strdup(base),
        };

        printf("idVendor:%x\nidProduct:%x\nifc:%s\natCom:%s\ndriver:%s\nmodemPath:%s\n", modemHwInfo.idVendor, modemHwInfo.idProduct, modemHwInfo.interface, modemHwInfo.atCom, modemHwInfo.driver, modemHwInfo.path);

    }

    globfree(&glob_result);

    return EXIT_SUCCESS;
}
