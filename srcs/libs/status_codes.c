#include "utils.h"
#include "status_codes.h"
#include "libs.h"

t_status	get_status(t_status_code code)
{
    size_t		i;
	t_status	status = {UNKNOWN_CODE, "unkown error", 4};
    struct {
        t_status_code	code;
        const char		*message;
		int				log_level;
    } status_table[] = {
        #define X(name, code, message, log_level) {code, message, log_level},
        STATUS_LIST
        #undef X
    };

    i = 0;
    while (i < sizeof(status_table) / sizeof(status_table[0]))
    {
        if (status_table[i].code == code)
		{
			status.code = status_table[i].code;
			status.message = status_table[i].message;
			status.log_level = status_table[i].log_level;
			return (status);
		}
        i++;
    }
	return (status);
}

#undef X

void    log_message(int fd, t_status_code code, ...)
{
    t_status status;
    va_list args;

    if (code == SUCCESS_CODE)
        fd = 2;
    va_start(args, code);
	status = get_status(code);
    logger(fd, (char *)status.message, args);
    va_end(args);
}