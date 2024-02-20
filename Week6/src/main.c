#include <sys/types.h>
#include <fcntl.h>

#include "genWord.h"
#include "queue.h"
#include "polybius.h"

int main(int argc, char* argv[]){
    /*
    1. Write 10,000 words into tmp.txt.
    */
    int num = 100; //TODO: change to 10000
    char* fileName = "tmp.txt";
    FILE *wirte = fopen(fileName, "w");
    if(wirte == NULL) {
        perror("Error:");
        return 1;
    }

    for (int i = 0; i < num; i++) {
        char* r = genWord();
        fprintf(wirte, "%s\n", r);
        printf("%d Writing: %s into %s\n", i+1, r, fileName);
        free(r);
    }

    fclose(wirte);

    /*
    2. Read words in tmp.txt and store in queue.
    */
    queue_t* queue = createQ();
    FILE* readFile = fopen(fileName, "r");
    if(readFile == NULL) {
        perror("Error:");
        return 1;
    }

    int cnt = 0;
    while(!feof(readFile)){
        char word[MAXLEN + 1];
        // if(strcmp(word, "/n")==0)
        //     break;
        fscanf(readFile, "%s", word);
        printf("Reading: %s\n", word);
        process_t* p = createProcess(cnt++, word, 0, 0);
        add2q(queue, p);
        // free(word);
    }
    printQ(queue);

    /*
    3. Encrypt 100 words a time through the subprocess of poolybius cipher.
    */
    // int pipefd[2]; // 0 for read, 1 for write
    // pid_t pid;
    // int fd; // file descriptor

    // char* encodedFile = "output/encoded_";
    // strcat(encodedFile, genSeed());
    // strcat(encodedFile, ".txt");
    // fd = open(encodedFile, O_RDONLY | O_CREAT | O_TRUNC, 0644);
    // if (fd == -1) {
    //     perror("open");
    //     return 1;
    // }

    // if (pipe(pipefd) == -1) {
    //     perror("pipe");
    //     exit(EXIT_FAILURE);
    // }

    // pid = fork();
    // if (pid == -1) {
    //     perror("fork");
    //     exit(EXIT_FAILURE);
    // }

    // if (pid == 0) {
    //     // Child process
    //     close(pipefd[1]);

    //     if (dup2(fd, STDOUT_FILENO) == -1) {
    //         perror("dup2");
    //         exit(1);
    //     }
    //     close(fd);

    //     int size = 6;
    //     table_t *table = newTable(size);
    //     char c = 'A';
    //     for (int i = 0; i < table->size; i++){
    //         for (int j = 0; j < table->size; j++){
    //             table->square[i][j] = c++;
    //         }
    //     }

    //     char buffer[1000];
    //     while (1) {
    //         ssize_t count = read(pipefd[0], buffer, sizeof(buffer));  // Read data from pipe
    //         if (count == 0) {  // Check for end of stream (pipe closed)
    //             break;
    //         }
    //         printf("Child received: %s\n", buffer);
    //     }
        

    //     close(pipefd[0]); 
    // } else {
    //     close(pipefd[0]);

    //     int cnt = 0;
    //     char sentence[1200];
    //     sentence[0] = '\0';
    //     while(cnt++ < 100 && qsize(queue) > 0){
    //         process_t* p = (process_t*)popQ(queue);
    //         char* word = p->name;
    //         strcat(sentence, word);
    //         strcat(sentence, " ");
            
    //         if(cnt == 100){
    //             cnt = 0;
    //             sentence[0] = '\0';
    //             write(pipefd[1], sentence, strlen(sentence) + 1);  // Write data to pipe
    //             sleep(1);
    //         }
    //     }
    //     write(pipefd[1], sentence, strlen(sentence) + 1);  // Write data to pipe

    //     close(pipefd[1]);
    //     wait(NULL);  // Wait for child
    //     close(fd);
    // }


    // int after = lineCnt(fileName); // get the line number after write
    // /*test*/
    // if(num == after){
    //     printf("Successfully wrote %d lines into %s.\n", num, fileName);
    // }else{
    //     printf("Failed");
    // }

    deleteQ(queue);
    fclose(readFile);
    return 0;
}