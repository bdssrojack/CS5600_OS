/*
 * server.c -- TCP Socket Server
 *
 * adapted from:
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <unistd.h>

#include "command.h"

int socket_desc;
int server_ready = 1, server_failed = -1;

/**
 * @brief Signal handler for closing the socket
 *
 * @param sig signal number
 */
void close_socket(int sig) {
    close(socket_desc);
    // close(client_sock);
    printf("\nClosing the socket\n");
    exit(0);
}

/**
 * @brief Server's respond function for write operation
 */
void writeRespond(int client_sock) {
    // 1. Send ready in write mode to client
    if (send(client_sock, &server_ready, sizeof(server_ready), 0) < 0) {
        printf("Unable to send ready in write mode\n");
        return;
    }
    printf("Client's operation received. Server is in WRITE mode\n");

    // 2. Receive file path and size from client
    char file_path[FILE_PATH_LEN];
    memset(file_path, 0, sizeof(file_path));
    if (recv(client_sock, file_path, sizeof(file_path), 0) < 0) {
        printf("Couldn't receive file path\n");
        return;
    }
    printf("File path received: %s\n", file_path);
    int file_size;
    if (recv(client_sock, &file_size, sizeof(file_size), 0) < 0) {
        printf("Couldn't receive file size\n");
        return;
    }
    printf("File size received: %d\n", file_size);

    // 3. Set up file descriptor and response ready to client
    int fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        printf("Couldn't open file\n");
        return;
    }

    if (send(client_sock, &server_ready, sizeof(server_ready), 0) < 0) {
        printf("Unable to send ready in write mode\n");
        return;
    }

    // lock the fd
    if (flock(fd, LOCK_EX) < 0) {
        printf("Couldn't lock file descriptor\n");
        return;
    }

    printf("File descriptor ready\n");

    // 4. Receive data stream from client and write to file
    char buffer[8196];
    int read_size;
    int total_size = 0;
    while (total_size < file_size) {
        read_size = recv(client_sock, buffer, sizeof(buffer), 0);
        if (read_size < 0) {
            printf("Couldn't receive data stream\n");
            return;
        }
        if (write(fd, buffer, read_size) < 0) {
            printf("Couldn't write to file\n");
            return;
        }
        total_size += read_size;
    }

    printf("Data stream written to file\n");

    // 5. Send write status and close file descriptor
    if (send(client_sock, &server_ready, sizeof(server_ready), 0) < 0) {
        printf("Unable to send write status\n");
        return;
    }
    printf("File written successfully\n");

    // unlock the fd
    if (flock(fd, LOCK_UN) < 0) {
        printf("Couldn't unlock file descriptor\n");
        return;
    }

    close(fd);
}

/**
 * @brief Server's respond function for get operation
 */
void getRespond(int client_sock) {
    // 2. Send ready in get mode to client
    if (send(client_sock, &server_ready, sizeof(server_ready), 0) < 0) {
        printf("Unable to send ready in get mode\n");
        return;
    }
    printf("Client's operation received. Server is in GET mode\n");

    // 4. Receive file path from client, found the file in server
    char file_path[FILE_PATH_LEN];
    memset(file_path, 0, sizeof(file_path));
    if (recv(client_sock, file_path, sizeof(file_path), 0) < 0) {
        printf("Couldn't receive file path\n");
        return;
    }
    printf("File path received: %s\n", file_path);

    // 5. If not found, send failed to client and return
    // If found, send file size and file data to client
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        printf("File not found\n");
        if (send(client_sock, &server_failed, sizeof(server_failed), 0) < 0) {
            printf("Unable to send failed to client\n");
            return;
        }
        return;
    } else {
        printf("File found\n");
        int file_size = 0;
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);
        if (send(client_sock, &file_size, sizeof(file_size), 0) < 0) {
            printf("Unable to send file size in get mode\n");
            return;
        }

        char buffer[1024];
        int read_size = 0;
        while ((read_size = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            if (send(client_sock, buffer, read_size, 0) < 0) {
                printf("Unable to send file data in get mode\n");
                return;
            }
        }

        fclose(file);
        printf("File data sent to client\n");
    }
}

/**
 * @brief Server's respond function for rm operation
 */
void rmRespond(int client_sock) {
    // 2. Send ready in remove mode to client
    if (send(client_sock, &server_ready, sizeof(server_ready), 0) < 0) {
        printf("Unable to send ready in remove mode\n");
        return;
    }
    printf("Client's operation received. Server is in RM mode\n");

    // 4. Receive file path from client, remove the file in server
    char file_path[FILE_PATH_LEN];
    memset(file_path, 0, sizeof(file_path));
    if (recv(client_sock, file_path, sizeof(file_path), 0) < 0) {
        printf("Couldn't receive file path\n");
        return;
    }
    printf("File path received: %s\n", file_path);

    // 5. Send remove status to client
    if (remove(file_path) < 0) {
        printf("Couldn't remove file\n");
        if (send(client_sock, &server_failed, sizeof(server_failed), 0) < 0) {
            printf("Unable to send remove status\n");
            return;
        }
        return;
    } else {
        printf("File removed\n");
        if (send(client_sock, &server_ready, sizeof(server_ready), 0) < 0) {
            printf("Unable to send remove status\n");
            return;
        }
    }
}

void* client_handler(void* socket_desc) {
    int client_sock = *(int*)socket_desc;
    free(socket_desc);
    printf("New thread running for client %d\n", client_sock);

    // Receive the operation of client's command:
    operation_t op;
    if (recv(client_sock, &op, sizeof(operation_t), 0) < 0) {
        printf("Couldn't receive\n");
        close(client_sock);
        return NULL;
    }

    switch (op) {
        case WRITE:
            writeRespond(client_sock);
            break;
        case GET:
            getRespond(client_sock);
            break;
        case RM:
            rmRespond(client_sock);
            break;

        default:
            break;
    }

    // Closing the client socket:
    close(client_sock);

    return NULL;
}

int main(void) {
    // Register signal handler
    signal(SIGINT, close_socket);  // Ctrl + C

    socklen_t client_size;
    struct sockaddr_in server_addr, client_addr;

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set SO_REUSEADDR option for immediately rebinding the port after last
    // closing:
    int opt = 1;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
        0) {
        printf("Error setting SO_REUSEADDR option\n");
        close(socket_desc);
        return -1;
    }

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind to the set port and IP:
    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) <
        0) {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");

    // Listen for clients:
    if (listen(socket_desc, 1) < 0) {
        printf("Error while listening\n");
        close(socket_desc);
        return -1;
    }
    printf("\nListening for incoming connections.....\n");

    while (1) {
        printf("=====================================\n");
        // Accept an incoming connection:
        client_size = sizeof(client_addr);
        int* client_sock_ptr = malloc(sizeof(int));
        *client_sock_ptr =
            accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);

        if (*client_sock_ptr < 0) {
            printf("Can't accept, exit\n");
            close(socket_desc);
            close(*client_sock_ptr);
            free(client_sock_ptr);
            return -1;
        }

        printf("Client connected at IP: %s and port: %i\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new thread for the client
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, client_handler,
                           (void*)client_sock_ptr) < 0) {
            printf("Could not create thread\n");
            free(client_sock_ptr);
            continue;
        }

    }

    return 0;
}
