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

    /* 1. init */
    if (waf_init("/var/log/waf.log", "hpslib.json") == -1) {
        return -1;
    }

    /* 2. show */
    waf_show();

    /* 3. waf_match */

    char *buff = "http://192.168.137.200/?name=/etc/passwd";
    int matched_rule_id = 0;
    rc = waf_match("$u_uri",
            buff, 
            strlen(buff), 
            &matched_rule_id);

    printf("rc=%d matched_rule_id:%d\n", rc, matched_rule_id);

    /* 4. waf_fini */
    waf_fini();

    return 0;
}
