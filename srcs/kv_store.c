#include "kv_store.h"
#include "kv_table.h"
#include "libs.h"

t_status   tb_create(char *filename)
{
    t_kv_table		*table;
    const char		*filepath;
    t_status		status;

    table = NULL;
    filepath = NULL;
    RETURN_ON_ERROR(kv_init_table(&table, 8));
    RETURN_ON_ERROR(construct_table_path(filename, DIR_DATA, &filepath));
    RETURN_ON_ERROR(kv_save_file(table, filepath));
    status = status_create(0, SUCCESS, LOG_INFO);
cleanup:
    if (filepath)
        free((void *)filepath);
    if (table)
        kv_free_table(table);
    return (status);    
}

t_status   tb_drop(char *filename)
{
    const char		*filepath;
    t_status		status;

    filepath = NULL;
    RETURN_ON_ERROR(construct_table_path(filename, DIR_DATA, &filepath));
    if (access(filepath, F_OK) != 0)
    {
		status = status_create(-1, ERROR_TABLE_MISSING, LOG_ERROR);
        goto cleanup;
    }
    if (remove(filepath) != 0)
    {
        status = status_create(-1, ERROR_TABLE_DROP, LOG_ERROR);
        goto cleanup;
    }
    status = status_create(0, SUCCESS, LOG_INFO);
cleanup:
    if (filepath)
        free((void *)filepath);
    return (status);
}

t_status   tb_rename(char *old, char *new)
{
    const char		*old_filepath;
    const char		*new_filepath;
    t_status		status;

    old_filepath = NULL;
    new_filepath = NULL;
    RETURN_ON_ERROR(construct_table_path(old, DIR_DATA, &old_filepath));
    RETURN_ON_ERROR(construct_table_path(new, DIR_DATA, &new_filepath));
    if (rename(old_filepath, new_filepath) != 0)
    {
		status = status_create(-1, ERROR_TABLE_RENAME, LOG_ERROR);
        goto cleanup;
    }
    status = status_create(0, SUCCESS, LOG_INFO);
cleanup:
    if (old_filepath)
        free((void *)old_filepath);
    if (new_filepath)
        free((void *)new_filepath);
    return (status);
}