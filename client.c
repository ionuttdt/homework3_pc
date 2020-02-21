/*
Duta Viorel-Ionut, 321CB
Tema 3 PC
*/
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <json-c/json.h>
#include "requests.h"
#include "helpers.h"

int main(int argc, char *argv[])
{
	char *message;
	char *response;
	int sockfd;

//================================ Task 1 ===================================

	sockfd = open_connection(IP_SERVER, PORT_SERVER,AF_INET, SOCK_STREAM,0 );	// deschidere conexiune
	message = compute_get_request(IP_SERVER,"/task1/start",NULL);	// construire cerere
	send_to_server(sockfd, message);								// trimitere mesaj care server
	response = receive_from_server(sockfd);							// raspunsul de la server
	close_connection(sockfd);										// inchidere conexiune
/*    printf("TASK 1:\n");
    printf("\n");
    printf("%s\n", response);*/

//================================= Task 2 ==================================

	char *tok ;
	tok = strtok(response, "\n");
	char **cook = malloc(sizeof(char *)*2);					// memorarea stringului citit cu strtok
	char *data_tok = calloc(1000,sizeof(char));				// informatie din enunt
	char *method_ = calloc(10, sizeof(char));				// tipul metrodei
	cook[0] = calloc(500, sizeof(char));
	cook[1] = calloc(500, sizeof(char));
	tok = strtok(NULL, "\n");

	tok = strtok(NULL, "\n");								// citirea celor doua cookie
	memcpy(cook[0], tok, strlen(tok));
	tok = strtok(NULL, "\n");
	memcpy(cook[1], tok, strlen(tok));
	
	tok = strtok(NULL, "\n");								// parcurg informatiile de care nu am nevoie
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n"); // eununt

	struct json_object *parsed;
	struct json_object *enunt;
	struct json_object *url;
	struct json_object *method;
	struct json_object *type;
	struct json_object *data;

	// parsarea propriu-zisa
	parsed = json_tokener_parse(tok);
	json_object_object_get_ex(parsed, "enunt", &enunt);
	json_object_object_get_ex(parsed, "url", &url);
	json_object_object_get_ex(parsed, "method", &method);
	json_object_object_get_ex(parsed, "type", &type);
	json_object_object_get_ex(parsed, "data", &data);

	// transpun datele parsate in stringuri
	memcpy(data_tok, json_object_get_string(data), 1000);
	memcpy(method_, json_object_get_string(method), 10);

	// date pentru logare
	char *username = calloc(50, sizeof(char));
	char *username_ = calloc(50, sizeof(char));
	char *password = calloc(50, sizeof(char));
	char *password_ = calloc(50, sizeof(char));

	tok = strtok(data_tok, "\"{} :,");
	memcpy(username, tok, 50);
	tok = strtok(NULL, "\"{} :,");
	memcpy(username_, tok, 50);
	tok = strtok(NULL, "\"{} :,");
	memcpy(password, tok, 50);
	tok = strtok(NULL, "\"{} :,");
	memcpy(password_, tok, 50);

	// transform elementele parsate in char *
	char *url_2 = calloc(100,sizeof(char));
	memcpy(url_2, json_object_get_string(url), 100);
	char *type_2 = calloc(100, sizeof(char));
	memcpy(type_2, json_object_get_string(type), 100);
	char *cook_2 = calloc(500, sizeof(char));

	//cookie
	tok = strtok(cook[0], " ");
	tok = strtok(NULL, " ");
	memcpy(cook_2, tok, strlen(tok));
	tok = strtok(cook[1], " ");
	tok = strtok(NULL, " ;");
	strcat(cook_2, tok);

	// date pentru logare
	char *form_data = format(username, username_, password, password_);

	// apelul cererii
	if( strcmp(method_, "POST") == 0) {
		message = compute_post_request2(IP_SERVER, url_2, form_data, type_2, cook_2, NULL);
		sockfd = open_connection(IP_SERVER, PORT_SERVER,AF_INET, SOCK_STREAM,0 );
		send_to_server(sockfd, message);
		response = receive_from_server(sockfd);
		close_connection(sockfd);
/*		printf("\n");
		printf("TASK 2:\n");
		printf("\n");
		printf("%s\n", response);*/
	}

//================================== Task 3 ====================================
	tok = NULL;
	tok = strtok(response, "\n");
	tok = strtok(NULL, "\n");

	// construiesc cookie-urile
	tok = strtok(NULL, "\n");
	memset(cook[0], 0, 500);
	memcpy(cook[0], tok, strlen(tok));
	tok = strtok(NULL, "\n");
	memset(cook[1], 0, 500);
	memcpy(cook[1], tok, strlen(tok));

	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n"); //data

	// parsarea datelor din format json
	parsed = json_tokener_parse(tok);
	json_object_object_get_ex(parsed, "enunt", &enunt);
	json_object_object_get_ex(parsed, "url", &url);
	json_object_object_get_ex(parsed, "method", &method);
	json_object_object_get_ex(parsed, "data", &data);

	// sterg datele de la taskul 2
	memset(url_2, 0, 100);
	memset(method_, 0, 10);
	memset(data_tok, 0, 1000);

	// transformarea dateleor parsate in char*
	memcpy(url_2, json_object_get_string(url), 100);
	memcpy(method_, json_object_get_string(method), 10);
	memcpy(data_tok, json_object_get_string(data), 1000);

	// parsarea pentru token si id:
	parsed = json_tokener_parse(data_tok);
	struct json_object *token;
	struct json_object *queryParams;
	char *token_ = calloc(800, sizeof(char));
	char *queryParams_ = calloc(100, sizeof(char));
	json_object_object_get_ex(parsed, "token", &token);
	json_object_object_get_ex(parsed, "queryParams", &queryParams);

	memcpy(token_, json_object_get_string(token), 800);
	memcpy(queryParams_, json_object_get_string(queryParams), 100);

	// id pentru cererea get
	char *id_ = calloc(sizeof(queryParams), sizeof(char));
	tok = strtok(queryParams_, "\" :{}");
	strcat(id_, tok);
	strcat(id_, "=");
	tok = strtok(NULL, "\" :{}");
	strcat(id_, tok);

	// construiesc cookie
	memset(cook_2, 0, 500);
	tok = strtok(cook[0], " ");
	tok = strtok(NULL, " ");
	memcpy(cook_2, tok, strlen(tok));

	tok = strtok(cook[1], " ");
	tok = strtok(NULL, " ;");
	strcat(cook_2, tok);

	// raspunsurile la ghicitori
	char *url_param_ = calloc(200, sizeof(char));
	strcat(url_param_, "raspuns1=omul&raspuns2=numele&");
	strcat(url_param_, id_);

	// cererea
	if( strcmp(method_, "GET") == 0) {
		sockfd = open_connection(IP_SERVER, PORT_SERVER,AF_INET, SOCK_STREAM,0 );
		message = compute_get_request2(IP_SERVER, url_2, url_param_, cook_2, token_);
		send_to_server(sockfd, message);
		response = receive_from_server(sockfd);
		close_connection(sockfd);
/*		printf("\n");
		printf("TASK 3:\n");
		printf("\n");
		printf("%s\n", response);*/
	}

//================================== Task 4 ====================================

	tok = NULL;
	tok = strtok(response, "\n");
	tok = strtok(NULL, "\n");

	// cookie
	tok = strtok(NULL, "\n");
	memset(cook[0], 0, 500);
	memcpy(cook[0], tok, strlen(tok));
	tok = strtok(NULL, "\n");
	memset(cook[1], 0, 500);
	memcpy(cook[1], tok, strlen(tok));

	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n"); //data

	// parsarea datelor din format json
	parsed = json_tokener_parse(tok);
	json_object_object_get_ex(parsed, "enunt", &enunt);
	json_object_object_get_ex(parsed, "url", &url);
	json_object_object_get_ex(parsed, "method", &method);

	memset(url_2, 0, 100);					// stergerea datelor din url si method
	memset(method_, 0, 10);

	// trasnformarea datelor in char*
	memcpy(url_2, json_object_get_string(url), 100);
	memcpy(method_, json_object_get_string(method), 10);

	memset(cook_2, 0, 500);
	tok = strtok(cook[0], " ");
	tok = strtok(NULL, " ");
	memcpy(cook_2, tok, strlen(tok));

	tok = strtok(cook[1], " ");
	tok = strtok(NULL, " ;");
	strcat(cook_2, tok);

	// cererea la care am folsoit tokenul de la taskul anterior
	sockfd = open_connection(IP_SERVER, PORT_SERVER,AF_INET, SOCK_STREAM,0 );
	message = compute_get_request2(IP_SERVER, url_2, NULL, cook_2, token_);
	send_to_server(sockfd, message);
	response = receive_from_server(sockfd);
	close_connection(sockfd);
	/*printf("\n");
	printf("TASK 4:\n");
	printf("\n");
	printf("%s\n", response);*/

//================================== Task 5 ====================================

	tok = NULL;
	tok = strtok(response, "\n");
	tok = strtok(NULL, "\n");

	tok = strtok(NULL, "\n");
	memset(cook[0], 0, 500);
	memcpy(cook[0], tok, strlen(tok));
	tok = strtok(NULL, "\n");
	memset(cook[1], 0, 500);
	memcpy(cook[1], tok, strlen(tok));


	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n");
	tok = strtok(NULL, "\n"); //data

	parsed = json_tokener_parse(tok);
	json_object_object_get_ex(parsed, "enunt", &enunt);
	json_object_object_get_ex(parsed, "url", &url);
	json_object_object_get_ex(parsed, "method", &method);
	json_object_object_get_ex(parsed, "type", &type);
	json_object_object_get_ex(parsed, "data", &data);

	memset(cook_2, 0, 500);
	tok = strtok(cook[0], " ");
	tok = strtok(NULL, " ");
	memcpy(cook_2, tok, strlen(tok));
	tok = strtok(cook[1], " ");
	tok = strtok(NULL, " ;");
	strcat(cook_2, tok);

	memset(url_2, 0, 100);					// stergerea datelor din url si method
	memset(method_, 0, 10);
	memset(data_tok, 0, 1000);


	// trasnformarea datelor in char*
	memcpy(url_2, json_object_get_string(url), 100);
	memcpy(method_, json_object_get_string(method), 10);
	memcpy(data_tok, json_object_get_string(data), 1000);

	parsed = json_tokener_parse(data_tok);
	json_object_object_get_ex(parsed, "url", &enunt);
	json_object_object_get_ex(parsed, "url", &url);

	struct json_object *url5;
	char *url_5 = calloc(50, sizeof(char));

	json_object_object_get_ex(parsed, "url", &url5);
	json_object_object_get_ex(parsed, "queryParams", &queryParams);

	memcpy(queryParams_, json_object_get_string(queryParams), 100 );
	memcpy(url_5, json_object_get_string(url5), 50 );

	char hostbuffer[256];
	char *IPBuffer;
	struct hostent *host_e;

	tok = strtok(url_5, "/");
	memcpy(hostbuffer, tok, strlen(tok));
	tok = strtok(NULL, "");

	char *aux_url5 = calloc(100, sizeof(char) );
	strcat(aux_url5, "/");
	strcat(aux_url5, tok);

	//gethostname(hostbuffer, sizeof(hostbuffer));
	host_e = gethostbyname(hostbuffer);
	IPBuffer = inet_ntoa(*((struct in_addr*)host_e->h_addr_list[0] ));	// ip

	memset(url_param_, 0, 100);
	tok = strtok(queryParams_, "{} \",:");
	strcat(url_param_, tok);
	strcat(url_param_, "=");
	tok = strtok(NULL, "{} \",:");
	strcat(url_param_, tok);
	char * aux_urlparam = calloc(100, sizeof(char));
	tok = strtok(NULL, "{} \",:");
	strcat(aux_urlparam, tok);
	strcat(aux_urlparam, "=");
	tok = strtok(NULL, "{} \",:");
	strcat(aux_urlparam, tok);
	strcat(aux_urlparam, "&");
	strcat(aux_urlparam, url_param_);

	memset(type_2, 0, 100);
	memcpy(type_2, json_object_get_string(type), 100);

	sockfd = open_connection(IPBuffer, 80 ,AF_INET, SOCK_STREAM,0 );	// deschidere conexiune

	message = compute_get_request(hostbuffer ,aux_url5,aux_urlparam);	// construire cerere
	send_to_server(sockfd, message);								// trimitere mesaj care server
	response = receive_from_server(sockfd);							// raspunsul de la server
	close_connection(sockfd);
	//printf("\n");	
	//printf("%s\n", response);

	int w = 11;
	tok = strtok(response, "\n");
	while(w > 0) {
		tok = strtok(NULL, "\n");
		w--;
	}

	//compute_post_request2(char *host, char *url, char *form_data, char *content, char * cook);
	message = compute_post_request2(IP_SERVER, url_2, tok, type_2, cook_2, token_);
	sockfd = open_connection(IP_SERVER, PORT_SERVER,AF_INET, SOCK_STREAM,0 );
	send_to_server(sockfd, message);
	response = receive_from_server(sockfd);
	close_connection(sockfd);
	
	tok = strtok(response, "\n"); w = 7;
	while(w > 0) {
		tok = strtok(NULL, "\n");
		w--;
	}
	printf("%s\n",tok );

	free(message);
	return 0;
}