#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define PORT 4444

bool password_compare(char* real, char* submitted) {
    for(int i=0; i<strlen(real); i++) {
        if(*real++ != *submitted++) {
            return false;
        }

        sleep(1);

        if(!submitted) {
            return false;
        }
    }
    return true;
}

bool isUserAuthenticated(char username[16], char password[16]) {
  char username_buffer[16], password_buffer[16];
  char line[50];
  char* delimiter = ",\n\r";
  char* token;

  FILE *stream = fopen("Data/Passwords.csv", "r");

  while (fgets(line, 50, stream)) {
    token = strtok(line, delimiter);

    strcpy(username_buffer,token);
    token = strtok(NULL, delimiter);
    strcpy(password_buffer,token);

    if((strcmp(username, username_buffer) == 0) && (password_compare(password_buffer, password) > 0)) {
      fclose(stream);
      return true;
    }
  }

  fclose(stream);
  return false;
}

void chatWithClient(int newSocket, struct sockaddr_in newAddr) {
    char username_buffer[1024];
    char password_buffer[1024];
    char server_buffer[1024];
    bool userAuthenticated = false;
    int buffer_size;

    while(1) {
        // Receive username from client
        buffer_size = recv(newSocket, username_buffer, 1024, 0);
        username_buffer[buffer_size] = '\0';
        if(strcmp(username_buffer, ":exit") == 0) {
            printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
            return;
        }
        printf("Username: %s\n", username_buffer);

        // Receive password from client
        buffer_size = recv(newSocket, password_buffer, 1024, 0);
        password_buffer[buffer_size] = '\0';
        if(strcmp(password_buffer, ":exit") == 0) {
            printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
            return;
        }
        printf("Password: %s\n", password_buffer);

        // Send response to client
        if(isUserAuthenticated(username_buffer, password_buffer)) {
            memcpy(server_buffer, "Authenticated", strlen("Authenticated"));
        } else {
            memcpy(server_buffer, "Unauthenticated", strlen("Unauthenticated"));
        }

        send(newSocket, server_buffer, strlen(server_buffer), 0);

        // Clear buffers
        bzero(server_buffer, sizeof(server_buffer)); 
        bzero(username_buffer, sizeof(username_buffer));
        bzero(password_buffer, sizeof(password_buffer));                 
    }
}

int main() {

    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Server Socket is created\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0) {
        printf("[-]Error in binding.\n");
        exit(1);
    }
    printf("[+]Bind to port %d\n", 4444);

    if(listen(sockfd, 10) == 0) {
        printf("Listening....\n");
    } else {
        printf("[-]Error in binding.\n");
    }

    while(1) {
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0) {
            exit(1);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        // Create process to the new connected client
        if((childpid = fork()) == 0) {
            close(sockfd);

            chatWithClient(newSocket, newAddr);
        }
    }
    
    close(newSocket);

    return 0;
}