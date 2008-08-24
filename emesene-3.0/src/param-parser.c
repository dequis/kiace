
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>

#include "param-parser.h"

int count_words(char *);

/* i'm proud of this pointer mess [because it's my first pointer mess] */
/* @wariano: strtok sucks, because the man page says so */

char **dx_parse_command(char *input) {
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

int dx_match_command(char *cmd, int expect, ...) {
    va_list ap;
    char **parsed;
    char *cmp;
    int i;

    va_start(ap, expect);

    parsed = dx_parse_command(cmd);

    for (i=0; i < expect && parsed[i] != NULL; i++) {
        cmp = va_arg(ap, char *); /*vik_kernes*/
        if (cmp == NULL) {
            continue;
        }
        if (strncmp(parsed[i], cmp, strlen(cmp))) {
            break;
        }
    }
    dx_free_parsed_command(parsed);
    va_end(ap);

    return (i == expect); /*if everything goes well this should be true*/
}

char *dx_get_param(char *cmd, int number) {
    char **parsed;
    char *retval;
    int i;

    parsed = dx_parse_command(cmd);

    /* walk the array safely */
    for (i = 0; i < number; i++) {
        if (parsed[i] == NULL) {
            break; /*no more array, stop here*/
        }
    }
    retval = parsed[i]; /*NULL on error*/

    dx_free_parsed_command(parsed);

    return retval;
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
