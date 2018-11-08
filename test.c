

/**
 * use test.json config
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "waf.h"
#include "cJSON.h"
#include "log.h"
#include "list.h"

typedef struct
{
    list_head_t list;
    char *key;
    char *value;
} test_config_node_t;

typedef struct
{
    char *method;
    char *uri;
    char *args;
    char *cookies;
    char *request_body;

    list_head_t headers_head;
    list_head_t vars_head;
    list_head_t mzs_head;
} test_config_t;

    static void *
test_config_create (void)
{
    test_config_t *new = NULL;

    if ((new = malloc (sizeof (test_config_t))) == NULL)
    {
        log_error ("Error: malloc");
        return NULL;
    }

    memset (new, 0, sizeof (test_config_t));

    INIT_LIST_HEAD (&(new->headers_head));
    INIT_LIST_HEAD (&(new->vars_head));
    INIT_LIST_HEAD (&(new->mzs_head));

    return new;
}

    static void
test_config_free_list (list_head_t * head, const char *name)
{
    test_config_node_t *node, *tmp;
    //log_info("free name[%s]", name);

    list_for_each_entry_safe (node, tmp, head, list)
    {
        list_del (&node->list);
        if (node->key)
        {
            free (node->key);
        }
        if (node->value)
        {
            free (node->value);
        }
        free (node);
    }
}

    static void
test_config_free (test_config_t * cfg)
{
    if (cfg == NULL)
    {
        return;
    }

#define CFG_FREE(x) \
    if (cfg->x) {  \
        free(cfg->x);  \
    }

    CFG_FREE (method);
    CFG_FREE (args);
    CFG_FREE (uri);
    CFG_FREE (request_body);

#undef CFG_FREE

    /* free headers & vars & mzs */
    test_config_free_list (&cfg->headers_head, "headers");
    test_config_free_list (&cfg->vars_head, "vars");
    test_config_free_list (&cfg->mzs_head, "mzs");
}

    static void
test_config_show_list (list_head_t * head, const char *name)
{
    test_config_node_t *node;

    log_info ("%s:", name);
    list_for_each_entry (node, head, list)
    {
        log_info ("key:[%s] value:[%s]", node->key, node->value);
    }
}

    static void
test_config_show (test_config_t * cfg)
{
    if (cfg == NULL)
    {
        return;
    }

#define CFG_PRT(x) \
    if (cfg->x) {   \
        log_info("method:[%s]", cfg->x);   \
    }

    CFG_PRT (method);
    CFG_PRT (args);
    CFG_PRT (uri);
    CFG_PRT (request_body);

#undef CFG_PRT

    /* TODO show headers & vars & mzs */
    test_config_show_list (&cfg->headers_head, "headers");
    test_config_show_list (&cfg->vars_head, "vars");
    test_config_show_list (&cfg->mzs_head, "mzs");
}

    static int
test_get_method (const char *method)
{
    if (strcasecmp (method, "HTTP_GET") == 0)
    {
        return HTTP_GET;
    }
    else if (strcasecmp (method, "HTTP_HEAD") == 0)
    {
        return HTTP_HEAD;
    }
    else if (strcasecmp (method, "HTTP_POST") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_PUT") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_DELETE") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_MKCOL") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_COPY") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_MOVE") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_OPTIONS") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_PROPFIND") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_PROPPATCH") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_LOCK") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_UNLOCK") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_PATCH") == 0)
    {

    }
    else if (strcasecmp (method, "HTTP_TRACE") == 0)
    {

    }
    else
    {
        return HTTP_UNKNOWN;
    }

    return HTTP_UNKNOWN;
}

    static int
