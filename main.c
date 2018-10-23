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

    if (waf_init("/var/log/waf.log", "hpslib.json") == -1) {
        return -1;
    }

    waf_fini();

    return 0;
}
