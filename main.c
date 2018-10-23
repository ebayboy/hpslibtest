#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hpslib.h"
#include "cfg_parser.h"
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

    wafcfg_t *wafcfg = NULL;

    if ((wafcfg = malloc(sizeof(wafcfg_t))) == NULL) {
        return -1;
    }
    memset(wafcfg, 0, sizeof(wafcfg));

    rc = cfg_parser_parse(filename, wafcfg);

    printf("rc=%d\n", rc);

    if (wafcfg) {
        free(wafcfg);
    }

    return 0;
}
