    #include "kv_store.h"

    void    handle_set(t_kv_pair **table, int argc, char **argv)
    {
        if (argc == 3)
            kv_set(table, argv[1], argv[2]);
        else
            fn_putstr("Usage: set <key> <value>\n");
    }

    void    handle_get(t_kv_pair **table, int argc, char **argv)
    {
        // const char *value;
        if (argc == 2)
        {
            // value =  kv_get(table, argv[1]);
            printf("%s\n", kv_get(table, argv[1]) ? kv_get(table, argv[1]) : "Error: Key not found");
        }
        else
            fn_putstr("Usage: get <key>\n");
    }

    void    handle_list(t_kv_pair **table, int argc, char **argv)
    {
        (void)argc;
        (void)argv;
        kv_list(table);
    }

    void	exec_cmd(t_kv_pair **table, int argc, char **argv)
    {
        long unsigned int i;
        t_command commands[] = {
            {"set", handle_set},
            {"get", handle_get},
            {"list", handle_list}
        };

        i = 0;
        while (i < sizeof(commands) / sizeof(commands[0]))
        {
            if (fn_strcmp(argv[0], commands[i].cmd) == 0)
            {
                commands[i].handler(table, argc, argv);
                return ;
            }
            i++;
        }
    }