test_load_config_list (cJSON * root, const char *item_name,
        list_head_t * head)
{
    /* load header */
    cJSON *headers, *header, *hdr_key, *hdr_value;
    test_config_node_t *node;
    int i, size = 0;

    if (root == NULL || item_name == NULL || head == NULL)
    {
        log_error ("input check error!");
        return -1;
    }

    if ((headers = cJSON_GetObjectItem (root, item_name)) == NULL)
    {
        log_info ("headers null, continue");
        return 0;
    }

    size = cJSON_GetArraySize (headers);
    for (i = 0; i < size; i++)
    {
        if ((header = cJSON_GetArrayItem (headers, i)) == NULL)
        {
            continue;
        }
        if ((hdr_key = cJSON_GetObjectItem (header, "key")) == NULL)
        {
            continue;
        }
        if ((hdr_value = cJSON_GetObjectItem (header, "value")) == NULL)
        {
            continue;
        }

        if ((node = malloc (sizeof (test_config_node_t))) == NULL)
        {
            log_error ("malloc test_config_node_t failed!");
            return -1;
        }
        memset (node, 0, sizeof (test_config_node_t));

        if (hdr_key)
        {
            node->key = strdup (hdr_key->valuestring);
        }
        if (hdr_value)
        {
            node->value = strdup (hdr_value->valuestring);
        }

#if 0
        list_head_t head1;
        INIT_LIST_HEAD (&head1);
#endif

        list_add_tail (&node->list, head);
        log_info ("key:%s value:%s", node->key, node->value);
    }

    return 0;
}

    static int
test_load_config_list_add_param (list_head_t * head, int type, void *data)
{
    test_config_node_t *node;

    list_for_each_entry (node, head, list)
    {
        if (waf_data_add_param (data,
                    type,
                    node->key, strlen (node->key),
                    node->value, strlen (node->value)) == -1)
        {
            log_error ("Error: waf_data_add_param ua error.");
            return -1;
        }
    }

    return 0;
}

    static int
test_load_config (test_config_t * cfg, const char *config, void **data)
{
    long temp_len = 0;
    FILE *fp = NULL;
    char *temp = NULL;
    cJSON *root = NULL, *it = NULL, *it_sub;
    int ret = 0, size = 0, i;

    if (config == NULL || strlen (config) == 0)
    {
        return -1;
    }

    if ((fp = fopen (config, "r")) == NULL)
    {
        return -1;
    }

    if ((temp_len = fsize (fp)) == -1)
    {
        return -1;
    }

    log_info ("temp_len:%d\n", temp_len);
    if ((temp = (char *) malloc (temp_len + 1)) == NULL)
    {
        ret = -1;
        goto out;
    }
    memset (temp, 0, sizeof (temp));

    if (fread (temp, temp_len, 1, fp) != 1)
    {
        ret = -1;
        goto out;
    }

    log_info ("file content:[%s]\n", temp);
    if ((root = cJSON_Parse (temp)) == NULL)
    {
        ret = -1;
        goto out;
    }

    if ((it = cJSON_GetObjectItem (root, "method")) != NULL)
    {
        cfg->method = strdup (it->valuestring);
    }

    if ((it = cJSON_GetObjectItem (root, "args")) != NULL)
    {
        cfg->args = strdup (it->valuestring);
    }

    if ((it = cJSON_GetObjectItem (root, "uri")) != NULL)
    {
        cfg->uri = strdup (it->valuestring);
    }

    if ((it = cJSON_GetObjectItem (root, "cookies")) != NULL)
    {
        cfg->cookies = strdup (it->valuestring);
    }

    if ((it = cJSON_GetObjectItem (root, "request_body")) != NULL)
    {
        cfg->request_body = strdup (it->valuestring);
    }

    /* TODO */
    /* load header */
    if (test_load_config_list (root, "headers", &cfg->headers_head) == -1)
    {
        log_error ("test_load_config_list headers");
        return -1;
    }
    if (test_load_config_list (root, "vars", &cfg->vars_head) == -1)
    {
        log_error ("test_load_config_list vars");
        return -1;
    }
    if (test_load_config_list (root, "mzs", &cfg->mzs_head) == -1)
    {
        log_error ("test_load_config_list mzs");
        return -1;
    }

    /* create data */
    *data =
        waf_data_create (test_get_method (cfg->method), cfg->uri,
                strlen (cfg->uri), cfg->args, strlen (cfg->args),
                cfg->cookies, strlen (cfg->cookies), cfg->request_body,
                strlen (cfg->request_body));
    if (*data == NULL)
    {
        return -1;
    }
    /* add param to data */
    if (test_load_config_list_add_param
            (&cfg->headers_head, PARAM_HDR_TYPE, *data) == -1)
    {
        log_error ("test_load_config_list_add_param headers");
        return -1;
    }
    if (test_load_config_list_add_param (&cfg->vars_head, PARAM_VAR_TYPE, *data)
            == -1)
    {
        log_error ("test_load_config_list_add_param vars");
        return -1;
    }
    if (test_load_config_list_add_param (&cfg->mzs_head, PARAM_MZ_TYPE, *data)
            == -1)
    {
        log_error ("test_load_config_list_add_param mzs");
        return -1;
    }

out:

    if (fp)
    {
        fclose (fp);
    }

    if (root)
    {
        cJSON_Delete (root);
    }

    if (temp)
    {
        free (temp);
    }

    return ret;
}

