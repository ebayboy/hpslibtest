

/**
 * use test.json config
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "waf.h"
#include "cJSON.h"
#include "log.h"
#include "list.h"


typedef struct {
    list_head_t list;
    char *key;
    char *value;
} test_config_node_t;

typedef struct {
    char *method;
    char *uri;
    char *args;
    char *cookies;
    char *request_body;

    list_head_t headers_head;
    list_head_t vars_head;
    list_head_t mzs_head;
} test_config_t;

static void *test_config_create(void)
{
    test_config_t *new = NULL;

    if ((new = malloc(sizeof(test_config_t))) == NULL) {
        log_error("Error: malloc");
        return NULL;
    }

    memset(new, 0, sizeof(test_config_t));

    INIT_LIST_HEAD(&(new->headers_head));
    INIT_LIST_HEAD(&(new->vars_head));
    INIT_LIST_HEAD(&(new->mzs_head));

    return new;
}

static void test_config_free_list(list_head_t *head, const char *name)
{
    test_config_node_t *node, *tmp;
    log_info("free name[%s]", name);

    list_for_each_entry_safe(node, tmp, head, list) {
        list_del(&node->list); 
        if (node->key) {
            free(node->key);
        }
        if (node->value) {
            free(node->value);
        }
        free(node);
    }
}

static void test_config_free(test_config_t *cfg)
{
    if (cfg == NULL) {
        return ;
    }

#define CFG_FREE(x) \
    if (cfg->x) {  \
        free(cfg->x);  \
    }

    CFG_FREE(method);
    CFG_FREE(args);
    CFG_FREE(uri);
    CFG_FREE(request_body);

#undef CFG_FREE

    /* free headers & vars & mzs */
    test_config_free_list(&cfg->headers_head, "headers");
    test_config_free_list(&cfg->vars_head, "vars");
    test_config_free_list(&cfg->mzs_head, "mzs");
}

static void test_config_show_list(list_head_t *head, const char *name)
{
    test_config_node_t *node;

    log_info("%s:", name);
    list_for_each_entry(node, head, list) {
        log_info("key:[%s] value:[%s]", node->key, node->value);
    }
}

static void test_config_show(test_config_t *cfg)
{
    if (cfg == NULL) {
        return;
    }

#define CFG_PRT(x) \
    if (cfg->x) {   \
        log_info("method:[%s]", cfg->x);   \
    }

    CFG_PRT(method);
    CFG_PRT(args);
    CFG_PRT(uri);
    CFG_PRT(request_body);

#undef CFG_PRT

    /* TODO show headers & vars & mzs */
    test_config_show_list(&cfg->headers_head, "headers");
    test_config_show_list(&cfg->vars_head, "vars");
    test_config_show_list(&cfg->mzs_head, "mzs");
 }

static int test_get_method(const char *method)
{
    if (strcasecmp(method, "HTTP_GET") == 0) {
        return HTTP_GET;
    } else if (strcasecmp(method, "HTTP_HEAD") == 0) {
        return HTTP_HEAD; 
    } else if (strcasecmp(method, "HTTP_POST") == 0) {

    } else if (strcasecmp(method, "HTTP_PUT") == 0) {

    } else if (strcasecmp(method, "HTTP_DELETE") == 0) {

    } else if (strcasecmp(method, "HTTP_MKCOL") == 0) {

    } else if (strcasecmp(method, "HTTP_COPY") == 0) {

    } else if (strcasecmp(method, "HTTP_MOVE") == 0) {

    } else if (strcasecmp(method, "HTTP_OPTIONS") == 0) {

    } else if (strcasecmp(method, "HTTP_PROPFIND") == 0) {

    } else if (strcasecmp(method, "HTTP_PROPPATCH") == 0) {

    } else if (strcasecmp(method, "HTTP_LOCK") == 0) {

    } else if (strcasecmp(method, "HTTP_UNLOCK") == 0) {

    } else if (strcasecmp(method, "HTTP_PATCH") == 0) {

    } else if (strcasecmp(method, "HTTP_TRACE") == 0) {

    } else {
        return HTTP_UNKNOWN;
    }

    return HTTP_UNKNOWN;
}

