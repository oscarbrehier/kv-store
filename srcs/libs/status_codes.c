#include "utils.h"
#include "status_codes.h"
#include "libs.h"

#include "status_codes.h"

char *status_messages[STATUS_CODE_COUNT] = {
    [SUCCESS]                         = "OK",
    [STATUS_UNKNOWN]                  = "unknown",

    [ERROR_MEMORY_ALLOCATION]         = "failed to allocate memory",
    [ERROR_MEMORY_REALLOCATION]       = "failed to reallocate memory",
    [ERROR_MUTEX_INIT]                = "failed to initialize table mutex",
    [ERROR_NULL_POINTER]              = "null pointer received",

    [ERROR_STR_INT]                   = "failed to convert string to integer",

    [ERROR_FILE_OPEN]                 = "unable to open file or directory",
    [ERROR_FILE_READ]                 = "unable to read file",
    [ERROR_FILE_CLOSE]                = "unable to close file",
    [ERROR_FILE_WRITE]                = "unable to write to file",
    [ERROR_FILE_HEADER]               = "invalid key-value file header",

    [ERROR_COMMAND_TABLE_MISSING]     = "command table has not been initialized",
    [ERROR_COMMAND_STRUCT_NOT_FOUND]  = "command not found",
    [ERROR_COMMAND_NAME_MISSING]      = "command name is missing",

    [ERROR_INIT_TABLE]                = "failed to initialize table",
    [ERROR_TABLE_MISSING]             = "the specified table could not be found",
    [ERROR_TABLE_DROP]                = "failed to drop the specified table",
    [ERROR_TABLE_RENAME]              = "failed to rename table",

    [WARNING_KEY_EXISTS]              = "key already exists, value will be updated",
    [ERROR_KEY_NOT_FOUND]             = "key not found",
    [ERROR_VALUE_TYPE_MISMATCH]       = "value type mismatch",
    [ERROR_READ_KEY]                  = "failed to read key from file",
    [ERROR_READ_VAL]                  = "failed to read value from file",
    [ERROR_READ_VAL_LEN]              = "failed to read value length from file",

    [AUTH_USERNAME_TAKEN]             = "this username is already taken",
    [AUTH_INTERNAL_ERROR]             = "an error occured, please try again",
    [AUTH_INVALID_CREDENTIALS]		  = "invalid username or password"
};

void	status_log(int fd, t_status_code code, ...)
{
	va_list		args;

	if (code == SUCCESS)
		fd = 2;
	va_start(args, code);
	logger(fd, (char *)status_messages[code], args);
	va_end(args);
}

t_status	status_create(int exit_code, t_status_code code, int log_level)
{
	t_status	status;

	status.exit = exit_code;
	status.code = code;
	status.log_level = log_level;
	return (status);
}