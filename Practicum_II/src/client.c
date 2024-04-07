/*
 * client.c -- TCP Socket Client
 *
 * adapted from:
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "command.h"

int socket_desc;

void close_socket() {
    close(socket_desc);
    printf("\nClosing the client socket\n");
    exit(0);
}

/**
 * @brief Send write request to server
 * Command format: ./rfs WRITE <local_file_path> [remote_file_path]
 *
 * @param cmd
 */
void writeRequest(command_t* cmd) {
    int server_ready = -1;

    // 1. Send operation type to server:
    if (send(socket_desc, &cmd->op, sizeof(operation_t), 0) < 0) {
        printf("Unable to send operation type.\n");
        close_socket();
    }
    printf("Operation type sent to server\n");

    // 2. Server response: into write mode
    if (recv(socket_desc, &server_ready, sizeof(server_ready), 0) < 0) {
        printf("Error while receiving server's msg\n");
        close_socket();
    }
    printf("Server is ready for write\n");

    // 3. Send remote file path and size to server:
    if (server_ready) {
        // If the remote file or path is omitted, use the values for the first
        // argument.
        if (strlen(cmd->file_path_2) == 0) {
            strcpy(cmd->file_path_2, cmd->file_path_1);
        }
        if (send(socket_desc, cmd->file_path_2, strlen(cmd->file_path_2), 0) <
            0) {
            printf("Unable to send remote file path\n");
            close_socket();
        }

        int file_size = 0;
        FILE* local_file = fopen(cmd->file_path_1, "r");
        fseek(local_file, 0, SEEK_END);
        file_size = ftell(local_file);
        rewind(local_file);

        if (send(socket_desc, &file_size, sizeof(file_size), 0) < 0) {
            printf("Unable to send file size\n");
            fclose(local_file);
            close_socket();
        }
        fclose(local_file);
    } else {
        printf("Server is not ready for write\n");
        close_socket();
    }
    printf("Remote file path and size sent to server\n");

    // 4. Server response: file descriptor ready
    server_ready = -1;
    if (recv(socket_desc, &server_ready, sizeof(server_ready), 0) < 0) {
        printf("Error while receiving server's msg\n");
        close_socket();
    }
    printf("File descriptor is ready in Server\n");

    // 5. Send local data stream to server:
    if (server_ready) {
        FILE* local_file = fopen(cmd->file_path_1, "r");
        if (local_file == NULL) {
            printf("Unable to open local file\n");
            close_socket();
        }

        char buffer[1024];
        int read_size = 0;

        while ((read_size = fread(buffer, 1, sizeof(buffer), local_file)) > 0) {
            if (send(socket_desc, buffer, read_size, 0) < 0) {
                printf("Unable to send data to server\n");
                close_socket();
            }
        }

        fclose(local_file);
    } else {
        printf("Server is not ready for write\n");
        close_socket();
    }
    printf("Local data stream sent to server\n");

    // 6. Server response: operation status
    server_ready = -1;
    if (recv(socket_desc, &server_ready, sizeof(server_ready), 0) < 0) {
        printf("Error while receiving server's msg\n");
        close_socket();
    }

    if (server_ready) {
        printf("Write operation successful\n");
    } else {
        printf("Write operation failed\n");
    }
}

/**
 * @brief Send get request to server
 * 
 * Command format: ./rfs GET <remote_file_path> [local_file_path]
 *
 * @param cmd
 */
