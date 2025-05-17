#ifndef STATUS_CODES_H
# define STATUS_CODES_H

// == Log Levels == 
// DEBUG (1)
// INFO (2)
// WARNING (3) 
// ERROR (4)
// FATAL (5)
    
#define LOG_DEBUG 1
#define LOG_INFO 2
#define LOG_WARNING 3
#define LOG_ERROR 4
#define LOG_FATAL 5
#define LOG_NOLOG 6

// #define SUCCESS "OK"
// #define ERROR_UNKNOWN "unknown"

// #define ERROR_MEMORY_ALLOCATION "failed to allocate memory"
// #define ERROR_MEMORY_REALLOCATION "failed to reallocate memory"
// #define ERROR_MUTEX_INIT "failed to initialize table mutex"

// #define ERROR_STR_INT "failed to convert string to integer"

// #define ERROR_FILE_OPEN "unable to open file or directory"
// #define ERROR_FILE_READ "unable to read file"
// #define ERROR_FILE_CLOSE "unable to close file"
// #define ERROR_FILE_WRITE "unable to write to file"

// #define ERROR_COMMAND_TABLE_MISSING "command table has not been initialized"
// #define ERROR_COMMAND_STRUCT_NOT_FOUND "command not found"
// #define ERROR_COMMAND_NAME_MISSING "command name is missing"

// #define ERROR_INIT_TABLE "failed to initialize table"
// #define ERROR_TABLE_MISSING "the specified table could not be found"
// #define ERROR_TABLE_DROP "failed to drop the specified table"
// #define ERROR_TABLE_RENAME "failed to rename table"

// #define WARNING_KEY_EXISTS "key already exists, value will be updated"
// #define ERROR_KEY_NOT_FOUND "key not found"
// #define ERROR_VALUE_TYPE_MISMATCH "value type mismatch"
// #define ERROR_READ_KEY "failed to read key from file"
// #define ERROR_READ_VAL "failed to read value from file"
// #define ERROR_READ_VAL_LEN "failed to read value length from file"

typedef enum e_status_code {
    SUCCESS,
    STATUS_UNKNOWN,

    ERROR_MEMORY_ALLOCATION,
    ERROR_MEMORY_REALLOCATION,
    ERROR_MUTEX_INIT,
    ERROR_NULL_POINTER,

    ERROR_STR_INT,

    ERROR_FILE_OPEN,
    ERROR_FILE_READ,
    ERROR_FILE_CLOSE,
    ERROR_FILE_WRITE,
    ERROR_FILE_HEADER,

    ERROR_COMMAND_TABLE_MISSING,
    ERROR_COMMAND_STRUCT_NOT_FOUND,
    ERROR_COMMAND_NAME_MISSING,

    ERROR_INIT_TABLE,
    ERROR_TABLE_MISSING,
    ERROR_TABLE_DROP,
    ERROR_TABLE_RENAME,

    WARNING_KEY_EXISTS,
    ERROR_KEY_NOT_FOUND,
    ERROR_VALUE_TYPE_MISMATCH,
    ERROR_READ_KEY,
    ERROR_READ_VAL,
    ERROR_READ_VAL_LEN,
    ERROR_READ_TYPE,

    AUTH_USERNAME_TAKEN,
    AUTH_INTERNAL_ERROR,
    AUTH_INVALID_CREDENTIALS,

    STATUS_CODE_COUNT  // always keep this last for bounds checking
} t_status_code;

extern char *status_messages[STATUS_CODE_COUNT];

typedef struct s_status
{
    int				exit;
	int				log_level;
    t_status_code   code;
} t_status;

t_status	status_create(int exit_code, t_status_code code, int log_level);
void	    status_log(int fd, t_status_code code, ...);

#endif