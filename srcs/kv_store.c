#include "kv_store.h"
#include "kv_table.h"
#include "libs.h"

t_status_code   tb_create(char *filename)
{
    t_kv_table      *table;
    const char      *filepath;
    t_status_code   status;

    table = NULL;
    filepath = NULL;
    RETURN_ON_ERROR(kv_init_table(&table, 8));
    RETURN_ON_ERROR(construct_table_path(filename, DIR_DATA, &filepath));
    RETURN_ON_ERROR(kv_save_file(table, filepath));
    status = SUCCESS_CODE;
cleanup:
    if (filepath)
        free((void *)filepath);
    if (table)
        kv_free_table(table);
    return (status);    
}

t_status_code   tb_drop(char *filename)
{
    const char      *filepath;
    t_status_code   status;

    filepath = NULL;
    RETURN_ON_ERROR(construct_table_path(filename, DIR_DATA, &filepath));
    if (access(filepath, F_OK) != 0)
    {
        status = ERROR_TABLE_MISSING_CODE;
        goto cleanup;
    }
    if (remove(filepath) != 0)
    {
        status = ERROR_TABLE_DROP_CODE;
        goto cleanup;
    }
    status = SUCCESS_CODE;

cleanup:
    if (filepath)
        free((void *)filepath);
    return (status);
}

t_status_code   tb_rename(char *old, char *new)
{
    const char      *old_filepath;
    const char      *new_filepath;
    t_status_code   status;

    old_filepath = NULL;
    new_filepath = NULL;
    RETURN_ON_ERROR(construct_table_path(old, DIR_DATA, &old_filepath));
    RETURN_ON_ERROR(construct_table_path(new, DIR_DATA, &new_filepath));
    if (rename(old_filepath, new_filepath) != 0)
    {
        status = ERROR_TABLE_RENAME_CODE;
        goto cleanup;
    }
    status = SUCCESS_CODE;

cleanup:
    if (old_filepath)
        free((void *)old_filepath);
    if (new_filepath)
        free((void *)new_filepath);
    return (status);
}