 * SMTP - Csmtp.h
 * name: Uriel Shani
 */

#include <stdio.h>          // Standard I/O functions
#include <string.h>         // String manipulation functions
#include <stdlib.h>         // Standard library functions
#include <sys/time.h>       // Time structures for select()
#include <sys/types.h>      // Primitive system data types
#include <sys/socket.h>     // Socket definitions
#include <netinet/in.h>     // Internet address family
#include <arpa/inet.h>      // Functions for manipulating IP addresses
#include <netdb.h>          // Structures for host entry

#ifndef CSMTP_H_            // Include guard to prevent multiple inclusions
#define CSMTP_H_

// Function prototypes for SMTP client
char* getPostfix(char *header, char *postfix);         // Extracts postfix from server reply header
void readServer(int fd, char *line, int* lim, char *postfix); // Reads server response
int checkReply(char reply);                            // Checks server reply code
int getLine(int fd, char line[], int max);             // Gets a line of data from the socket

#endif /* CSMTP_H_ */       // End of include guard