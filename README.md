# cSMTP

## Overview
cSMTP is a simple SMTP (Simple Mail Transfer Protocol) client written in C. It demonstrates the basics of connecting to an SMTP server, sending email commands, and transmitting an email message from a source to a destination address.

## Features
- Establishes a connection to an SMTP server
- Sends `HELO`, `MAIL FROM`, `RCPT TO`, and `DATA` commands
- Reads server responses
- Handles simple email sending with specified source and destination addresses

## Files
- `Csmtp.c`: The main program file containing the implementation of the SMTP client.
- `Csmtp.h`: The header file containing function prototypes and necessary includes.

## Prerequisites
To compile and run the cSMTP client, you need:
- A C compiler (e.g., `gcc`)
- A Unix-like operating system (Linux, macOS, etc.)
- Network access to an SMTP server

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/cSMTP.git
   ```
2. Navigate to the repository directory:
   ```sh
   cd cSMTP
   ```
3. Compile the code:
   ```sh
   gcc -o cSMTP Csmtp.c
   ```

## Usage
To run the cSMTP client, use the following command:
```sh
./cSMTP <file_with_addresses> <smtp_server> [port_number]
```
- `<file_with_addresses>`: A file containing the source and destination email addresses. The source address should be on the first line, and the destination address on the second line.
- `<smtp_server>`: The hostname or IP address of the SMTP server.
- `[port_number]` (optional): The port number of the SMTP server (default is 25).

### Example
```sh
./cSMTP addresses.txt smtp.example.com 587
```
In this example, `addresses.txt` contains:
```
source@example.com
destination@example.com
```

## Code Explanation

### Csmtp.c
The main implementation file for the cSMTP client. It includes:
- Establishing a connection to the SMTP server
- Sending SMTP commands and handling server responses
- Reading email addresses from a file
- Sending email content from the file

### Csmtp.h
The header file defining function prototypes and necessary includes for the SMTP client.

## Functions

### Csmtp.c
- `int main(int argc, char **argv)`: The main function to execute the SMTP client.
- `int getLine(int fd, char* line, int lim)`: Reads a line of data from the socket.
- `void readServer(int fd, char *line, int *lim, char *postfix)`: Reads the server response.
- `int checkReply(char reply)`: Checks the server reply code.
- `char* getPostfix(char* header, char* postfix)`: Extracts postfix from the server reply header.

### Csmtp.h
- `char* getPostfix(char *header, char *postfix)`: Function prototype for extracting postfix from server reply header.
- `void readServer(int fd, char *line, int* lim, char *postfix)`: Function prototype for reading server response.
- `int checkReply(char reply)`: Function prototype for checking server reply code.
- `int getLine(int fd, char line[], int max)`: Function prototype for getting a line of data from the socket.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contributing
Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

## Acknowledgments
This project was created by Uriel Shani as a simple demonstration of an SMTP client in C.

For any questions or issues, please contact the repository owner.
