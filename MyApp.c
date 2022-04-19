#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isUserAuthenticated(char username[20], char password[20]) {
  char username_buffer[20], password_buffer[20];
  char line[50];
  char* delimiter = ",\n\r";
  char* token;

  FILE *stream = fopen("Data/Passwords.csv", "r");

  while (fgets(line, 50, stream)) {
    token = strtok(line, delimiter);

    strcpy(username_buffer,token);
    token = strtok(NULL, delimiter);
    strcpy(password_buffer,token);
    //password_buffer[strlen(password_buffer)-2] = '\0';

    if((strcmp(username, username_buffer) == 0) && (strcmp(password, password_buffer) == 0)) {
      fclose(stream);
      return true;
    }
  }

  fclose(stream);
  return false;
}

void printUserData(char username[20]) {
  printf("Authenticated!\n");
}

int main() {
    char username[20];
    char password[20];

    while(1) {
      printf("Username: ");
      scanf("%s", (char*)&username);
      printf("Password: ");
      scanf("%s", (char*)&password);

      if (isUserAuthenticated(username, password)) {
        printUserData(username);
      } else {
        printf("Unauthenticated!\n");
      }
      printf("\n");
    }
}
