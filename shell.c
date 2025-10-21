#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>

extern char **environ;

// Join dir and name into out. Assumes out has enough space.
static void join_path(char *out, const char *dir, const char *name) {
    size_t dlen = strlen(dir);
    if (dlen > 0 && dir[dlen - 1] == '/') {
        snprintf(out, PATH_MAX, "%s%s", dir, name);
    } else {
        snprintf(out, PATH_MAX, "%s/%s", dir, name);
    }
}

// Return a malloc'ed string with the resolved executable path, or NULL if not found.
static char *resolve_command(const char *cmd) {
    if (cmd == NULL || *cmd == '\0') return NULL;

    // Case 1: absolute path
    if (cmd[0] == '/') {
        if (access(cmd, X_OK) == 0) {
            return strdup(cmd);
        }
        return NULL;
    }

    // Case 2: contains slash → relative path
    if (strchr(cmd, '/') != NULL) {
        if (access(cmd, X_OK) == 0) {
            return strdup(cmd);
        }
        return NULL;
    }

    // Case 3: search PATH
    const char *path_env = getenv("PATH");
    if (!path_env) return NULL;

    char *paths = strdup(path_env);
    if (!paths) return NULL;

    char candidate[PATH_MAX];
    char *saveptr = NULL;
    for (char *dir = strtok_r(paths, ":", &saveptr);
         dir != NULL;
         dir = strtok_r(NULL, ":", &saveptr)) {
        if (*dir == '\0') {
            if (snprintf(candidate, sizeof(candidate), "./%s", cmd) >= 0 &&
                access(candidate, X_OK) == 0) {
                free(paths);
                return strdup(candidate);
            }
            continue;
        }
        join_path(candidate, dir, cmd);
        if (access(candidate, X_OK) == 0) {
            free(paths);
            return strdup(candidate);
        }
    }
    free(paths);
    return NULL;
}

int main(void) {
    char line[1024];
    char *argv[64];

    while (1) {
        printf("osshell> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;

        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        // Simple parser: split by space
        int argc = 0;
        char *token = strtok(line, " ");
        while (token && argc < 63) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        argv[argc] = NULL;
        if (argc == 0) continue;

        if (strcmp(argv[0], "exit") == 0) break;

        // Resolve executable path
        char *exe_path = resolve_command(argv[0]);
        if (!exe_path) {
            fprintf(stderr, "command not found: %s\n", argv[0]);
            continue;
        }

        pid_t pid = fork();
        if (pid == 0) {
            execve(exe_path, argv, environ);
            perror("execve");
            _exit(127);
        } else if (pid > 0) {
            int status = 0;
            waitpid(pid, &status, 0);
        } else {
            perror("fork");
        }
        free(exe_path);
    }

    return 0;
}

