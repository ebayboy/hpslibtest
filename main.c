#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "waf_config.h"
#include "waf.h"
//#include "common.h"

int main()
{
    char *filename="hpslib.json";
    int rc = 0;

    /* 1. init */
    if (waf_init("/var/log/waf.log", "hpslib.json") == -1) {
        fprintf(stderr, "Error: init\n");
        return -1;
    }

    /* 2. show */
    waf_show();

    /* waf data */
    //char *uri = "http://192.168.137.200/?name=/etc/passwd";
    //char *args = "name=/etc/passwd&a=b";
    char *uri = "http://192.168.137.200/";
    char *args = "";
    char *request_body = NULL;
    void *data =  waf_data_create(HTTP_GET, uri, strlen(uri), args, strlen(args), request_body, 0);

    /* add header */
    char *ua = "user-agent-/etc/passwd112233";
    if (waf_data_add_param(data,
            PARAM_HDR_TYPE,
            WAF_HDR_UA, strlen(WAF_HDR_UA),
            ua , strlen(ua)) == -1) {
        fprintf(stderr, "Error: waf_data_add_param ua error.");
        goto out;
    }

    char *referer = "referer-/etc/passwd445566";
    if (waf_data_add_param(data,
            PARAM_HDR_TYPE,
            WAF_HDR_REFERER, strlen(WAF_HDR_REFERER),
            referer , strlen(referer)) == -1) {
        fprintf(stderr, "Error: waf_data_add_param referer error.");
        goto out;
    }

    /* add var */
    char *referer_var = "$http_referer";
    if (waf_data_add_param(data,
            PARAM_VAR_TYPE,
            referer_var, strlen(referer_var), 
            WAF_HDR_REFERER, strlen(WAF_HDR_REFERER)) == -1) {
        fprintf(stderr, "Error: waf_data_add_param referer var error.");
        goto out;
    }

    /* waf data show */
    waf_data_show(data);

    /* 3. waf_match */
    int matched_rule_id = 0;

    rc = waf_match(data, &matched_rule_id);
    printf("rc=%d matched_rule_id:%d\n", rc, matched_rule_id);

    if (data) {
        waf_data_destroy(data);
    }

out:

    /* 4. waf_fini */
    waf_fini();

    return 0;
}
