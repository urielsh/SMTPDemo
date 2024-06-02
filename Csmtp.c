/*SMTP - Csmtp.c
 * name: Uriel Shani
 */

#include "Csmtp.h"

#define OK 1
#define ERROR -1
#define BUFFER 128             // Buffer size definitions
#define LINE 256
#define MAX_BLOCK 512
#define MAX_REQUEST 1024
#define GAP " "                // Gap string for reply handling

char* reply;
char* postfix;
char header[LINE];             // Buffer for storing the header

int main(int argc, char **argv)
{
    char source[BUFFER];       // Buffer for source address
    char dst[BUFFER];          // Buffer for destination address
    int sockfd;
    struct sockaddr_in serv_addr;   // Server address structure
    char request[MAX_REQUEST+1];    // Buffer for requests
    char data[MAX_BLOCK];      // Buffer for data to be sent
    int MAX_RE = 2;            // Maximum retries

    unsigned int server_port = 25;  // Default SMTP port
    struct hostent *hostptr;        // Host entity structure
    struct in_addr *ptr;            // Address structure pointer
    unsigned short port_number;
    FILE* fp;

    // Check for correct number of command-line arguments
    if (argc < 3) {
        printf("Usage: %s smtpClient <%s> [port-number]\n", argv[0], argv[1]);
        return 1;
    }

    // Set port number
    if (argc < 4)
        port_number = 25;
    else
        port_number = atoi(argv[3]);

    // Get host information
    if ((hostptr = (struct hostent *) gethostbyname(argv[2])) == NULL) {
        perror("gethostbyname error for host");
        return 1;
    }

    // Open the file containing the source and destination addresses
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Read source and destination addresses from file
    fgets(source, BUFFER, fp);
    fgets(dst, BUFFER, fp);
    printf("%s\n%s\n", source, dst);

    // Set up the server address structure
    ptr = (struct in_addr *) *(hostptr->h_addr_list);
    printf("DEBUG: server address: %u %s\n", ptr->s_addr, inet_ntoa(*ptr));
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = ptr->s_addr;
    serv_addr.sin_port = htons(port_number);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("server: can't open stream socket");
        return 1;
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("client: can't connect to server");
        return 1;
    }

    // Start communication with the server
    bzero(header, LINE);
    getLine(sockfd, header, LINE);
    printf("\nS: %s", header);
    postfix = getPostfix(header, postfix);

    // Send HELO command
    bzero(request, MAX_REQUEST);
    sprintf(request, "HELO %s\r\n", argv[2]);
    if (checkReply(request[0]) == ERROR)
        exit(1);
    printf("C: %s", request);
    write(sockfd, request, strlen(request));

    bzero(header, LINE);
    getLine(sockfd, header, LINE);
    printf("S: %s\n", header);

    // Send MAIL FROM command
    bzero(request, MAX_REQUEST);
    sprintf(request, "MAIL FROM: %s\r\n", source);
    if (checkReply(request[0]) == ERROR)
        exit(1);
    printf("C: %s", request);
    write(sockfd, request, strlen(request));

    bzero(header, LINE);
    getLine(sockfd, header, LINE);
    printf("S: %s\n", header);

    // Send RCPT TO command
    bzero(request, MAX_REQUEST);
    sprintf(request, "RCPT TO: %s\r\n", dst);
    if (checkReply(request[0]) == ERROR)
        exit(1);
    printf("C: %s", request);
    write(sockfd, request, strlen(request));

    readServer(sockfd, reply, &MAX_RE, postfix);
    printf("S: %s\n", reply);
    free(reply);

    // Send DATA command
    bzero(request, MAX_REQUEST);
    sprintf(request, "DATA\r\n");
    if (checkReply(request[0]) == ERROR)
        exit(1);
    printf("C: %s", request);
    write(sockfd, request, strlen(request));

    bzero(header, LINE);
    getLine(sockfd, header, LINE - 1);
    printf("S: %s\r\n", header);

    // Send email content
    while (fgets(data, MAX_BLOCK, fp) != NULL) {
        bzero(request, MAX_REQUEST);
        sprintf(request, "%s\r\n", data);
        printf("C: %s", request);
        write(sockfd, request, strlen(request));
    }

    // End data with a single period
    bzero(request, MAX_REQUEST);
    sprintf(request, ".\r\n");
    printf("CLIENT: %s", request);
    write(sockfd, request, strlen(request));
    readServer(sockfd, reply, &MAX_RE, postfix);
    printf("SERVER: %s\n", reply);
    free(reply);

    // Send QUIT command
    bzero(request, MAX_REQUEST);
    sprintf(request, "QUIT\r\n");
    printf("CLIENT: %s", request);
    write(sockfd, request, strlen(request));
    readServer(sockfd, reply, &MAX_RE, postfix);
    printf("SERVER: %s\n", reply);
    free(reply);

    // Close socket and file
    close(sockfd);
    fclose(fp);

    return 0;
}

// Get a line of data from the socket
int getLine(int fd, char* line, int lim)
{
    int i = 0;
    char c;

    while (--lim > 0 && read(fd, &c, 1) > 0 && c != '\n' && c != '\0') {
        line[i] = c;
        i++;
    }
    if (c == '\n') {
        line[i] = c;
        i++;
    }

    line[i] = '\0';
    return i;
}

// Read the server response
void readServer(int fd, char *line, int *lim, char *postfix)
{
    int length = 3;
    char temp[20];
    int sum = 0;
    int i = 0;

    reply = malloc(sizeof(char) * length);

    bzero(reply, length);
    bzero(temp, 20);
    strcpy(reply, GAP);
    strcpy(temp, GAP);

    while (1) {
        bzero(temp, 20);
        strcpy(temp, GAP);
        i = read(fd, temp, 19);
        if (i == ERROR) {
            perror("cannot read from socket");
            return;
        }
        sum += i;

        if (sum < length) {
            strcat(reply, temp);
        } else {
            length = sum + 1;
            reply = (char*) realloc(reply, length - 1);
            strcat(reply, temp);
        }

        if (strstr(reply, postfix) != NULL)
            return;
    }
}

// Check the server reply code
int checkReply(char reply)
{
    switch ((int)reply) {
        case 2: goto ok;
        case 3: goto ok;
        case 4:
            perror("Bad Request Error!");
            break;
        case 5:
            perror("Server Error!");
            break;
        default:
            return ERROR;
    }
ok:
    return OK;
}

// Extract postfix from the server reply header
char* getPostfix(char* header, char* postfix)
{
    char *temp;

    temp = strstr(header, "-");
    temp = temp + 2;

    if (temp != NULL) {
        postfix = malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(postfix, temp);
        printf("\n%s", postfix);
    }
    return postfix;
}
