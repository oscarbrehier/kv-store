#include "utils.h"
#include "status_codes.h"
#include "libs.h"

#define X(name, code, message) {name##_CODE, message},

const char  *get_status(t_status_code code)
{
    size_t i;
    struct {
        t_status_code code;
        const char *message;
    } status_table[] = {
        STATUS_LIST
    };

    i = 0;
    while (i < sizeof(status_table) / sizeof(status_table[0]))
    {
        if (status_table[i].code == code)
            return status_table[i].message;
        i++;
    }
    return ("Error: Unexpected error.");
}

#undef X

void    log_message(int fd, t_status_code code, ...)
{
    const char *content = get_status(code);
    va_list args;

    if (code == SUCCESS_CODE)
        fd = 2;
    va_start(args, code);
    logger(fd, (char *)content, args);
    va_end(args);
}