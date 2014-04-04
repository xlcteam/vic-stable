#ifndef _VIC_VAR
#define _VIC_VAR

#include "vic.h"

#include <stdlib.h>
#include <stdint.h>

#define VIC_VAR_NONE 255

struct vic_vars{
	char* name;
	char* val;
        void* pval;
} *vic_vars;

static uint8_t vic_varcount= 0;

void vic_var_set_new_bind(char* name, char* val, void* pval)
{
	if (vic_varcount == 0){
		vic_vars = (struct vic_vars*)
			malloc(
				++vic_varcount * sizeof(struct vic_vars)
			);
	} else {
		vic_vars = (struct vic_vars*)
			realloc(
				vic_vars,
				++vic_varcount * sizeof(struct vic_vars)
			);
	}

//	printf("task: %s\n", name);

	vic_vars[vic_varcount - 1].name = strdup(name);
	vic_vars[vic_varcount - 1].val = strdup(val);
	vic_vars[vic_varcount - 1].pval = pval;
}

uint8_t vic_var_get_id (char* name)
{
	int i;
	for (i = 0; i < vic_varcount; i++) {
		if (strcmp(name, vic_vars[i].name) == 0) {
			return i;
		}
	}
	return VIC_VAR_NONE;
}


void vic_var_set_bind(char* name, char* val, void *pval)
{

	uint8_t id;
        id = vic_var_get_id(name);

	if (id == VIC_VAR_NONE){
		vic_var_set_new_bind(name, val, pval);
	} else {
		free(vic_vars[id].val);
		vic_vars[id].val = strdup(val);
                if (pval != NULL) {
                        vic_vars[id].pval = pval;
                }

                int len, i;
                len = strlen(val);
                i = 0;

                if (val[0] == '-')
                        i = 1;

                for (; i < len; i++) {
                        if (!isdigit(val[i]))
                                break;
                }

                if (i == len) {
                        int* p;
                        p = (int *) vic_vars[id].pval;
                        if (p != NULL)
                                *p = atoi(val);

                } else {
                        char* p;
                        p = (char *) vic_vars[id].pval;
                        if (p != NULL) {
                                free(p);
                                p = strdup(val);
                        }
                }
	}

}

char* vic_var_get(char* name)
{
	int i;
	for (i = 0; i < vic_varcount; i++) {
		if (strcmp(name, vic_vars[i].name) == 0) {
			return vic_vars[i].val;
		}
	}

	return 0;

}

char* vic_var_replace(char* str)
{
        char* buffer = (char *) malloc(sizeof(char));
        uint8_t buffer_len = 0;

        char* var = (char *) malloc(sizeof(char));
        uint8_t var_len = 0;

        uint8_t in_var = 0;
        do {

                /* all variables start with $ */
                if (*str == '$' && in_var == 0) {
                        in_var = 1;
                /* variable names can contain alphanumeric chars and underscore */
                } else if (isalnum(*str) || *str == '_') {
        	        var = (char *) realloc(buffer,
						(var_len + 1) * sizeof(char));
			var[var_len++] = *str;

                } else {
			buffer = (char *) realloc(buffer,
						(buffer_len + 1) * sizeof(char));
			buffer[buffer_len++] = *str;
                }



        } while(*(str++) != '\0');

}


#endif

