#ifndef _VIC_FUNC
#define _VIC_FUNC

#include "vic.h"

#include <stdlib.h>
#include <stdint.h>

#define VIC_FUNC_BCKG 0x01

struct vic_fntable{
	char* name;
	void(*fn)();
	uint8_t mask;
} *vic_fntable;

static int vic_fncount = 0;

struct vic_aliases{
	char *name;
	char *alias;
} *vic_aliases;
static int vic_alias_count = 0;

void vic_fn_add_mask(char* name, void(*fn)(), uint8_t mask)
{
        if(vic_fncount == 0) {
                vic_fntable = (struct vic_fntable*)
                        malloc(
                                ++vic_fncount * sizeof(struct vic_fntable)
                        );
        } else {
                vic_fntable = (struct vic_fntable*)
                        realloc(
                                vic_fntable,
                                ++vic_fncount * sizeof(struct vic_fntable)
                        );
        }

  	vic_fntable[vic_fncount - 1].name = name;
  	vic_fntable[vic_fncount - 1].fn = *fn;
        vic_fntable[vic_fncount - 1].mask = mask;

}

void vic_fn_rm(char* name)
{
	int i;
	for (i = 0; i < vic_fncount ; i++) {
		if(strcmp(name, vic_fntable[i].name) == 0){
			free(vic_fntable[i].name);

			vic_fntable[i].name = NULL;
			vic_fntable[i].fn = NULL;

			break;
		}
	}
}

int vic_fn_call(const char* name)
{
        int i;

        for (i = 0; i < vic_fncount ; i++) {
		if(vic_fntable[i].fn == NULL)
			continue;

		if (!strcmp(vic_fntable[i].name , name)) {
                      (*(vic_fntable[i].fn))();
                      return 1;
                }
        }
        return 0;
}

void vic_alias_add(char *name, char *alias)
{
        if(vic_alias_count == 0) {
                vic_aliases = (struct vic_aliases*)
                        malloc(
                                ++vic_alias_count * sizeof(struct vic_aliases)
                        );
        } else {
                vic_aliases = (struct vic_aliases*)
                        realloc(
                                vic_aliases,
                                ++vic_alias_count * sizeof(struct vic_aliases)
                        );
        }
	vic_aliases[vic_alias_count -1].name = name;
	vic_aliases[vic_alias_count -1].alias = alias;

}

void vic_alias_rm(char* name)
{
	int i;
	for (i = 0; i < vic_alias_count; i++) {
		if(strcmp(name, vic_aliases[i].name) == 0){
			free(vic_aliases[i].name);
			free(vic_aliases[i].alias);

			vic_aliases[i].name = NULL;
			vic_aliases[i].alias = NULL;

			break;
		}
	}
}

char *vic_alias(char *name)
{
	int i;
	for (i = 0; i < vic_alias_count; i++){
#ifdef DEBUG
		vic_print(vic_aliases[i].name);
		vic_print(" -> ");
		vic_println(vic_aliases[i].alias);
#endif
        if (vic_aliases[i].name == NULL)
            continue;

		if(strcmp(vic_aliases[i].name, name) == 0){
			return vic_aliases[i].alias;
		}
	}

	return 0;

}

void vic_func_ls_alias(void)
{
	int i;

	for (i = 0; i < vic_alias_count; i++) {
        if(vic_aliases[i].name == NULL)
            continue;

		vic_print(vic_aliases[i].name);
		vic_print("\t => \t");
		vic_println(vic_aliases[i].alias);
	}
}

void vic_func_ls(void)
{
	int i;

	for (i = 0; i < vic_fncount ; i++) {
		if(vic_fntable[i].fn == NULL)
			continue;

		vic_print(vic_fntable[i].name);
		vic_print("\t -> \t");
		vic_print_hex((int) *(vic_fntable[i].fn));
		vic_println("");
	}
}

void vic_func_echo()
{
	vic_func();
	vic_println(vic_buff);
}

void vic_func_help(void)
{


}


char** vic__args(const char* in, int *argc)
{
	char *tmp;
	tmp = (char *) malloc(sizeof(char));
	int len = 0;

	char **argv;

	uint8_t in_str = 0, in_sstr = 0;

	argv = NULL;
	argv = (char **) malloc(sizeof( *argv ));
	*argc = 0;

	do {
		if ((*in == ' ' || *in == '\0') && (!in_str && !in_sstr)) {
			tmp = (char *) realloc(tmp, (len + 1) * sizeof(char));
			tmp[len++] = '\0';


			argv = (char **) realloc(argv, (*argc + 1) * sizeof( char *));

			argv[*argc] = (char *) malloc((len+1) * sizeof(char));
			argv[*argc] = strdup(tmp);

			free(tmp);
			tmp = (char *) malloc(sizeof(char));
			len = 0;

			*argc += 1;

		} else if (*in == '"' && in_sstr == 0 && *(in - 1) != '\\' ) {
			in_str = !in_str;
		} else if (*in == '\'' && in_str == 0 && *(in - 1) != '\\' )  {
			in_sstr = !in_sstr;
		} else {
			/* escaping \" and \' */
			if ((*in == '"' || *in == '\'') && *(in - 1) == '\\') {
				tmp[len - 1] = *in;
			} else {
				tmp = (char *) realloc(tmp, (len + 1) * sizeof(char));
				tmp[len++] = *in;
			}
		}


	} while (*in++ != '\0');

	return argv;

}

#endif

