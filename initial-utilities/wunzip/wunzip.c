#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int len;
    char c;
} rle_t;

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    char *filePath;
    FILE *f;
    int res;
    size_t readlen;
    for (int i = 1; i < argc; ++i) {
        filePath = argv[i];
        f = fopen(filePath, "r");
        if (f == NULL) {
            perror("wunzip: unable to open file\n");
            exit(1);
        }

        rle_t entry;
        // NOTE: can't use sizeof here because the compiler makes it 8 instead of 5
        while ((readlen = fread(&entry, 5, 1, f)) > 0) {
            for (int j = 0; j < entry.len; ++j) {
                printf("%c", entry.c);
            }
        }

        res = fclose(f);
        if (res != 0) {
            perror("wunzip: unable to close file\n");
            exit(1);
        }
    }

    return 0;
}
