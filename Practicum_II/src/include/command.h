/**
 * @file command.h
 * @author Zhongshi Luo
 * @brief data structure for parsing command line argument
 * @version 0.1
 * @date 2024-04-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef COMMAND_H
#define COMMAND_H

#define FILE_PATH_LEN 256

typedef enum operation { WRITE, GET, RM } operation_t;

/**
 * @brief data structure for parsing command line argument
 *
 */
typedef struct command {
    enum operation op;
    char local_file_path[FILE_PATH_LEN];
    char remote_file_path[FILE_PATH_LEN];
} command_t;

#endif  // COMMAND_H