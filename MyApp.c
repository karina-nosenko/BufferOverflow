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

    if((strcmp(username, username_buffer) == 0) && (strcmp(password, password_buffer) == 0)) {
      fclose(stream);
      return true;
    }
  }

  fclose(stream);
  return false;
}

void printUserData(char username[20]) {
  char username_buffer[20], cardNumber[20], expirationDate[10], cvc[10];
  char line[50];
  char* delimiter = ",\n\r";
  char* token;

  FILE *stream = fopen("Data/UserCreditCardData.csv", "r");

  while (fgets(line, 50, stream)) {
    token = strtok(line, delimiter);

    strcpy(username_buffer,token);
    token = strtok(NULL, delimiter);

    if((strcmp(username, username_buffer) == 0)) {
      strcpy(cardNumber,token);
      token = strtok(NULL, delimiter);
      strcpy(expirationDate,token);
      token = strtok(NULL, delimiter);
      strcpy(cvc,token);

      printf("____________________________\n\nUsername: %s\nCard Number: %s\nExpiration date: %s\nCVC: %s\n____________________________\n", (char*)username, (char*)cardNumber, (char*)expirationDate, (char*)cvc);
      return;
    }
  }
}

int main() {
  bool userAuthenticated = false;
  char password[20] = {0};
  char username[20] = {0};

  while(1) {
    printf("Username: ");
    gets(username);
    printf("Password: ");
    gets(password);

    if (isUserAuthenticated(username, password)) {
      userAuthenticated = true;
    }

    if (userAuthenticated) {
      printUserData(username);
    } else {
      printf("Unauthenticated!\n");
    }

    printf("\n");
    userAuthenticated = false;
  }
}
