// compiled with gcc (Debian 11.3.0-5) 11.3.0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> //mkdir()
#include <dirent.h>   //opendir(), DIR

#define MAX_COMMAND_SIZE 20
#define MAX_FILE_NAME 50
#define MAX_INPUT_SIZE 100
#define MAX_OUTPUT_SIZE 100

clipboard[(int)1e9];

void super_md(char *address, int n)
{
    int i;
    char *dir = (char *)calloc(n, sizeof(char));
    for (i = n; i >= 0; i--)
        if (address[i] == '/')
            break;
    strncpy(dir, address, i);
    int stat = mkdir(dir, 0777);
    if (stat == -1)
        super_md(dir, i);
    mkdir(dir, 0777);
    free(dir);
}

void createFile(char *fileName, char *output)
{
    FILE *newFile;
    newFile = fopen(fileName, "r");
    if (newFile != NULL)
    {
        strcpy(output, "File existed!");
    }
    else
    {
        DIR *dir = opendir(fileName);
        if (dir != NULL)
        {
            strcpy(output, "There is already a folder with the same name as the file name you specified. Specify a different name.");
        }
        else
        {
            newFile = fopen(fileName, "w");
            if (newFile == NULL)
            {
                int n = strlen(fileName);
                super_md(fileName, n);
                newFile = fopen(fileName, "w");
            }
            strcpy(output, "Success!");
        }
    }
    fclose(newFile);
}

char *get_dir(char *fileName, int n)
{
    int i;
    char *dir = (char *)calloc(1500, sizeof(char));
    for (i = n; i >= 0; i--)
        if (fileName[i] == '/')
            break;
    if (i > 0)
        strncpy(dir, fileName, i);
    return dir;
}

void insert(char *fileName, char *str, int lineNum, int charNum, char *output)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
        strcpy(output, "Something went wrong!");
    else
    {
        int n = strlen(fileName);
        char *dir = get_dir(fileName, n);
        strcat(dir, "/thisIsAnAuxiliaryFile.txt");
        FILE *auxiliary_file = fopen(dir, "w");
        char c = 0;
        --lineNum;
        while (lineNum > 0 && !feof(file))
        {
            c = fgetc(file);
            fputc(c, auxiliary_file);
            if (c == '\n')
                --lineNum;
        }
        while (charNum > 0 && !feof(file))
        {
            c = fgetc(file);
            charNum--;
            fputc(c, auxiliary_file);
        }
        fputs(str, auxiliary_file);
        while (1)
        {
            c = fgetc(file);
            if (c == EOF)
                break;
            fputc(c, auxiliary_file);
        }
        fclose(file);
        fclose(auxiliary_file);
        remove(fileName);
        rename(dir, fileName);
        strcpy(output, "Success!");
    }
}

void cat(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        puts("Something went wrong!");
        return;
    }
    char c;
    while (1)
    {
        c = fgetc(file);
        if (c == -1)
            break;
        printf("%c", c);
    }
    fclose(file);
    printf("\n");
}

void removeStr(char *fileName, int lineNum, int charNum, int size, char jahat, char *output)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        strcpy(output, "Something went wrong!");
        return;
    }
    int l = strlen(fileName);
    char *dir = get_dir(fileName, l);
    strcat(dir, "/thisIsAnAuxiliaryFile.txt");
    FILE *auxiliary_file = fopen(dir, "w");
    --lineNum;
    char c = 0;
    int n = 0;
    while (lineNum > 0 && !feof(file))
    {
        c = fgetc(file);
        putc(c, auxiliary_file);
        ++n;
        if (c == '\n')
            --lineNum;
    }
    while (charNum > 0 && !feof(file))
    {
        c = fgetc(file);
        --charNum;
        putc(c, auxiliary_file);
        ++n;
    }
    if (jahat == 'b')
    {
        fseek(auxiliary_file, -size, SEEK_CUR);
    }
    else if (jahat == 'f')
        fseek(file, size, SEEK_CUR);
    while (1)
    {
        c = fgetc(file);
        if (c == EOF)
            break;
        fputc(c, auxiliary_file);
    }
    strcpy(output, "Success!");
    fclose(file);
    fclose(auxiliary_file);
    remove(fileName);
    rename(dir, fileName);
}

int extract_from_input(char *input, char *subStr)
{
    int n;
    if (input[0] == '"')
    {
        input += 1;
        sscanf(input, "%[^\"]", subStr);
        n = strlen(subStr) + 2;
    }
    else
    {
        sscanf(input, "%s", subStr);
        n = strlen(subStr);
    }
    return n;
}

int main()
{
    char *input_line = (char *)calloc(MAX_INPUT_SIZE, sizeof(char));
    char command[MAX_COMMAND_SIZE];
    char fileName[MAX_FILE_NAME];
    char output[MAX_OUTPUT_SIZE];
    char str[200];
    char lineNum[100], charNum[100], size[1000], flag;
    while (1)
    {
        fgets(input_line, MAX_INPUT_SIZE, stdin);
        input_line[strlen(input_line) - 1] = 0;
        int i;
        for (i = 0;; i++)
        {
            if (input_line[i] == 32 || input_line[i] == 0)
            {
                command[i] = 0;
                break;
            }
            command[i] = input_line[i];
        }
        input_line += i + 1;
        if (0 == strcmp(command, "createfile"))
        {
            extract_from_input(input_line, fileName);
            createFile(fileName, output);
        }
        else if (0 == strcmp(command, "insert"))
        {
            input_line += extract_from_input(input_line, fileName) + 1;
            input_line += extract_from_input(input_line, str) + 1;
            sscanf(input_line, "%[^:]", lineNum);
            input_line += strlen(lineNum) + 1;
            sscanf(input_line, "%s", charNum);
            insert(fileName, str, atoi(lineNum), atoi(charNum), output);
        }
        else if (0 == strcmp(command, "cat"))
        {
            extract_from_input(input_line, fileName);
            cat(fileName);
            continue;
        }
        else if (0 == strcmp(command, "remove"))
        {
            input_line += extract_from_input(input_line, fileName) + 1;
            sscanf(input_line, "%[^:]", lineNum);
            input_line += strlen(lineNum) + 1;
            sscanf(input_line, "%s %s -%c", charNum, size, &flag);
            removeStr(fileName, atoi(lineNum), atoi(charNum), atoi(size), flag, output);
        }
        else if (0 == strcmp(command, "exit"))
        {
            return 0;
        }
        else
        {
            strcpy(output, "Invalid command!");
        }
        puts(output);
    }
    free(input_line);
    return 0;
}
