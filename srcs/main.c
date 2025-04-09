#include "kv_store.h"

t_kv_pair *hashTable[TABLE_SIZE] = {NULL};

void test_kv_store()
{
    kv_set(hashTable, "clé4", "valeur4");
	kv_set(hashTable, "clé5", "valeur5");
	kv_set(hashTable, "clé6", "valeur6");
}

void kv_free_table(t_kv_pair **table)
{
    int i;
    t_kv_pair *current;
    t_kv_pair *next;

    i = 0;
    while (i < TABLE_SIZE)
    {
        current = table[i];
        while (current != NULL)
        {
            next = current->next;
            free(current);
            current = next;
        }
        table[i] = NULL;
        i++;
    }
}

int	main(void)
{
	// test_kv_store();
	// printf("%s\n", kv_get(hashTable, "clé4"));
	// printf("%s\n", kv_get(hashTable, "clé5"));
	// // kv_delete(hashTable, "clé5");
	// printf("%s\n-------\n", kv_get(hashTable, "clé5") == NULL ? "NULL" : "found");
	// kv_save_to_file(hashTable, "data.txt");

	// kv_load_from_file(hashTable, "data.txt");

	// printf("== %s == \n", kv_get(hashTable, "clé4"));

	// printf("list");

	// kv_list(hashTable);

	run_cli(hashTable);
	kv_free_table(hashTable);
	return (0);
};