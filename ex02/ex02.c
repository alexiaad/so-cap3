/*
 * Problema 2
 * Imbunatatiti interpretorul de la problema 1 astfel incat sa recunoasca
 * < si > ca redirectari ale intrarii si iesirii, realizand efectiv
 * redirectarile.
 *   Exemple:  ECOU salut > a.txt
 *             sort < nume.txt
 *
 * Utilizare: ex02
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
    char line[1024];

    for (;;)
    {
        char *argv[128];
        char input_file[1024] = "";
        char output_file[1024] = "";
        int argc = 0;
        int i = 0;
        int saved_stdin = -1;
        int saved_stdout = -1;
        int redirection_error = 0;
        int j;

        printf("> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        line[strcspn(line, "\r\n")] = '\0';

        while (line[i] != '\0')
        {
            while (line[i] == ' ')
                i++;
            if (line[i] == '\0')
                break;

            if (line[i] == '<' || line[i] == '>')
            {
                char operation = line[i++];
                char *destination;

                while (line[i] == ' ')
                    i++;
                destination = &line[i];
                while (line[i] != '\0' && line[i] != ' ' &&
                       line[i] != '<' && line[i] != '>')
                    i++;
                if (line[i] != '\0')
                    line[i++] = '\0';

                if (*destination == '\0')
                {
                    redirection_error = 1;
                }
                else if (operation == '<')
                {
                    if (strlen(destination) >= sizeof(input_file))
                        redirection_error = 1;
                    else
                        strcpy(input_file, destination);
                }
                else
                {
                    if (strlen(destination) >= sizeof(output_file))
                        redirection_error = 1;
                    else
                        strcpy(output_file, destination);
                }
                continue;
            }

            if (argc >= 127)
            {
                redirection_error = 1;
                break;
            }
            argv[argc++] = &line[i];
            while (line[i] != '\0' && line[i] != ' ' &&
                   line[i] != '<' && line[i] != '>')
                i++;
            if (line[i] == ' ')
                line[i++] = '\0';
            else if (line[i] == '<' || line[i] == '>')
            {
                char operation = line[i];
                char *destination;

                line[i++] = '\0';
                while (line[i] == ' ')
                    i++;
                destination = &line[i];
                while (line[i] != '\0' && line[i] != ' ' &&
                       line[i] != '<' && line[i] != '>')
                    i++;
                if (line[i] != '\0')
                    line[i++] = '\0';

                if (*destination == '\0')
                {
                    redirection_error = 1;
                }
                else if (operation == '<')
                {
                    if (strlen(destination) >= sizeof(input_file))
                        redirection_error = 1;
                    else
                        strcpy(input_file, destination);
                }
                else
                {
                    if (strlen(destination) >= sizeof(output_file))
                        redirection_error = 1;
                    else
                        strcpy(output_file, destination);
                }
            }
        }
        argv[argc] = NULL;

        if (argc == 0)
        {
            if (redirection_error)
                puts("Comanda incorecta");
            continue;
        }

        if (!redirection_error && input_file[0] != '\0')
        {
            int fd = _open(input_file, _O_RDONLY);
            if (fd == -1)
                redirection_error = 1;
            else
            {
                saved_stdin = _dup(_fileno(stdin));
                if (saved_stdin == -1 || _dup2(fd, _fileno(stdin)) == -1)
                    redirection_error = 1;
                _close(fd);
            }
        }

        if (!redirection_error && output_file[0] != '\0')
        {
            int fd = _open(output_file,
                           _O_WRONLY | _O_CREAT | _O_TRUNC,
                           _S_IREAD | _S_IWRITE);
            if (fd == -1)
            {
                redirection_error = 1;
            }
            else
            {
                saved_stdout = _dup(_fileno(stdout));
                if (saved_stdout == -1 || _dup2(fd, _fileno(stdout)) == -1)
                    redirection_error = 1;
                _close(fd);
            }
        }

        if (redirection_error)
        {
            puts("Comanda incorecta");
        }
        else if (_stricmp(argv[0], "DIR") == 0 ||
                 _stricmp(argv[0], "TIME") == 0 ||
                 _stricmp(argv[0], "DATE") == 0)
        {
            char command[1024] = "";
            int command_error = 0;
            for (j = 0; j < argc; j++)
            {
                if (j > 0)
                {
                    if (strlen(command) + 1 >= sizeof(command))
                        command_error = 1;
                    else
                        strcat(command, " ");
                }
                if (!command_error &&
                    strlen(command) + strlen(argv[j]) >= sizeof(command))
                    command_error = 1;
                if (!command_error)
                    strcat(command, argv[j]);
            }
            if (command_error ||
                _spawnlp(_P_WAIT, "cmd.exe", "cmd.exe", "/C", command, NULL) == -1)
                puts("Comanda incorecta");
        }
        else if (_stricmp(argv[0], "COMANDA") == 0)
        {
            if (_spawnlp(_P_WAIT, "cmd.exe", "cmd.exe", NULL) == -1)
                puts("Comanda incorecta");
        }
        else if (_stricmp(argv[0], "ECOU") == 0)
        {
            for (j = 1; j < argc; j++)
            {
                if (j > 1)
                    putchar(' ');
                fputs(argv[j], stdout);
            }
            putchar('\n');
        }
        else
        {
            if (_spawnvp(_P_WAIT, argv[0], argv) == -1)
                puts("Comanda incorecta");
        }

        fflush(stdout);
        if (saved_stdout != -1)
        {
            _dup2(saved_stdout, _fileno(stdout));
            _close(saved_stdout);
        }
        if (saved_stdin != -1)
        {
            _dup2(saved_stdin, _fileno(stdin));
            _close(saved_stdin);
        }
        fflush(stdout);
    }

    return 0;
}
