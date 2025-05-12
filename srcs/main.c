#include "kv_store.h"
#include "kv_table.h"
#include "status_codes.h"
#include "libs.h"
#include "shell.h"

int	main(void)
{
    t_kv_store      *store;

    store = (t_kv_store *)malloc(sizeof(t_kv_store));
    if (!store)
    {
        logger(1, status_messages[ERROR_MEMORY_REALLOCATION]);
        return (1);
    }

    store->table = NULL;
    store->name[0] = '\0';
    if (init_shell() == 0)
    {
        run_shell(store);
    }
	kv_free_table(store->table);
    free(store);
	return (0);
};