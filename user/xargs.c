#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("xargs usage: xargs <arg1> [<args>...]\n");
        exit(0);
    }
    char *program_name = argv[1];

    char *args[MAXARG];
    memset(args, 0, sizeof(args));

    int cnt = 0;
    for (int i = 1; i < argc; i++)
    {
        args[cnt] = malloc(sizeof(char) * strlen(argv[i]));
        strcpy(args[cnt], argv[i]);
        cnt++;
    }

    char buf[512];
    int cursor = 0;

    while (read(0, &buf[cursor], 1))
    {
        if (buf[cursor] == '\n')
        {
            buf[cursor] = 0;
            args[cnt] = malloc(sizeof(buf));
            strcpy(args[cnt], buf);
            cursor = 0;
            int pid = fork();
            if (pid == 0)
            {
                exec(program_name, (char **)args);
                exit(0);
            }
            else
            {
                wait(0);
            }
        }
        cursor++;
    }
    exit(0);
}