static int test_load_config_list(cJSON *root, const char *item_name, list_head_t *head)
{
    /* load header */
    cJSON *headers, *header, *hdr_key, *hdr_value;
    test_config_node_t *node;
    int i, size = 0;

    if (root == NULL || item_name == NULL || head == NULL) {
        log_error("input check error!");
        return -1;
    }

    if (list_empty(head)) {
        log_info("[%s] is empty!", item_name);
    }

    if ((headers = cJSON_GetObjectItem(root,item_name)) == NULL) {
        log_info("headers null, continue");
        return 0;
    }

    size = cJSON_GetArraySize(headers);
    for (i = 0; i < size; i++) {
        if ((header  = cJSON_GetArrayItem(headers, i)) == NULL) {
            continue;
        }
        if ((hdr_key = cJSON_GetObjectItem(header, "key")) == NULL) {
            continue;
        }
        if ((hdr_value = cJSON_GetObjectItem(header, "value")) == NULL) {
            continue;
        }

        if ((node = malloc(sizeof(test_config_node_t))) == NULL) {
            log_error("malloc test_config_node_t failed!"); 
            return -1;
        }
        memset(node, 0, sizeof(test_config_node_t));

        if (hdr_key) {
            node->key = strdup(hdr_key->valuestring);
        }
        if (hdr_value) {
            node->value = strdup(hdr_value->valuestring);
        }

#if 0
        list_head_t head1;
        INIT_LIST_HEAD(&head1);
#endif

        list_add_tail(&node->list, head);
        log_info("key:%s value:%s", node->key, node->value);
    }  

    return 0;
}

static int test_load_config_list_add_param(list_head_t *head,int type, void *data)
{
    test_config_node_t *node;

    list_for_each_entry(node, head, list) {
        if (waf_data_add_param(data,
                    type, 
                    node->key, strlen(node->key),
                    node->value, strlen(node->value)) == -1) {
            log_error("Error: waf_data_add_param ua error.");
            return -1;
        }
    }

    return 0;
}

static int test_load_config(test_config_t *cfg, const char *filename, void **data)
{
    long temp_len = 0;
    FILE *fp = NULL;
    char *temp = NULL;
    cJSON *root = NULL, *it = NULL, *it_sub;
    int ret = 0, size = 0, i;

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

    if ((it = cJSON_GetObjectItem(root,"method")) != NULL) {
        cfg->method = strdup(it->valuestring);
    }

    if ((it = cJSON_GetObjectItem(root,"args")) != NULL) {
        cfg->args = strdup(it->valuestring);
    }

    if ((it = cJSON_GetObjectItem(root,"uri")) != NULL) {
        cfg->uri = strdup(it->valuestring);
    }
 
    if ((it = cJSON_GetObjectItem(root,"cookies")) != NULL) {
        cfg->cookies = strdup(it->valuestring);
    }
  
    if ((it = cJSON_GetObjectItem(root,"request_body")) != NULL) {
        cfg->request_body = strdup(it->valuestring);
    }

    /* TODO */
    /* load header */
    if (test_load_config_list(root, "headers", &cfg->headers_head) == -1) {
        log_error("test_load_config_list headers");
        return -1;
    }
    if (test_load_config_list(root, "vars", &cfg->vars_head) == -1) {
        log_error("test_load_config_list vars");
        return -1;
    }
    if (test_load_config_list(root, "mzs", &cfg->mzs_head) == -1) {
        log_error("test_load_config_list mzs");
        return -1;
    }

    /* create data */
    *data = waf_data_create(test_get_method(
                cfg->method), cfg->uri, strlen(cfg->uri), 
            cfg->args, strlen(cfg->args), 
            cfg->cookies, strlen(cfg->cookies), 
            cfg->request_body, strlen(cfg->request_body));
    if (*data == NULL) {
        return -1;
    }
   /* add param to data */
    if (test_load_config_list_add_param(&cfg->headers_head, PARAM_HDR_TYPE, *data) == -1) {
        log_error("test_load_config_list_add_param headers");
        return -1;
    }
    if (test_load_config_list_add_param(&cfg->vars_head, PARAM_VAR_TYPE, *data) == -1) {
        log_error("test_load_config_list_add_param vars");
        return -1;
    }
    if (test_load_config_list_add_param(&cfg->mzs_head, PARAM_MZ_TYPE, *data) == -1) {
        log_error("test_load_config_list_add_param mzs");
        return -1;
    }

    /* load mzs */
    test_config_show(cfg);

 out:

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

    test_config_t *cfg;

    if ((cfg = test_config_create()) == NULL) {
        rc = -1;
        goto out;
    }
        
    if (test_load_config(cfg, testconf, &data) == -1) {
        rc = -1;
        goto out;
    }

    test_config_show(cfg);

    if (data == NULL) {
        log_error("Error: data is NULL!");
        goto out;
    }


    /* TODO */
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

    test_config_free(cfg);

    return 0;
}
