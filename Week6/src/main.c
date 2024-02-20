#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>

#include "genWord.h"
#include "polybius.h"
#include "queue.h"

void writeCipherToFile(const char *sentence, table_t *table) {
  // Create a file name with the current time
  char fileName[256];
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  strftime(fileName, sizeof(fileName), "cipher_%Y%m%d%H%M%S.txt", tm);
  printf("Writing to %s\n", fileName);

  mkdir("output", 0755);

  char filePath[256];
  snprintf(filePath, sizeof(filePath), "output/%s", fileName);

  // Open the file for writing
  int fd = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  // Save the current standard output
  int stdout_copy = dup(STDOUT_FILENO);

  // Redirect standard output to the file
  if (dup2(fd, STDOUT_FILENO) == -1) {
    perror("dup2");
    exit(EXIT_FAILURE);
  }

  // Write the cipher to the file
  printf("%s\n", pbEncode(sentence, table));

  // Restore the original standard output
  if (dup2(stdout_copy, STDOUT_FILENO) == -1) {
    perror("dup2");
    exit(EXIT_FAILURE);
  }

  // Close the file descriptor and the copy of the original standard output
  close(fd);
  close(stdout_copy);
}


int main(int argc, char *argv[]) {
  /*
  1. Write 10,000 words into tmp.txt.
  */
  int num = 101;  // TODO: change to 10000
  char *fileName = "tmp.txt";
  FILE *wirte = fopen(fileName, "w");
  if (wirte == NULL) {
    perror("Error:");
    return 1;
  }

  for (int i = 0; i < num; i++) {
    char *r = genWord();
    fprintf(wirte, "%s", r);
    if (i != num - 1) fprintf(wirte, "\n");
    printf("%d Writing: %s into %s\n", i + 1, r, fileName);
    free(r);
  }

  fclose(wirte);

  /*
  2. Read words in tmp.txt and store in queue.
  */
  queue_t *queue = createQ();
  FILE *readFile = fopen(fileName, "r");
  if (readFile == NULL) {
    perror("Error:");
    return 1;
  }

  int cnt = 0;
  while (!feof(readFile)) {
    char *word = (char *)malloc(MAXLEN + 1);
    fscanf(readFile, "%s", word);
    if (strcmp(word, "") == 0) continue;
    process_t *p = createProcess(cnt++, word, 0, 0);
    add2q(queue, p);
  }
  printQ(queue);

  /*
  3. Encrypt 100 words a time through the subprocess of poolybius cipher.
  */
  int pipefd[2];  // 0 for read, 1 for write
  pid_t pid;

  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    // Child process
    close(pipefd[1]);

    int size = 6;
    table_t *table = newTable(size);
    char c = 'A';
    for (int i = 0; i < table->size; i++) {
      for (int j = 0; j < table->size; j++) {
        table->square[i][j] = c++;
      }
    }

    char buffer[SENTENCE_BUFFER_SIZE];
    while (read(pipefd[0], buffer, sizeof(buffer)) > 0) {
      printf("Child process received: %s\n", buffer);
      writeCipherToFile(buffer, table);
    }

    close(pipefd[0]);
    freeTable(table);
    exit(0);
  } else {
    close(pipefd[0]);

    int cnt = 0;
    char sentence[SENTENCE_BUFFER_SIZE] = "";

    while (qsize(queue) > 0) {
      process_t *p = (process_t *)popQ(queue);
      strcat(sentence, p->name);
      strcat(sentence, " ");
      freeProcess(p);

      if (++cnt % 100 == 0) {
        ssize_t bytes_written = write(pipefd[1], sentence, strlen(sentence) + 1);
        sleep(1);
        sentence[0] = '\0';
      }
    }
    if (cnt % 100 != 0)
      write(pipefd[1], sentence, strlen(sentence) + 1);  // Write data to pipe

    close(pipefd[1]);
    waitpid(pid, NULL, 0);  // Wait for child
  }

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