#ifndef KV_ERROR_H
#define KV_ERROR_H

#define SUCCESS 0
#define ERROR_KEY_EXISTS 1
#define ERROR_MEMORY_ALLOCATION 2
#define ERROR_INVALID_INPUT 3
#define ERROR_KEY_NOT_FOUND 4
#define ERROR_FILE_OPEN 5
#define ERROR_FILE_WRITE 6
#define ERROR_FILE_READ 7
#define ERROR_FILE_FORMAT 8
#define ERROR_INVALID_COMMAND 9
#define ERROR_FILE_SAVE 10
#define ERROR_UNEXPECTED_EXIT 11
#define ERROR_HELP_SYNTAX 12

extern const char *error_messages[];

void    logger(int code);

#endif