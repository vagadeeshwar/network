#ifndef SERVER_UTIL_H
#define SERVER_UTIL_H

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

void recv_udp_s(int, char *, struct sockaddr_in *);
void send_udp_s(int, char *, struct sockaddr_in *);
void send_tcp_s(int, char *);
void recv_tcp_s(int, char *);
int sock_s(int);
int conn_s(int);
unsigned int checksum(const char *str);
char *getVRC(const char *);
char *getLRC(const char *);
void xor (char *, char *, int);
char *getCRC(const char *);

#endif