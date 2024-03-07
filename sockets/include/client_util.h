#ifndef CLIENT_UTIL_H
#define CLIENT_UTIL_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

void send_tcp_c(int, char *);
void recv_tcp_c(int, char *);
int conn_c(char *, int);
void send_udp_c(int, char *, struct sockaddr_in *);
void recv_udp_c(int, char *, struct sockaddr_in *);

#endif