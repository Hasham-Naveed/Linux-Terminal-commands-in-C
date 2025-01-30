#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<signal.h>
#include<dirent.h>
#include<errno.h>
#include<time.h>

#define BUFFER_SIZE 2048
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE   "\x1b[34m"
#define ANSI_COLOR_MAGENTA   "\x1b[35m"
#define ANSI_COLOR_CYAN   "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BOLD   "\x1b[1m"

void makeDirectory(char *ch){
char dirPath[40];
mode_t mode = S_IRWXU;
int j=0;
for(int i=6;ch[i]!='\0';i++,j++){
    if(ch[i]!='\n'){
        dirPath[j]=ch[i];
    }
    else{
        j--;
    }
}
dirPath[j]='\0';
int status=mkdir(dirPath,mode);
if(status==0){printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_BLUE "Directory created successfully\n" ANSI_COLOR_RESET);}
else{ printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_BLUE "Error creating directory\n" ANSI_COLOR_RESET);}
}

void Pwd(){
    char cwd[4096];
    if(getcwd(cwd,sizeof(cwd))!=NULL){
        printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET,cwd);
    }
    else{
        perror(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_GREEN "pwd error" ANSI_COLOR_RESET);
    }
}

void Cd(char *ch)
{
    char dirPath[40];
    for(int i=3,j=0;ch[i]!='\n';i++,j++){
    dirPath[j]=ch[i];
    }
    if(chdir(dirPath)==0){
        printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_BLUE "Changed to directory:%s\n" ANSI_COLOR_RESET,dirPath);
    }
    else{
        perror(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_BLUE "Error changing direcotry\n" ANSI_COLOR_RESET);
    }
}
void Remove(char *ch){
    char filePath[40];
    int j=0;
    for(int i=3;ch[i]!='\n';i++,j++){
    filePath[j]=ch[i];
    }
    filePath[j]='\0';
    if(remove(filePath)==0){
        printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_GREEN"%s removed successfully\n" ANSI_COLOR_RESET,filePath);
    }
    else{
        perror(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_GREEN "Error removing file\n" ANSI_COLOR_RESET);
    }
}

void Kill(char *ch){
    char filePath[40];
    for(int i=5,j=0;ch[i]!='\n';i++,j++){
    filePath[j]=ch[i];
    }
    pid_t Pid=atoi(filePath);
    if(kill(Pid,SIGTERM)==0){
        printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_CYAN "Process with Pid %d killed successfully \xF0\x9F\x98\x88\n" ANSI_COLOR_RESET,Pid);
    }
    else{
        perror(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_CYAN "Error killing process" ANSI_COLOR_RESET);
    }
}


void Copy(char *ch) {
    char sourceFile[80]; // Increased buffer size
    char destinationFile[80];
    int i = 3, j = 0;

    // Copy source filename, including extension
    while (ch[i] != '\0' && ch[i] != ' ') {
        sourceFile[j] = ch[i];
        i++;
        j++;
    }
    sourceFile[j] = '\0';

    // Skip whitespace
    i++;

    // Copy destination filename
    j = 0;
    while (ch[i] != '\n' && ch[i] != '\0') {
        destinationFile[j] = ch[i];
        i++;
        j++;
    }
    destinationFile[j] = '\0';

    FILE *source = fopen(sourceFile, "r");
    FILE *destination = fopen(destinationFile, "w");

    if (!source) {
        printf("Error opening %s\n", sourceFile);
        return;
    }
    if (!destination) {
        printf("Error opening %s\n", destinationFile);
        return;
    }

    char buffer[BUFFER_SIZE];

    // Check for confirmation if overwriting
    if (access(destinationFile, F_OK) == 0) {
        char confirmation;
        printf("File %s already exists. Overwrite? (y/N): ", destinationFile);
        scanf("%c", &confirmation);
        if (confirmation != 'y' && confirmation != 'Y') {
            fclose(source);
            fclose(destination);
            return;
        }
    }

    // Read and write data in chunks
    while (fgets(buffer, BUFFER_SIZE, source) != NULL) {
        if (fputs(buffer, destination) == EOF) {
            printf("Error writing to %s\n", destinationFile);
            break;
        }
    }

    fclose(source);
    fclose(destination);
}


void Cat(char *ch){
    char file[40];
    int j=0;
    for(int i=4;ch[i]!='\n';i++,j++){
        file[j]=ch[i];
    }
    file[j]='\0';
    FILE *file1=fopen(file,"r");
    if(file==NULL){
        printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_WHITE "Error opening file\n"ANSI_COLOR_RESET );
    }
    char buffer[BUFFER_SIZE];
    while(fgets(buffer,sizeof(buffer),file1)){
        printf(ANSI_COLOR_BOLD ANSI_COLOR_WHITE "%s"ANSI_COLOR_RESET ,buffer);
    }
}

void stripNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void List(char *ch){
    char *array[50];
    ch[strlen(ch)-1]='\0';
    char *option_1;
    char *dirPath=ch;
    char *temp;
    if(strcmp(ch,"ls")==0){
        struct dirent *d;
        DIR *dh =opendir(".");
          while ((d = readdir(dh)) != NULL) {
            if (d->d_name[0] == '.')
                continue;
            // d->d_name[strlen(d->d_name)-1]='\0';
            printf(ANSI_COLOR_BOLD ANSI_COLOR_GREEN "%s  " ANSI_COLOR_RESET,d->d_name);
        }
        printf("\n");
        return;
    }
    else if(ch[3]!='-'){
        struct dirent *d;
        temp=strsep(&dirPath," ");
        DIR *dh =opendir(dirPath);
          while ((d = readdir(dh)) != NULL) {
            if (d->d_name[0] == '.')
                continue;
            // d->d_name[strlen(d->d_name)-1]='\0';
            printf(ANSI_COLOR_BOLD ANSI_COLOR_GREEN "%s  " ANSI_COLOR_RESET,d->d_name);
        }
        printf("\n");
        return;
    }
temp = strsep(&dirPath, "-");
    option_1 = strsep(&dirPath, " ");
    struct stat fileStat;
    struct dirent *d;
    DIR *dh = opendir(dirPath);
    if (!dh) {
        if (errno == ENOENT) {
            perror(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_GREEN "Directory doesn't exist\n"ANSI_COLOR_RESET );
        } else {
            perror(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_GREEN "Unable to read directory\n"ANSI_COLOR_RESET );
        }
        return;
    }
    int showHidden = 0;
    int showDetails = 0;
    int reverseOrder = 0;
    switch (option_1[0]) {
        case 'a':
            showHidden = 1;
            break;
        case 'l':
            showDetails = 1;
            break;
        case 'r':
            reverseOrder = 1;
            break;
        default:
            printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_GREEN "Choose any one from 'a' or 'l' or 'r'\n"ANSI_COLOR_RESET );
            return;
    }
    if (showDetails) {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "%-20s %-15s %-30s %s\n" ANSI_COLOR_RESET, "File Name", "Size (bytes)", "Last Modified", "Is Directory");
    } else {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, "File Name");
    }
    int i=0;
    while((d = readdir(dh)) != NULL) {
        if (!showHidden && d->d_name[0] == '.')
            continue;
        char filePath[100];
        strcpy(filePath, dirPath);
        strcat(filePath, "/");
        strcat(filePath, d->d_name);
        if(reverseOrder){
            stripNewline(d->d_name);
            array[i]=d->d_name;
            i++;
        }
        else if (showDetails && stat(filePath, &fileStat) == -1) {
            perror(ANSI_COLOR_BOLD ANSI_COLOR_RED"Error getting file information"ANSI_COLOR_RESET);
            continue;
        }
        else if (showDetails) {
            stripNewline(d->d_name);
            char modifiedTime[30];
            strftime(modifiedTime, sizeof(modifiedTime), "%Y-%m-%d %H:%M:%S", localtime(&fileStat.st_mtime));
            printf(ANSI_COLOR_BOLD ANSI_COLOR_GREEN "%-20s %-15lld %-30s %s\n" ANSI_COLOR_RESET, d->d_name, (long long)fileStat.st_size, modifiedTime,
            S_ISDIR(fileStat.st_mode) ? "Yes" : "No");
        }
        else {
            stripNewline(d->d_name);
            printf(ANSI_COLOR_BOLD ANSI_COLOR_GREEN"%s\n" ANSI_COLOR_RESET, d->d_name);
        }
    }
    if(reverseOrder){
    for(int j=0;j<i-1;j++){
        if(strcmp(array[j],array[j+1])<0){
                char *temporary=array[j];
                array[j]=array[j+1];
                array[j+1]=temporary;
            }
        }
        for(int j=0;j<i;j++){
            printf("%s ",array[j]);
        } 
    }
    closedir(dh);
    printf("\n");
}
int main(){
int flag=1;
char ch[40];
while(flag==1)
{
 printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "@fa21-bcs-090>" ANSI_COLOR_RESET);
 fgets(ch,sizeof(ch),stdin);
 if(strncmp(ch,"EXIT",4)==0){flag=0;}
 else{
if(strcmp(ch,"\n")==0){
 }
 else if(strncmp(ch,"mkdir",5)==0)
 { makeDirectory(ch);
 }
 else if(strncmp(ch,"pwd",3)==0)
 {
    Pwd();
 }
 else if(strncmp(ch,"cd",2)==0){
    Cd(ch);
 }
 else if(strncmp(ch,"rm",2)==0){
    Remove(ch);
 }
 else if(strncmp(ch,"kill",4)==0){
    Kill(ch);
 }
 else if(strncmp(ch,"cp",2)==0){
    Copy(ch);
 }
 else if(strncmp(ch,"cat",3)==0){
    Cat(ch);
 }
 else if(strncmp(ch,"ls",2)==0){
    List(ch);
 }
 else{
 printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "fa21-bcs-090>" ANSI_COLOR_BLUE "Command not found\n" ANSI_COLOR_RESET);
 }
 }
 }
 }
