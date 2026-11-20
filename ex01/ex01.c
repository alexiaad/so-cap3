/*
 * Problema 1
 * Realizati un interpretor de comenzi propriu:
 *   - recunoaste comenzile DIR, TIME, DATE, COMANDA, ECOU;
 *   - DIR, TIME, DATE sunt transmise interpretorului sistemului (cmd);
 *   - COMANDA lanseaza interpretorul sistemului pana se tasteaza EXIT;
 *   - ECOU afiseaza toate argumentele primite;
 *   - orice alta comanda e lansata ca program (spawnvp: directorul
 *     curent, apoi PATH); daca nu reuseste se afiseaza "Comanda incorecta";
 *   - separatorul dintre cuvinte este doar spatiul.
 *
 * Utilizare: ex01
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>

int main(void)
{
    char line[1024];

    for (;;)
    {
        char *argv[128];
        char *p;
        int argc = 0;
        char command[1024];

        printf("> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        line[strcspn(line, "\r\n")] = '\0';
        strcpy(command, line);

        p = line;
        while (*p != '\0')
        {
            while (*p == ' ')
                p++;
            if (*p == '\0')
                break;

            argv[argc++] = p;
            while (*p != '\0' && *p != ' ')
                p++;
            if (*p == ' ')
                *p++ = '\0';
        }
        argv[argc] = NULL;

        if (argc == 0)
            continue;

        if (_stricmp(argv[0], "DIR") == 0 ||
            _stricmp(argv[0], "TIME") == 0 ||
            _stricmp(argv[0], "DATE") == 0)
        {
            if (_spawnlp(_P_WAIT, "cmd.exe", "cmd.exe", "/C", command, NULL) == -1)
                puts("Comanda incorecta");
        }
        else if (_stricmp(argv[0], "COMANDA") == 0)
        {
            if (_spawnlp(_P_WAIT, "cmd.exe", "cmd.exe", NULL) == -1)
                puts("Comanda incorecta");
        }
        else if (_stricmp(argv[0], "ECOU") == 0)
        {
            for (int i = 1; i < argc; i++)
            {
                if (i > 1)
                    putchar(' ');
                fputs(argv[i], stdout);
            }
            putchar('\n');
        }
        else
        {
            if (_spawnvp(_P_WAIT, argv[0], argv) == -1)
                puts("Comanda incorecta");
        }
    }

    return 0;
}
