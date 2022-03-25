
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char const *path, char const *target)
{
    int fd;
    struct dirent de;
    struct stat st;

    char buf[512], *p;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_DIR)
    {
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0)
            {
                continue;
            }
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }

            if (st.type == T_FILE)
            {
                if (strcmp(p, target) == 0)
                {
                    printf("%s\n", buf);
                }
            }
            else if (st.type == T_DIR)
            {
                find(buf, target);
            }
        }
    }
    close(fd);
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(1, "find usage: find <dir> <filename>");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
