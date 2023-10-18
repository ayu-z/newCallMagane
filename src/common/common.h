#ifndef __COMMON_H__
#define __COMMON_H__


#define MAX_STRING_LENGTH 512


char *extractSegment(const char *source, const char *delimiter, int index);
char *extractSegmentRange(const char *source, const char *delimiter, int start, int end);
int getAtComIndex(const char *node, int index, char *result);
int getIntrefacePath(const char *interface, char *result);
int getInterfaceDriver(const char *interface, char *result);
int interfaceIsModem(const char *ifc_driver);

#endif // !__COMMH
