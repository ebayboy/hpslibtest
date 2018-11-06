

/**
 * use test.json config
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "waf.h"
#include "cJSON.h"
#include "log.h"

static int test_load_config(const char *filename, void **data)
{
    long temp_len = 0;
    FILE *fp = NULL;
    char *temp = NULL;
    cJSON *root = NULL, *it = NULL;
    int ret = 0;

    if (filename == NULL || strlen(filename) == 0) {
        return -1;
    }

    if ((fp = fopen(filename, "r")) == NULL) {
        return -1;
    }

    if ((temp_len = fsize(fp)) == -1) {
        return -1;
    }

    log_info("temp_len:%d\n", temp_len);
    if ((temp = (char*)malloc(temp_len + 1)) == NULL) {
        ret = -1;
        goto out;
    }
    memset(temp, 0, sizeof(temp));

    if (fread(temp, temp_len, 1, fp) != 1) {
        ret = -1;
        goto out;
    }

    log_info("file content:[%s]\n", temp);
    if ((root = cJSON_Parse(temp)) == NULL) {
        ret = -1;
        goto out;
    }

    char *args = NULL, *uri = NULL, *request_body = NULL, *cookies = NULL;

    if ((it = cJSON_GetObjectItem(root,"args")) != NULL) {
        args = strdup(it->valuestring);
    }

    if ((it = cJSON_GetObjectItem(root,"uri")) != NULL) {
        uri = strdup(it->valuestring);
    }
 
    if ((it = cJSON_GetObjectItem(root,"cookies")) != NULL) {
        cookies = strdup(it->valuestring);
    }
  
    if ((it = cJSON_GetObjectItem(root,"request_body")) != NULL) {
        request_body = strdup(it->valuestring);
    }

    *data = waf_data_create(HTTP_GET, uri, strlen(uri), 
            args, strlen(args), cookies, strlen(cookies), 
            request_body, strlen(request_body));
    if (*data == NULL) {
        return -1;
    }

    /* load header */


out:

    if (args) {
        free(args);
    }

    if (uri) {
        free(uri);
    }

    if (request_body) {
        free(request_body);
    }

    if (cookies) {
        free(cookies);
    }

    if (fp) {
        fclose(fp);
    }

    if (root) {
        cJSON_Delete(root);
    }

    if (temp) {
        free(temp);
    }
    
    return ret;
}


int main()
{
    char *filename="hpslib.json";
    char *testconf="test.json";
    char *log_file="/var/log/waf_test.log";
    int rc = 0;

    /*set log */
    FILE *log_fp = NULL;


    if ((log_fp = fopen(log_file, "a+")) == NULL) {
        fprintf(stderr, "open log file [%s] error.\n", log_file);
        return -1;
    }

    log_set_fp(log_fp);

    /* 1. init */
    if (waf_init("/var/log/waf.log", "hpslib.json") == -1) {
        fprintf(stderr, "Error: init\n");
        return -1;
    }

    /* 2. show */
    waf_show();

    /* load test.json */
    char *args = "";
    char *uri = "";
    char *request_body = "";
    char *cookies = "";

    void *data;
        
    if (test_load_config(testconf, &data) == -1) {
        rc = -1;
        goto out;
    }

#if 0
        char *tmp = "tmp12\%203abc";
    if (waf_data_add_param(data,
            PARAM_HDR_TYPE,
            "tmp", strlen(tmp),
            tmp , strlen(tmp)) == -1) {
        fprintf(stderr, "Error: waf_data_add_param ua error.");
        goto out;
    }

    /* add header */
    char *ua = "1122\%20334455";
    if (waf_data_add_param(data,
            PARAM_HDR_TYPE,
            WAF_HDR_UA, strlen(WAF_HDR_UA),
            ua , strlen(ua)) == -1) {
        fprintf(stderr, "Error: waf_data_add_param ua error.");
        goto out;
    }

    //char *referer = "referer-/etc/passwd445566";
    char *referer = "referer";
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
            PARAM_MZ_TYPE,
            referer_var, strlen(referer_var), 
            WAF_HDR_REFERER, strlen(WAF_HDR_REFERER)) == -1) {
        fprintf(stderr, "Error: waf_data_add_param referer var error.");
        goto out;
    }

    char *ua_var = "$u_user_agent";
    if (waf_data_add_param(data,
                PARAM_MZ_TYPE,
                ua_var, strlen(ua_var), 
                WAF_HDR_UA, strlen(WAF_HDR_UA)) == -1) {
        fprintf(stderr, "Error: waf_data_add_param referer var error.");
        goto out;
    }
#endif

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
    if (log_fp) {
        fclose(log_fp);
    }

    return 0;
}
