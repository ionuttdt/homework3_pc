/*
Duta Viorel-Ionut, 321CB
Tema 3 PC
*/
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *url_params)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    /*
        PAS 1: Scriem numele metodei, calea, parametri din url (daca exista) si tipul protocolului
    */
    if (url_params != NULL)
    {
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    }
    else
    {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);
    memset(line, 0, LINELEN);
    sprintf(line, "HOST: %s", host);
    compute_message(message, line);

    memset(line, 0, LINELEN);
    compute_message(message, line);
    memset(line, 0, LINELEN);

    return message;
}

char *compute_get_request2(char *host, char *url, char *url_params, char * cook, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    /*
        PAS 1: Scriem numele metodei, calea, parametri din url (daca exista) si tipul protocolului
    */
    if (url_params != NULL)
    {
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    }
    else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);
    memset(line, 0, LINELEN);

    sprintf(line, "HOST: %s", host);
    compute_message(message, line);
    memset(line, 0, LINELEN);

    if(token != NULL) {
        sprintf(line, "Authorization: Basic %s", token);
        compute_message(message, line);
        memset(line, 0, LINELEN);
    }

    if(cook != NULL) {
        sprintf(line, "COOKIE: %s", cook);
        compute_message(message, line);
        memset(line, 0, LINELEN);
    }

    memset(line, 0, LINELEN);
    compute_message(message, line);


    return message;
}

char *compute_post_request(char *host, char *url, char *form_data)
{

    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    memset(line, 0, LINELEN);
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    memset(line, 0, LINELEN);

    sprintf(line, "HOST: %s", host);
    compute_message(message, line);
    memset(line, 0, LINELEN);

    sprintf(line, "Content-Type: application/x-www-form-urlencoded");
    compute_message(message, line);
    memset(line, 0, LINELEN);

    sprintf(line, "Content-Length: %lu", strlen(form_data));
    compute_message(message, line);
    memset(line, 0, LINELEN);

    memset(line, 0, LINELEN);
    compute_message(message, line);


    compute_message(message, form_data);
    memset(line, 0, LINELEN);
    compute_message(message, line);

    return message;
}

char *compute_post_request2(char *host, char *url, char *form_data, char *content, char * cook, char * token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    memset(line, 0, LINELEN);

    sprintf(line, "HOST: %s", host);
    compute_message(message, line);
    memset(line, 0, LINELEN);

    sprintf(line,"Content-Type: %s",content) ;
    compute_message(message, line);
    memset(line, 0, LINELEN);

    sprintf(line, "Content-Length: %lu", strlen(form_data));
    compute_message(message, line);
    memset(line, 0, LINELEN);

    if(token != NULL) {
        sprintf(line, "Authorization: Basic %s", token);
        compute_message(message, line);
        memset(line, 0, LINELEN);
    }

    sprintf(line, "Cookie: %s", cook);
    compute_message(message, line);
    memset(line, 0, LINELEN);

    compute_message(message, line);
    memset(line, 0, LINELEN);

    memset(line, 0, LINELEN);


    sprintf(line, "%s", form_data);
    compute_message(message, line);
    memset(line, 0, LINELEN);

    return message;
}