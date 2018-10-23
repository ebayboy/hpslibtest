#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hpslib.h"
#include "waf_config.h"
#include "waf.h"
#include "common.h"

int main(int argc, char **args)
{
    printf("hello world!\n");
    char *filename="hpslib.json";

    libtest(filename);

    int rc = 0;

    if (waf_init("/var/log/waf.log") == -1) {
        return -1;
    }

    waf_config_t *waf_config = NULL;

    if ((waf_config = malloc(sizeof(waf_config_t))) == NULL) {
        return -1;
    }
    memset(waf_config, 0, sizeof(waf_config));

    rc = waf_config_init(filename, waf_config);

    printf("rc=%d\n", rc);

    if (waf_config) {
        free(waf_config);
    }


    waf_fini();

    return 0;
}
