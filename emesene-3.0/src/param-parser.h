#ifndef _DX_PARAM_PARSER_H
#define _DX_PARAM_PARSER_H

char **dx_parse_command(char *);
int dx_free_parsed_command(char **);
int dx_match_command(char *, int, ...);
char *dx_get_param(char *, int);

#endif
