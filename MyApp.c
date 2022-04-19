#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// void insertUserIdAndPassword(char id[20], char password[20])
// {
//     FILE *fpt;
//     fpt = fopen("Data/Passwords.csv", "a+");
//     fprintf(fpt, "%s, %s\n", id, password);
//     fclose(fpt);
// }
//
// void enterUserIdAndPassword()
// {
//     char user_id[20], user_password[20];
//     scanf("%s", &user_id);
//     scanf("%s", &user_password);
//     insertUserIdAndPassword(user_id, user_password);
// }
//
// int getUserData(char id[20], char password[20])
// {
//     char user_id[20], user_password[20];
//     FILE *stream = fopen("Data/Passwords.csv", "r");
//     char line[50];
//     const char s[2] = ",";
//     char *token;
//     while (fgets(line, 50, stream))
//     {
//         token = strtok(line, ",");
//         strcpy(user_id,token);
//         token = strtok(NULL, " ");
//         token = strtok(token, ",");
//         strcpy(user_password,token);
//         token = strtok(NULL, " ");
//         token = strtok(token, "\n");
//
//         // printf("%s%s",token,user_password);
//         if((strcmp(id,user_id)==0)&&(strcmp(password,user_password)==0))
//         {
//             return 1;
//         }
//     }
//     fclose(stream);
//     return 0;
// }

bool isUserAuthenticated(char username[20], char password[20]) {
  char username_buffer[20], password_buffer[20];
  char line[50];
  char* delimiter = ",";
  char* token;

  FILE *stream = fopen("Data/Passwords.csv", "r");

  while (fgets(line, 50, stream)) {
    token = strtok(line, delimiter);

    strcpy(username_buffer,token);
    token = strtok(NULL, delimiter);
    strcpy(password_buffer,token);
    password_buffer[strlen(password_buffer)-1] = '\0';

    printf("password:[%s], [%s]\n", (char*)password, (char*)password_buffer);
    printf("Strcmp-Password:%d\n",strcmp(password, password_buffer));

    if((strcmp(username, username_buffer) == 0) && (strcmp(password, password_buffer) == 0)) {
      fclose(stream);
      return true;
    }
  }

  fclose(stream);
  return false;
}

void printUserData(char username[20]) {
  printf("Authenticated!");
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