void getRequest(command_t* cmd) {
    int server_ready = -1;

    // 1. Send operation type to server:
    if (send(socket_desc, &cmd->op, sizeof(operation_t), 0) < 0) {
        printf("Unable to send operation type.\n");
        close_socket();
    }

    // 3. Server response: into get mode
    // Send remote file path to server:
    if (recv(socket_desc, &server_ready, sizeof(server_ready), 0) < 0) {
        printf("Error while receiving server's msg\n");
        close_socket();
    }

    if(server_ready) {
        printf("remote file path: %s\n", cmd->file_path_1);
        if (send(socket_desc, cmd->file_path_1, strlen(cmd->file_path_1), 0) < 0) {
            printf("Unable to send remote file path\n");
            close_socket();
        }
    } else {
        printf("Server is not ready for GET\n");
        close_socket();
    }

    // 6. Server response: file search failed (-1) or file size (>=0)
    // If failed, return; if succeeded, prepare local file descriptor and receive data stream from server

    int file_size;
    if (recv(socket_desc, &file_size, sizeof(file_size), 0) < 0) {
        printf("Error while receiving file size\n");
        close_socket();
    }    

    if (file_size < 0) {
        printf("File not found in server\n");
        close_socket();
    } else {
        // if the local file path is omitted, use the current file path
        if (strlen(cmd->file_path_2) == 0) {
            // TODO: get the remote file name
            strcpy(cmd->file_path_2, cmd->file_path_1);
            printf("local file path: %s\n", cmd->file_path_2);
        }

        FILE* local_file = fopen(cmd->file_path_2, "w");
        if (local_file == NULL) {
            printf("Unable to open local file\n");
            close_socket();
        }

        char buffer[8196];
        int read_size;
        int total_size = 0;
        while (total_size < file_size) {
            read_size = recv(socket_desc, buffer, sizeof(buffer), 0);
            if (read_size < 0) {
                printf("Couldn't receive data stream\n");
                close_socket();
            }
            if (fwrite(buffer, 1, read_size, local_file) < 0) {
                printf("Couldn't write to file\n");
                close_socket();
            }
            total_size += read_size;
        }

        printf("Data stream written to file\n");
        fclose(local_file);
    }

}

/**
 * @brief Send remove request to server
 *
 * @param cmd
 */
void rmRequest(command_t* cmd) {}

/**
 * @brief parse command line arguments that client receives
 *
 * @param argc
 * @param argv
 * @return command_t*
 */
command_t* parseCommand(int argc, char* argv[]) {
    if (argc > 4 || argc < 3) {
        perror("Invalid number of arguments\n");
        printf(
            "Usage: ./rfs WRITE <local_file_path> [remote_file_path]\n or "
            "./rfs GET <remote_file_path> <local_file_path>\n or ./rfs RM "
            "<file_path>\n");
        exit(1);
    }

    command_t* cmd = (command_t*)malloc(sizeof(command_t));

    if (strcmp(argv[1], "WRITE") == 0) {
        cmd->op = WRITE;
    } else if (strcmp(argv[1], "GET") == 0) {
        cmd->op = GET;
    } else if (strcmp(argv[1], "RM") == 0) {
        cmd->op = RM;
    } else {
        perror("Invalid operation");
        free(cmd);
        close_socket();
    }

    strncpy(cmd->file_path_1, argv[2], FILE_PATH_LEN - 1);
    cmd->file_path_1[FILE_PATH_LEN - 1] = '\0';  // Ensure null termination
    strcpy(cmd->file_path_1, argv[2]);

    if (argc == 4) {
        strncpy(cmd->file_path_2, argv[3], FILE_PATH_LEN - 1);
        cmd->file_path_2[FILE_PATH_LEN - 1] = '\0';  // Ensure null termination
        // strcpy(cmd->remote_file_path, argv[3]);
    } else {
        cmd->file_path_2[0] =
            '\0';  // No remote file path provided, set to empty string
    }

    return cmd;
}

int main(int argc, char* argv[]) {
    struct sockaddr_in server_addr;

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0) {
        printf("Unable to create socket\n");
        close(socket_desc);
        return -1;
    }

    printf("Socket created successfully\n");

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send connection request to server:
    if (connect(socket_desc, (struct sockaddr*)&server_addr,
                sizeof(server_addr)) < 0) {
        printf("Unable to connect\n");
        close(socket_desc);
        return -1;
    }
    printf("Connected with server successfully\n");

    command_t* cmd = parseCommand(argc, argv);

    switch (cmd->op) {
        case WRITE:
            writeRequest(cmd);
            break;
        case GET:
            getRequest(cmd);
            break;
        case RM:
            rmRequest(cmd);
            break;

        default:
            break;
    }

    // Close the socket:
    close(socket_desc);

    free(cmd);

    return 0;
}
