#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "make_argv.h"
#include "mem_region_list.h"

struct cmd_entry {
    const char *cmd;                // Command string.
    void (*proc)(int32_t, char **); // Command Procedure
    const char *desc;               // Description message. (NULL indicate shortcut command.)
};

static int32_t command_loop(void);
static const struct cmd_entry *find_command_entry(const char *cmd);
static void quit_command(int32_t argc, char **argv);
static void help_command(int32_t argc, char **argv);
static void dump_command(int32_t argc, char **argv);
static void assign_command(int32_t argc, char **argv);
static void release_command(int32_t argc, char **argv);

/**
 * Application run flag.
 * true : Runing.
 * false : Exit.
 */
static bool IsAppRun;

static const struct cmd_entry CommandEntries[] = {
    {"dump", dump_command, "Dump entries."},
    {"assign", assign_command, "Assign memory."},
    {"release", release_command, "Release memory."},
    {"help", help_command, "Print help message."},
    {"quit", quit_command, "Quit application."},
    {"?", help_command, NULL},
    {"q", quit_command, NULL},
};
static const int32_t CommandEntryCount = sizeof(CommandEntries) / sizeof(struct cmd_entry);

/**
 * Memory region list.
 */
static struct mem_region_list MemRegionList;

static struct mem_region MemRegions[16u];

/**
 * Entry point of this application.
 *
 * @param ac Count of arguments.
 * @param av Arguments.
 * @retval EXIT_SUCCESS application complete successfuly.
 * @retval EXIT_FAILURE application exit with error.
 */
int main(int ac, char **av) {
    int res = 0L;

    uint32_t mem_area_size = 512u;
    if (ac >= 2L) {
        mem_area_size = (uint32_t)(strtoul(av[1], NULL, 0));
    }
    void *mem_area = malloc(mem_area_size);
    if (mem_area == NULL) {
        fprintf(stderr, "Could not allocate meory. [len=%u]\n", mem_area_size);
    } else {
        mem_region_list_init(&MemRegionList, mem_area, mem_area_size, MemRegions, 16L);
        res = command_loop();
        mem_region_list_destroy(&MemRegionList);
        free(mem_area);
    }

    return (res == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/**
 * Process command loop.
 *
 * @retval 0 0 returned.
 */
static int32_t command_loop(void) {
    char cmd_buf[256u];
    char *argv[32u];

    IsAppRun = true;

    while (IsAppRun) {
        fputs("> ", stdout);
        fflush(stdout);
        if (fgets(cmd_buf, sizeof(cmd_buf), stdin) == NULL) {
            break;
        }
        cmd_buf[sizeof(cmd_buf) - 1u] = '\0';
        size_t len = strlen(cmd_buf);
        if ((len > 0u) && (cmd_buf[len - 1u] == '\n')) {
            cmd_buf[len - 1u] = '\0';
            len--;
        }
        if ((len > 0u) && (cmd_buf[len - 1u] == '\r')) {
            cmd_buf[len - 1u] = '\0';
            len--;
        }
        if (len > 0u) {
            int32_t args = make_argv(cmd_buf, argv, 32L);
            if (args > 0L) {
                const struct cmd_entry *entry = find_command_entry(argv[0u]);
                if (entry != NULL) {
                    entry->proc(args, argv);
                } else {
                    printf("%s not found.\n", argv[0u]);
                }
            }
        }
    }

    return 0L;
}

/**
 * Find command entry at CommandEntries.
 *
 * @param cmd Command string.
 * @retval Not NULL
 *           Found entry pointer.
 * @retval NULL
 *           Entry not found.
 */
static const struct cmd_entry *find_command_entry(const char *cmd) {
    for (int32_t i = 0L; i < CommandEntryCount; i++) {
        if (strcmp(CommandEntries[i].cmd, cmd) == 0L) {
            return &(CommandEntries[i]);
        }
    }

    return NULL;
}
/**
 * Process quit command.
 *
 * @param argc Count of arguments.
 * @param argv Arguments.
 */
static void quit_command(int32_t argc, char **argv) {
    IsAppRun = false;
    return;
}
/**
 * Process help command.
 *
 * @param argc Count of arguments.
 * @param argv Arguments.
 */
static void help_command(int32_t argc, char **argv) {
    if (argc == 2L) {
        const struct cmd_entry *entry = find_command_entry(argv[1u]);
        if (entry != NULL) {
            if (entry->desc != NULL) {
                printf("%s %s\n", entry->cmd, entry->desc);
            } else {
                printf("%s (short cut comand)\n", entry->cmd);
            }
        } else {
            printf("%s not found.\n", argv[1u]);
        }
    } else if (argc <= 1L) {
        for (int32_t i = 0L; i < CommandEntryCount; i++) {
            const struct cmd_entry *entry = &(CommandEntries[i]);
            if (entry->desc != NULL) {
                printf("%s %s\n", entry->cmd, entry->desc);
            }
        }
    } else {
        printf("usage:\n");
        printf("    help [command$]\n");
    }
}
/**
 * Process dump command.
 *
 * @param argc Count of arguments.
 * @param argv Arguments.
 */
static void dump_command(int32_t argc, char **argv) {
    mem_size_t used_size = mem_region_list_get_used(&MemRegionList);
    mem_size_t free_size = mem_region_list_get_free(&MemRegionList);
    printf("Used = %u\n", used_size);
    {
        const struct mem_region *pregion = MemRegionList.used.next;
        while (pregion != &(MemRegionList.used)) {
            printf("  %p %u\n", pregion->address, pregion->length);
            pregion = pregion->next;
        }
    }

    printf("Free = %u\n", free_size);
    {
        const struct mem_region *pregion = MemRegionList.free.next;
        while (pregion != &(MemRegionList.free)) {
            printf("  %p %u\n", pregion->address, pregion->length);
            pregion = pregion->next;
        }
    }
    printf("Total = %u\n", used_size + free_size);

    return;
}

/**
 * Process assign command.
 *
 * @param argc Count of arguments.
 * @param argv Arguments.
 */
static void assign_command(int32_t argc, char **argv) {
    if (argc != 2L) {
        printf("usage:\n");
        printf("  assign length#\n");
        return;
    }
    uint32_t length = (uint32_t)(strtoul(argv[1u], NULL, 0));
    void *ptr = mem_region_list_assign(&MemRegionList, length);
    if (ptr == NULL) {
        printf("Could not assign memory. [length=%u]\n", length);
    } else {
        printf("%p\n", ptr);
    }
}
/**
 * Process release command.
 *
 * @param argc Count of arguments.
 * @param argv Arguments.
 */
static void release_command(int32_t argc, char **argv) {
    if (argc != 2L) {
        printf("usage:\n");
        printf("  release address#\n");
        return;
    }
    char *p;
    void *ptr = (void *)(strtoull(argv[1u], &p, 0));
    if ((ptr == NULL) || (p == NULL) || (*p != '\0')) {
        printf("Invalid argument. [%s]\n", argv[1u]);
        return;
    }

    mem_region_list_release(&MemRegionList, ptr);

    return;
}
