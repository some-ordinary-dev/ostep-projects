#include <stdio.h>
#include <stdlib.h>

void flush(char c, int runlen) {
    if (c > 0 && runlen > 0) {
        // flush the current run len and char to the stdout
        fwrite(&runlen, 4, 1, stdout);
        fwrite(&c, 1, 1, stdout);
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    char *filePath, *line;
    FILE *f;
    size_t linecap;
    ssize_t linelen;
    int res, runlen;
    char current, next;

    for (int i = 1; i < argc; ++i) {
        filePath = argv[i];
        f = fopen(filePath, "r");
        if (f == NULL) {
            perror("wzip: unable to open file\n");
            exit(1);
        }

        line = NULL;
        linecap = 0;

        while ((linelen = getline(&line, &linecap, f)) > 0) {
            for (size_t i = 0; i < linelen; ++i) {
                next = line[i];
                if (next == current) {
                    ++runlen;
                } else {
                    flush(current, runlen);

                    runlen = 1;
                    current = next;
                }
            }
        }

        res = fclose(f);
        if (res != 0) {
            perror("wzip: unable to close file\n");
            exit(1);
        }
    }

    // flush here in case we ended up with a run that went to the EOF and/or across multiple files
    flush(current, runlen);

    return 0;
}
