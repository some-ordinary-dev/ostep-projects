#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_LINE_LEN 1000

int main(int argc, char **argv) {
    char line[MAX_LINE_LEN];
    char *filePath;
    int res;
    FILE *f;

    for (int i = 1; i < argc; ++i) {
        filePath = argv[i];
        f = fopen(filePath, "r");
        if (f == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }

        while (fgets(line, MAX_LINE_LEN, f) != NULL)
            printf("%s", line);

        res = ferror(f);
        if (res != 0) {
            perror("wcat: error while reading line from file\n");
            exit(1);
        }

        res = fclose(f);
        if (res != 0) {
            perror("wcat: error while closing file\n");
            exit(1);
        }
    }

    return 0;
}
