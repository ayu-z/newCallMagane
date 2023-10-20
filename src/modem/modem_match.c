#include <stdio.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>
#include <string.h>

typedef struct {
    int idVendor;
    int idProduct;
} UID_t;

typedef void (*cb_func)(void *);

void init(void *data) {
    printf("cb_func init: %s\n", (char *)data);
}

void dial(void *data) {
    printf("cb_func dial: %s\n", (char *)data);
}

typedef struct {
    char *name;
    int ATidx;
    cb_func init;
    cb_func dial;
} Modem_t;

typedef struct {
    UID_t uid;
    Modem_t modem;
} ModemInfo_t;

guint uid_hash(gconstpointer key) {
    const UID_t *hash_key = (const UID_t *)key;
    return g_int_hash(&hash_key->idVendor) ^ g_int_hash(&hash_key->idProduct);
}

int uid_equal(gconstpointer a, gconstpointer b) {
    const UID_t *uid_a = (const UID_t *)a;
    const UID_t *uid_b = (const UID_t *)b;
    return uid_a->idVendor == uid_b->idVendor && uid_a->idProduct == uid_b->idProduct;
}

GHashTable *hash_table;

void modem_destroy(gpointer value) {
    Modem_t *modem = (Modem_t *)value;
    free(modem->name);
    free(modem);
}

void modem_match_add(ModemInfo_t *modem_info) {
    Modem_t *value = malloc(sizeof(Modem_t));
    value->name = g_strdup(modem_info->modem.name);
    value->ATidx = modem_info->modem.ATidx;
    value->init = modem_info->modem.init;
    value->dial = modem_info->modem.dial;

    g_hash_table_insert(hash_table, &modem_info->uid, value);
}


void modem_init() {
    hash_table = g_hash_table_new_full(uid_hash, uid_equal, NULL, modem_destroy);
}


int main() {
    modem_init();

    ModemInfo_t modem_info = {
        .uid.idVendor = 0x1234,
        .uid.idProduct = 0x6789,
        .modem.name = "SRM821",
        .modem.ATidx = 2,
        .modem.init = init,
        .modem.dial = dial,
    };

    modem_match_add(&modem_info);

    UID_t search_key = {0x1234, 0x6789};
    Modem_t *found_value = g_hash_table_lookup(hash_table, &search_key);

    if (found_value != NULL) {
        printf("Name: %s\n", found_value->name);
        printf("AT IDX: %d\n", found_value->ATidx);
        found_value->init(NULL);
        found_value->dial(NULL);
    } else {
        printf("Value not found for the given key.\n");
    }

    g_hash_table_destroy(hash_table);

    return 0;
}