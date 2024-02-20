/*
* source.c / Week3 command line argument
*
* Zhongshi Luo / CS5600 / Northeastern University
* Spring 2024 / Jan 29, 2024
*
*/

#include <stdio.h>

const char* args[] = {"-u", "-l", "-cap"};
const int argsSize = sizeof(args)/sizeof(args[0]);

int stringMatch(const char *str1, const char *str2) {
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0') {
        if(str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    return str1[i] == str2[i];
}

int isValidOpt(const char* arg){
    for(int i = 0; i < argsSize; i++){
        if(stringMatch(arg, args[i])){
            return i;
        }
    }
    return -1;
}

int isLetter(char c){
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return 1;
    return 0;
}

void upCast(char* str){
    int i = 0;
    while(str[i] != '\0') {
        if(isLetter(str[i]) && str[i] <= 'z' && str[i] >= 'a'){
            str[i] = 'A' + (str[i] - 'a');
        }
        i++;
    }
}

void downCast(char* str){
    int i = 0;
    while(str[i] != '\0') {
        if(isLetter(str[i]) && str[i] >= 'A' && str[i] <= 'Z'){
            str[i] = 'a' + (str[i] - 'A');
        }
        i++;
    }
}

void capitalize(char* str){
    upCast(str);
    downCast(str+1);
}

int main(int argc, char *argv[]) {
    if(argc <= 2){
        printf("Usage: ./arger-luo [operation] [input string]\n");
        return -1;
    }
    
    char* arg = argv[1];
    int argIdx = isValidOpt(arg);
    if(argIdx == -1){
        printf("Invalid operation argument: %s\n", arg);
        return -1;
    }

    switch (argIdx)
    {
    case 0:
        for(int i = 2; i < argc; i++){
            upCast(argv[i]);
        }
        break;

    case 1:
        for(int i = 2; i < argc; i++){
            downCast(argv[i]);
        }
        break;

    case 2:
        for(int i = 2; i < argc; i++){
            capitalize(argv[i]);
        }
        break;
    
    default:
        break;
    }

    for(int i = 2; i < argc; i++){
        printf("%s ",argv[i]);
    }
    printf("\n");

    return 0;
}