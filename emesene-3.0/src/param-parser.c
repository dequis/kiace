
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_words(char *);

/* i'm proud of this pointer mess [because it's my first pointer mess] */

char** dx_parse_command(char *input) {
    char **array_first;
    char **array;
    char *buffer;
    char *ptr;

    array_first = array = malloc(sizeof(char *) * (count_words(input) + 1));
    buffer = malloc(strlen(input));

    ptr = input;
    *array = buffer;

    while (*ptr != '\0') {
        if (*ptr == ' ') {
            array++;
            *array = buffer + 1;
            *buffer = '\0';
        } else {
            *buffer = *ptr;
        }
        buffer++; ptr++;
    }

    *buffer = '\0'; /*last char of the buffer*/
    *(array + 1) = NULL; /*last item of the array*/

    return array_first;
}

int dx_free_parsed_command(char **cmd) {
    free(*cmd);
    free(cmd);
    return 0;
}

int count_words(char *input) {
    int retval = 0;
    char *ptr;

    ptr = input;
    while (*ptr != '\0') {
        if (*ptr == ' ') retval++;
        ptr++;
    }
    return retval + 1; /*include last word*/
}

#if 0
int main(int argc, char **argv) {
    char **asd;
    int i = 0;
    if (argc == 2) {
        asd = dx_parse_command(argv[1]);
        while (asd[i] != NULL) {
            printf("%s, ", asd[i]);
            i++;
        }
        printf("\n");
        dx_free_parsed_command(asd);
    }
    return 0;
}
#endif
