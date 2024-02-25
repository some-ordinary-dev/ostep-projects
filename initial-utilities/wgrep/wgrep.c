#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line_matches(char *line, size_t linelen, char *searchTerm) {
    size_t searchLen = strlen(searchTerm);
    size_t searchIdx = searchLen;

    // Perform a backwards search to remove any false negative matches
    // e.g., searchTerm = "ofoft", line = "ofofoft"
    // if we were doing a forward search we would match the first 4 characters
    // and fail on the 5th, and either have to backtrack to the second matching character
    // or incorrectly mark this line as not a match.
    // Doing a backwards search allows us to remove any need for backtracking.
    for (size_t i = linelen; i > 0 && searchIdx > 0; --i) {
        if (line[i - 1] == searchTerm[searchIdx - 1]) {
            --searchIdx;
        } else {
            // reset if we don't have a matching char
            searchIdx = searchLen;
        }
    }

    return searchIdx == 0;
}

void grep_file(char *searchTerm, FILE *f) {
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    int matches;

    while ((linelen = getline(&line, &linecap, f)) > 0) {
        matches = line_matches(line, linelen, searchTerm);
        if (matches) {
            printf("%s", line);
        }
    }
}

int main(int argc, char **argv) {
    char *filePath;
    int res;
    FILE *f;
    char *searchTerm;

    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

    searchTerm = argv[1];

    if (argc == 2) {
        grep_file(searchTerm, stdin);
    } else {
        for (int i = 2; i < argc; ++i) {
            filePath = argv[i];
            f = fopen(filePath, "r");

            if (f == NULL) {
                printf("wgrep: cannot open file\n");
                exit(1);
            }

            grep_file(searchTerm, f);

            res = fclose(f);
            if (res != 0) {
                perror("wgrep: unable to close file\n");
                exit(1);
            }
        }
    }

    return 0;
}


