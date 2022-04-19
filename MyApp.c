#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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

    if((strcmp(username, username_buffer) == 0) && (strcmp(password, password_buffer) == 0)) {
      fclose(stream);
      return true;
    }
  }

  fclose(stream);
  return false;
}

void printUserData(char username[16]) {
  char username_buffer[16], cardNumber[16], expirationDate[8], cvc[8];
  char line[50];
  char* delimiter = ",\n\r";
  char* token;

  if(strlen(username) <= 0) {
    username = "idan";
  }

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

      printf("____________________________\n\n");
      printf("Username: %s\n", (char*)username);
      printf("Card Number: %s\n", (char*)cardNumber);
      printf("Expiration date: %s\n", (char*)expirationDate);
      printf("CVC: %s\n", (char*)cvc);
      printf("____________________________\n");

      return;
    }
  }
}

void printAllPasswords() {
  char username[16], password[16];
  char line[50];
  char* delimiter = ",\n\r";
  char* token;

  FILE *stream = fopen("Data/Passwords.csv", "r");

  while (fgets(line, 50, stream)) {
    token = strtok(line, delimiter);
    strcpy(username, token);
    token = strtok(NULL, delimiter);
    strcpy(password, token);   

    printf("Username: %s\n",(char*)username);
    printf("Password: %s\n\n",(char*)password);
  }
}

int main() {
  bool userAuthenticated = false;
  char password[16] = {0};
  char username[16] = {0};

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
