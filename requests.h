#ifndef _REQUESTS_
#define _REQUESTS_

char *compute_get_request(char *host, char *url, char *url_params);
char *compute_post_request(char *host, char *url, char *form_data);
char *compute_post_request2(char *host, char *url, char *form_data, char *content, char * cook, char *token);
char *compute_get_request2(char *host, char *url, char *url_params, char * cook, char *token);

#endif