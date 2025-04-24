#include "cli.h"
#include "kv_store.h"

static void handle_set(t_kv_store *store, int argc, char **argv) {
    (void)store;
    (void)argc;
    (void)argv;
    logger(2, "pong");
}

// This function is called during initialization
void register_set_command(void) {
    static t_command cmd = {
        .name = "set",
        .usage = "set <key> <value>",
        .description = "Set a key-value pair in the current table",
        .handler = handle_set,
        .flags = NO_FLAG
    };
    
    register_command(&cmd);
}