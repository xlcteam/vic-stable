#ifndef _VIC_SERIAL
#define _VIC_SERIAL

#include "vic.h"

#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>


static int vic_buffer_len = 0;

uint8_t vic_returned = 0;

char *vic_buffer;
char *vic_buff;
char *vic_func;

void vic_buffer_append(char i)
{
        vic_buffer = (char *)
                realloc(vic_buffer,
                        (vic_buffer_len + 1) * sizeof(char)
                );
        vic_buffer[vic_buffer_len++] = i;
}

inline void vic_buffer_free(void)
{
	if(vic_buffer != NULL){
		free(vic_buffer);
        	vic_buffer = NULL;
	}

        if(vic_func != NULL){
		free(vic_func);
		vic_func = NULL;
	}

        vic_buffer_len = 0;
}

/*
  Processes input stream.
*/


void vic_process(char input)
{
	if (vic_buffer == NULL)
		vic_buffer = (char *) malloc(sizeof(char));
#ifndef ARDUINO
	if (input == '\n'){
#else
	if (input == 13) {
#endif
		vic_buffer_append(';');
		vic_buffer_append('\0');
#ifdef SHELL
		vic_println(" ");
#endif
		vic_exec(vic_buffer);
		vic_buffer_free();
#ifdef SHELL
		vic_print(VIC_PS1);
#endif
	} else if (input == 3) { /* reseting Arduino */
		void (*rst)() = 0;
		rst();

	} else if ((input == 8) || (input == 0x7f)) { /* BACKPSACE and DELETE */
		if (vic_buffer_len > 0){
			vic_buffer_len--;

			vic_out((char) 8);
			vic_out(' ');
			vic_out((char) 8);
		}

	} else {
#ifdef SHELL
		vic_out(input);
#endif
		vic_buffer_append(input);
	}

}

void vic_exec(char *input)
{
	char *buffer;
	char *func = NULL;
	buffer = (char *) malloc(sizeof(char));
	int len = 0;
	do {
		if(*input == ';') {
			if (len == 0)
				continue;

                        // finish the string with '\0'
			buffer = (char *) realloc(buffer,
				(len + 1) * sizeof(char));
			buffer[len++] = '\0';


			/* in case of calling a procedure */
			if(func == NULL) {
				char *tmp;

				// gives pointer to aliased string
				if ((tmp = vic_alias(buffer)) != 0){
					vic_exec(tmp);
					// free(buffer);

				} else {
					vic_func = buffer;
					vic_fn_call(vic_func);
				}


			} else {
				vic_func = func;
				vic_buff = buffer;
		#ifdef DEBUG
				vic_print("buf: '");
				vic_print(vic_buff);
				vic_println("'");

				vic_print("func: ");
				vic_println(vic_func);
		#endif
				vic_fn_call(vic_func);

			}

			if (vic_returned == 0 && (vic_config & VIC_RPC)){
				vic_out('%');
				vic_print(vic_func);
				vic_out('$');

			}

			if (vic_returned == 1)
				vic_returned = 0;


			len = 0;
			free(func);
			func = vic_func = NULL;

			free(buffer);
			buffer = NULL;

		/* ignoring whitespace */
		} else if (isspace(*input) && len == 0) {
			continue;

		/* separate the function */
		} else if (*input == ' ' && func == NULL) {

			buffer = (char *) realloc(buffer,
						(len + 1) * sizeof(char));
			buffer[len++] = '\0';

			func = buffer;
			buffer = (char *) malloc(sizeof(char));
			len = 0;

		} else {

			buffer = (char *) realloc(buffer,
						(len + 1) * sizeof(char));
			buffer[len++] = *input;

		}

	} while(*(++input) != '\0');
}

// from bitlash/src/bitlast-serial.c
void vic_print_int_base(int i, uint8_t n)
{
	char buf[8 * sizeof(uint8_t)];           // stack for the digits
        char *ptr = buf;
        if (i == 0) {
                vic_out('0');
                return;
        }


        while (i > 0) {
                *ptr++ = i % n;
                i /= n;
        }
        while (--ptr >= buf) vic_out((*ptr < 10) ? (*ptr + '0') : (*ptr - 10 + 'A'));
}

void vic_print_int(int n)
{
	if (n < 0) {
		vic_out('-');
		n = -n;
	}

	vic_print_int_base(n, 10);
}

void vic_print_hex(int n)
{
	vic_print("0x");
	vic_print_int_base(n, 16);
}

#endif
