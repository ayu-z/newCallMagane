#include "common.h"
#include <glob.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>   



char *extractSegment(const char *source, const char *delimiter, int index) 
{
    char copy[MAX_STRING_LENGTH];
    char *segment;

    strncpy(copy, source, sizeof(copy));
    copy[sizeof(copy) - 1] = '\0';

    segment = strtok(copy, delimiter);

    int totalSegments = 0;
    while (segment != NULL) {
        totalSegments++;
        segment = strtok(NULL, delimiter);
    }

    int targetIndex = (index >= 0) ? index : totalSegments + index + 1;

    strncpy(copy, source, sizeof(copy));
    copy[sizeof(copy) - 1] = '\0';

    segment = strtok(copy, delimiter);

    int currentIndex = 1;
    while (segment != NULL) {
        if (currentIndex == targetIndex) {
            return strdup(segment);
        }
        segment = strtok(NULL, delimiter);
        currentIndex++;
    }

    return NULL; 
}

char *extractSegmentRange(const char *source, const char *delimiter, int start, int end) 
{
    char copy[MAX_STRING_LENGTH];
    char *segment;

    strncpy(copy, source, sizeof(copy));
    copy[sizeof(copy) - 1] = '\0';

    segment = strtok(copy, delimiter);

    int totalSegments = 0;
    while (segment != NULL) {
        totalSegments++;
        segment = strtok(NULL, delimiter);
    }

    start = (start >= 0) ? start : totalSegments + start + 1;
    end = (end >= 0) ? end : totalSegments + end + 1;

    if (start > totalSegments || start > end || start <= 0) {
        return NULL;
    }

    strncpy(copy, source, sizeof(copy));
    copy[sizeof(copy) - 1] = '\0';

    segment = strtok(copy, delimiter);

    int currentIndex = 1;
    char *result = NULL;
    while (segment != NULL) {
        if (currentIndex >= start && currentIndex <= end) {
            if (result == NULL) {
                result = strdup(segment);
            } else {
                result = realloc(result, strlen(result) + strlen(delimiter) + strlen(segment) + 1);
                strcat(result, delimiter);
                strcat(result, segment);
            }
        }
        if (currentIndex > end) {
            break; 
        }
        segment = strtok(NULL, delimiter);
        currentIndex++;
    }
    return result;
}


int getAtComIndex(const char *node, int index, char *result) 
{
    glob_t glob_result;

    char pattern[MAX_STRING_LENGTH];
    snprintf(pattern, sizeof(pattern), "/sys/bus/usb/devices/%s/%s%s/ttyUSB%s", node, node, "?*", "?*");

    if (glob(pattern, 0, NULL, &glob_result) != 0) {
        perror("glob");
        return EXIT_FAILURE;
    }

    if (index > 0 && index <= glob_result.gl_pathc) {
        snprintf(result, MAX_STRING_LENGTH, "/dev/%s", basename(glob_result.gl_pathv[index - 1]));
        globfree(&glob_result);

        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Index out of range.\n");
        globfree(&glob_result);
        return EXIT_FAILURE;
    }
}

int getIntrefacePath(const char *interface, char *result) 
{
    char sysFs[MAX_STRING_LENGTH] = {0};
    char targetPath[MAX_STRING_LENGTH] = {0};

    snprintf(sysFs, MAX_STRING_LENGTH, "/sys/class/net/%s", interface);
    ssize_t bytesRead = readlink(sysFs, targetPath, sizeof(targetPath) - 1);

    if (bytesRead == -1) {
        perror("readlink");
        return EXIT_FAILURE;
    } else {
        targetPath[bytesRead] = '\0';
        strcpy(result, "/sys/");
        strcat(result, targetPath + 6);
        return EXIT_SUCCESS;
    }
}

int getInterfaceDriver(const char *interface, char *result) 
{
    char uevent[MAX_STRING_LENGTH] = {0};
    sprintf(uevent, "/sys/class/net/%s/device/uevent", interface);

    FILE *fp = fopen(uevent, "r");

    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[MAX_STRING_LENGTH];

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "DRIVER=") != NULL) {
            char *equalSign = strchr(line, '=');
            if (equalSign != NULL) {
                equalSign++;
                char *newline = strchr(equalSign, '\n');
                if (newline != NULL) {
                    *newline = '\0';
                }
                strcpy(result, equalSign);
                fclose(fp);
                return EXIT_SUCCESS;
            }
        }
    }

    fclose(fp);
    result[0] = '\0';
    return EXIT_FAILURE;
}


int interfaceIsModem(const char *ifc_driver)
{
    const char *drivers[] = {"GobiNet", "qmi_wwan", "cdc_mbim", "sierra_net", "cdc_ether", "cdc_ncm"};

    size_t numDrivers = sizeof(drivers) / sizeof(drivers[0]);

    int matched = 0;
    for (size_t i = 0; i < numDrivers; ++i) {
        if (strcmp(drivers[i], ifc_driver) == 0) {
            matched = 1;
            break;
        }
    }

    return matched;
}