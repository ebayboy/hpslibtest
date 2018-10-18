#include <stdio.h>

#include "hpslib.h"
#include "cfg_parser.h"

int main(int argc, char **args)
{
    printf("hello world!\n");
    char *filename="hpslib.json";

    libtest();

    int rc = cfg_parser_parse(filename);

    printf("rc=%d\n", rc);

    return 0;
}
