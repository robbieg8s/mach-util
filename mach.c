#include <mach/mach_time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*command_t)(const char *mach_name, const char *command_name, int argc, const char ** argv);

typedef struct command_info {
    const char * name;
    command_t command;
} command_info_t;

/**
 * Handy stub.
 */
int unimplemented(const char *mach_name, const char *command_name, int argc, const char ** argv) {
    fprintf(stderr, "Command '%s' is unimplemented\n", command_name);
    return EXIT_FAILURE;
}

int command_absolute_time (const char *mach_name, const char *command_name, int argc, const char ** argv) {
    if (argc != 0) {
        fprintf(stderr, "Usage: %s %s\n", mach_name, command_name);
        return EXIT_FAILURE;
    } else {
        printf("%llu\n", mach_absolute_time());
        return EXIT_SUCCESS;
    }
}

int command_approximate_time (const char *mach_name, const char *command_name, int argc, const char ** argv) {
    if (argc != 0) {
        fprintf(stderr, "Usage: %s %s\n", mach_name, command_name);
        return EXIT_FAILURE;
    } else {
        printf("%llu\n", mach_approximate_time());
        return EXIT_SUCCESS;
    }
}

int command_continuous_approximate_time(const char *mach_name, const char *command_name, int argc, const char ** argv) {
    if (argc != 0) {
        fprintf(stderr, "Usage: %s %s\n", mach_name, command_name);
        return EXIT_FAILURE;
    } else {
        printf("%llu\n", mach_continuous_approximate_time());
        return EXIT_SUCCESS;
    }
}

int command_continuous_time (const char *mach_name, const char *command_name, int argc, const char ** argv) {
    if (argc != 0) {
        fprintf(stderr, "Usage: %s %s\n", mach_name, command_name);
        return EXIT_FAILURE;
    } else {
        printf("%llu\n", mach_continuous_time());
        return EXIT_SUCCESS;
    }
}

int command_timebase_info(const char *mach_name, const char *command_name, int argc, const char ** argv) {
    if (argc != 0) {
        fprintf(stderr, "Usage: %s %s\n", mach_name, command_name);
        return EXIT_FAILURE;
    } else {
        mach_timebase_info_data_t info;
        kern_return_t result = mach_timebase_info(&info);
        if (KERN_SUCCESS != result) {
            fprintf(stderr,"mach_timebase_info failed: %d\n", result);
        } else {
            printf("numer=%d\ndenom=%d\n", info.numer, info.denom);
        }
        return EXIT_SUCCESS;
    }
}

/**
 * Maintain sorted.
 */
command_info_t g_commands[] = {
    { "absolute_time", command_absolute_time },
    { "approximate_time", command_approximate_time },
    { "continuous_approximate_time", command_continuous_approximate_time },
    { "continuous_time", command_continuous_time },
    { "timebase_info", command_timebase_info },
};

const size_t k_command_info_size = sizeof(g_commands[0]);
const size_t k_command_count = sizeof(g_commands)/k_command_info_size;

/**
 * Signature designed for bsearch.
 */
int command_compare_name(const void * key, const void * command_info) {
    return strcmp((const char *) key, ((const command_info_t *) command_info)->name);
}

void list_commands() {
    fprintf(stderr, "  Available commands:\n");
    for (int i = 0; i < k_command_count; ++i) {
        fprintf(stderr, "    %s\n", g_commands[i].name);
    }
}

int main(int argc, const char ** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [...]\n", *argv);
        list_commands();
        return EXIT_FAILURE;
    } else {
        const char * mach_name = argv[0];
        const char * command_name = argv[1];
        void * found = bsearch(command_name, &g_commands[0], k_command_count, k_command_info_size, command_compare_name);
        if (!found) {
            fprintf(stderr, "Command '%s' not found\n", command_name);
            list_commands();
            return EXIT_FAILURE;
        } else {
            const command_info_t * command_info = (command_info_t *) found;
            return (*(command_info->command))(mach_name, command_name, argc - 2, argv + 2);
        }
    }
}
