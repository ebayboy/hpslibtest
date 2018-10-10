#include <stdio.h>

#include "hpslib.h"
#include "cfg_parser.h"

int main(int argc, char **args)
{
    printf("hello world!\n");

    libtest();

    cfg_parser_parse();

    return 0;
}
