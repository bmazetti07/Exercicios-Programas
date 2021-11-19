#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void type_prompt (char * prompt) {
    char * username = NULL;
    char * path = NULL;

    username = getlogin ();
    path = getcwd (path, 0);

    sprintf (prompt, "{%s@%s} ", username, path);
}

char * read_prompt (char * line_read) {
    char prompt[150]; 
    type_prompt (prompt);

    if (line_read) {
        free (line_read);
        line_read = (char *) NULL;
    }

    line_read = readline (prompt);

    if (line_read && *line_read)
        add_history (line_read);

    return (line_read);
}

int main (int argc, char **argv) {
    char * command;
    char * parameters;
    char * paramEP[3];
    static char * all_command = NULL;

    while (1) {
        all_command = read_prompt (all_command);
        command = strtok (all_command, " ");
        parameters = strtok (NULL, " ");

        if (strcmp (command, "mkdir") == 0)
            mkdir (parameters, 0777);

        else if (strcmp (command, "kill") == 0) {
            char * aux = strtok (NULL, " ");
            pid_t pid = atoi (aux);

            kill (pid, SIGKILL);
        }

        else if (strcmp (command, "ln") == 0) {
            char * file = strtok (NULL, " ");
            char * shortcut = strtok (NULL, " ");

            symlink (file, shortcut);
        }

        else {
            paramEP[0] = strtok (NULL, " ");
            paramEP[1] = strtok (NULL, " ");
            paramEP[2] = strtok (NULL, " ");

            char * argumentos[] = { " ", parameters, paramEP[0], paramEP[1], paramEP[2], NULL };
            char * envp[] = { NULL };

            if (fork() != 0) {
                waitpid(-1, NULL, 0);
            }
            
            else {
                execve (command, argumentos, envp);
            }
        }
    }
}