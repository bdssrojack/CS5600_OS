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
 * @param op operation type
 * @param file_path_1 In write mode: local file path; 
 *                    in get mode: remote file path; 
 *                    in remove mode: file path
 *
 * @param file_path_2 In write mode: remote file path;
 *                    in get mode: local file path;
 *                    in remove mode: empty
 */
typedef struct command {
    enum operation op;
    char file_path_1[FILE_PATH_LEN];
    char file_path_2[FILE_PATH_LEN];
} command_t;

#endif  // COMMAND_H