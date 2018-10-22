#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hpslib.h"
#include "cfg_parser.h"

int main(int argc, char **args)
{
    printf("hello world!\n");
    char *filename="hpslib.json";

    libtest(filename);

    int rc = 0;

    waf_t *waf = NULL;

    if ((waf = malloc(sizeof(waf_t))) == NULL) {
        return -1;
    }
    memset(waf, 0, sizeof(waf));

    rc = cfg_parser_parse(filename, waf);

    printf("rc=%d\n", rc);

    if (waf) {
        free(waf);
    }

    return 0;
}
