#include <stdio.h>
#include <string.h>

void insertUserIdAndPassword(char id[20], char password[20])
{
    FILE *fpt;
    fpt = fopen("Data/Passwords.csv", "a+");
    fprintf(fpt, "%s, %s\n", id, password);
    fclose(fpt);
}

void enterUserIdAndPassword()
{
    char user_id[20], user_password[20];
    scanf("%s", &user_id);
    scanf("%s", &user_password);
    insertUserIdAndPassword(user_id, user_password);
}

int getUserData(char id[20], char password[20])
{
    char user_id[20], user_password[20];
    FILE *stream = fopen("Data/Passwords.csv", "r");
    char line[50];
    const char s[2] = ",";
    char *token;
    while (fgets(line, 50, stream))
    {
        char user_id[20];
        char user_password[20];
        token = strtok(line, ",");
        strcpy(user_id,token);
        token = strtok(NULL, " ");
        token = strtok(token, ",");
        strcpy(user_password,token);
        token = strtok(NULL, " ");
        token = strtok(token, "\n");

        // printf("%s%s",token,user_password);
        if((strcmp(id,user_id)==0)&&(strcmp(password,user_password)==0))
        {
            return 1;
        }
    }
    fclose(stream);
    return 0;
}

int main()
{
    char user_id[20];
    char user_pass[20];
    scanf("%s", &user_id);
    scanf("%s", &user_pass);
    printf("%d",getUserData(user_id,user_pass));
}