int get_option (
        int argc, 
        char **argv,
        char *config,
        char *log,
        char *test)
{
    char *cmd = argv[0];

    while (1) {
        int option_index = 0;
        struct option long_options[] = {
            {"config", 1, 0, 'c'},
            {"log", 1, 0, 'l'},
            {"test", 1, 0, 't'},
            {"help", 0, 0, 'h'}	//0代表没有参数
        };
        int c;

        //注意这里的冒号，有冒号就需要加参数值，没有冒号就不用加参数值
        c = getopt_long (argc, argv, "c:l:t:h", long_options, &option_index);	
        if (c == -1)
            break;

        switch (c) {
            case 'c':
                printf ("Input config file:[%s]\n", optarg);
                if (strlen(optarg) > 4096) {
                    log_error("max len 4095");
                    return -1;
                }
                strcpy(config, optarg);
                break;

            case 'l':
                printf ("Input log file:[%s]\n", optarg);
                if (strlen(optarg) > 4096) {
                    log_error("max len 4095");
                    return -1;
                }
                strcpy(log, optarg);
                break;

            case 't':
                printf ("Input test file:[%s]\n", optarg);
                strcpy(test, optarg);
                break;

            case 'h':
            default:
                printf ("-c lib config file\n-l log file\n-t test config file\n");
                return -1;
        }
    }

    return 0;
}

    int
main (int argc, char **argv)
{
    char config[1024] = {0};
    char log[1024] = {0};
    char test[1024] = {0};
    int rc = 0;
    FILE *log_fp = NULL;

    if (get_option (argc, argv, config, log, test) == -1) {
        return 0;
    }

    if (strlen(config) == 0) {
        strcpy(config, "hpslib.json");
    }
    if (strlen(log) == 0) {
        strcpy(log, "/var/log/waf_test.log");
    }
    if (strlen(test) == 0) {
        strcpy(test, "test.json");
    }

    log_info ("lib config file:[%s]", config);
    log_info ("test config file:[%s]", test);
    log_info ("log file:[%s]", log);

    /* init log */
    if ((log_fp = fopen (log, "a+")) == NULL)
    {
        log_info ("open log file [%s] error.\n", log);
        return -1;
    }
    log_set_fp (log_fp);

    /* waf init */
    if (waf_init ("/var/log/waf.log", "hpslib.json") == -1)
    {
        log_info ("Error: init");
        return -1;
    }

    /* waf show */
    waf_show ();

    /* load test.json */
    void *data = NULL;
    test_config_t *cfg = NULL;
    int matched_rule_id = 0;

    if ((cfg = test_config_create ()) == NULL)
    {
        rc = -1;
        goto out;
    }

    if (test_load_config (cfg, test, &data) == -1)
    {
        rc = -1;
        goto out;
    }

    test_config_show (cfg);

    /* waf data show */
    waf_data_show (data);

    /* waf match */
    rc = waf_match (data, &matched_rule_id);
    log_info ("Match result : rc=%d matched_rule_id:%d\n", rc, matched_rule_id);

out:

    /* 4. waf_fini */
    waf_fini ();
    if (log_fp)
    {
        fclose (log_fp);
    }

    if (cfg)
    {
        test_config_free (cfg);
    }

    if (data)
    {
        waf_data_destroy (data);
    }

    return 0;
}
