#ifndef STATUS_CODES_H
# define STATUS_CODES_H

#define STATUS_LIST \
	X(UNKNOWN, -1, "unknown error", 4) \
    X(SUCCESS, 0, "OK.", 2) \
    X(WARNING_KEY_EXISTS, 1, "Warning: Key already exists. Value will be updated.", 3) \
    X(ERROR_MEMORY_ALLOCATION, 2, "Error: Failed to allocate memory for key-value pair.", 4) \
    X(ERROR_MEMORY_REALLOCATION, 3, "Error: Failed to Reallocate memory.", 4) \
    X(ERROR_INVALID_INPUT, 4, "Error: Invalid key or value format.", 4) \
    X(ERROR_KEY_NOT_FOUND, 5, "Error: Key not found.", 4) \
    X(ERROR_FILE_OPEN, 6, "Error: Unable to open file for saving data.", 4) \
    X(ERROR_FILE_WRITE, 7, "Error: Failed to write data to file.", 4) \
    X(ERROR_FILE_READ, 8, "Error: Unable to open file for loading data.", 4) \
    X(ERROR_FILE_FORMAT, 9, "Error: Invalid file format. Unable to parse key-value pairs.", 4) \
    X(ERROR_FILE_CLOSE, 10, "Error: Failed to close file.", 4) \
    X(ERROR_INVALID_COMMAND, 11, "Error: Invalid command. Type 'help' for a list of available commands.", 4) \
    X(ERROR_FILE_SAVE, 12, "Error: Failed to save data to file before exit.", 4) \
    X(ERROR_UNEXPECTED_EXIT, 13, "Error: An unexpected error occurred during exit. Data may not have been saved.", 5) \
    X(ERROR_HELP_SYNTAX, 14, "Error: Incorrect usage of the command. Use 'help' to see the correct syntax.", 4) \
    X(ERROR_FILE_HEADER, 15, "Error: Invalid file header.", 4) \
    X(ERROR_READ_KEY, 16, "Error: Failed to read key from file.", 4) \
    X(ERROR_READ_VAL, 17, "Error: Failed to read value from file.", 4) \
    X(ERROR_READ_VAL_LEN, 18, "Error: Failed to read value length from file.", 4) \
    X(ERROR_INIT_TABLE, 19, "Error: Failed to initialize table.", 4) \
    X(ERROR_TABLE_MISSING, 20, "Error: The specified table could not be found.", 4) \
    X(ERROR_TABLE_DROP, 21, "Error: Failed to drop the specified table.", 4) \
    X(ERROR_TABLE_RENAME, 22, "Error: Failed to rename the specified table.", 4) \
    X(ERROR_UNEXPECTED, 22, "Error: An unexpected error occurred.", 5) \
    X(ERROR_COMMAND_TABLE_MISSING, 23, "Error: Command table has not been initialized.", 4) \
    X(ERROR_COMMAND_STRUCT_NOT_FOUND, 24, "Error: Command not found.", 4) \
    X(ERROR_COMMAND_NAME_MISSING, 25, "Error: Command name is missing.", 4) \
    X(ERROR_VALUE_TYPE_MISMATCH, 26, "Error: Value type mismatch.", 4) \
    X(ERROR_STR_TO_INT, 27, "Error: Failed to convert string to integer.", 4) \
    X(ERROR_MUTEX_INIT, 27, "Error: Failed to table mutex.", 4)
    
#define X(name, code, message, log_level) name##_CODE = code,

typedef enum {
    STATUS_LIST
} t_status_code;

#undef X

typedef struct s_status
{
    t_status_code	code;
    const char		*message;
	int				log_level;
} t_status;

t_status	get_status(t_status_code code);
void        log_message(int fd, t_status_code code, ...);

#endif