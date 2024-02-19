#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1900
//this function will find the % symbol
char *Find_Comment_Symbol( char comment, const char *readBuffer) {
    while (*readBuffer != '\0') {
        if (*readBuffer == comment) {
            return (char *)readBuffer;
        }
        readBuffer++;
    }
    return NULL;
}


void Remove_Comments(const char *TestCaseFile, const char *CleanTestCaseFile) {
   
    FILE *out = fopen(CleanTestCaseFile, "w");
    FILE *in = fopen(TestCaseFile, "r");
    char readBuffer[MAX_LEN];

    if (in == NULL) {
        printf("Input File Opening Error.\n");
        exit(1);
    }
    if (out == NULL) {
        printf("Output File Opening Error.\n");
        exit(1);
    }

    while (fgets(readBuffer, sizeof(readBuffer), in) != NULL) {
        // find a % (comment symbol) by traversing 
        char *com_ptr = Find_Comment_Symbol( '%', readBuffer); 
        if (com_ptr != NULL) {
            // if % exists, replace everything after that symbol with \n 
            *com_ptr = '\n';
            *(com_ptr + 1) = '\0';
        }
        
        fprintf(out, "%s", readBuffer);
        printf("%s", readBuffer);
    }

    fclose(in);
    fclose(out);
}

int main() {
    char TestCaseFile[50];
    char CleanTestCaseFile[100];

    printf("Enter the name of the test case file: ");
    scanf("%s", TestCaseFile);

    printf("Enter the name of the clean file: ");
    scanf("%s", CleanTestCaseFile);

    Remove_Comments(TestCaseFile, CleanTestCaseFile);
    printf("ALL THE COMMENTS IN THE TESTCASE FILE ARE SUCESSFULLY REMOVED.\n");

    return 0;
}
