#ifndef __TCP_h__
#define __TCP_h__
#define SA struct sockaddr
int TCP_Open(char*,int);
int TCP_Server_Open(int);
int TCP_Accept(int);
int TCP_Write_String(int, char*);
int TCP_Read_String(int, char*,int);
int TCP_Close(int);
char *Get_IP(char *);
#endif