#include <stdio.h>

#include "hpslib.h"
#include "cfg_parser.h"

int main(int argc, char **args)
{
    printf("hello world!\n");

    libtest();

    int rc = cfg_parser_parse("hpslib.json");

    printf("rc=%d\n", rc);

    return 0;
}
