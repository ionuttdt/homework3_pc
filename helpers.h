#ifndef _HELPERS_
#define _HELPERS_

#define BUFLEN 4096
#define LINELEN 1000
#define IP_SERVER "185.118.200.35"
#define PORT_SERVER 8081

void error(const char *msg);
void compute_message(char *message, char *line);
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);
void close_connection(int sockfd);
void send_to_server(int sockfd, char *message);
char *receive_from_server(int sockfd);
char *concat(const char *s1, const char *s2) ;
char *format(char *username, char *username_, char *password, char *password_);

#endif