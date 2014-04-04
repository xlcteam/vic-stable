#ifndef _VIC_TASKS
#define _VIC_TASKS

#include "vic.h"

#include <stdlib.h>
#include <stdint.h>


struct vic_tasks{
	char* name;
	unsigned long lastrun;
	unsigned int delay;
} *vic_tasks;

static int vic_taskcount = 0;

void vic_task_start(char* name, unsigned int delay)
{
	if (vic_taskcount == 0){
		vic_tasks = (struct vic_tasks*)
			malloc(
				++vic_taskcount * sizeof(struct vic_tasks)
			);
	} else {
		vic_tasks = (struct vic_tasks*)
			realloc(
				vic_tasks,
				++vic_taskcount * sizeof(struct vic_tasks)
			);
	}

//	printf("task: %s\n", name);

	vic_tasks[vic_taskcount - 1].name = name;
	vic_tasks[vic_taskcount - 1].delay = delay;
	vic_tasks[vic_taskcount - 1].lastrun = 0;

}

void vic_task_stop(uint8_t id)
{
	if (id >= vic_taskcount)
		return;

	if (vic_tasks[id].name == NULL)
		return;


	free(vic_tasks[id].name);

	vic_tasks[id].name = NULL;
	vic_tasks[id].delay = 0;
	vic_tasks[id].lastrun = 0;



}

void vic_tasks_run(void)
{
	int i;
	for (i = 0; i < vic_taskcount; i++) {
		if (vic_tasks[i].name == NULL)
			continue;

		if ((vic_tasks[i].delay + vic_tasks[i].lastrun) <= millis()){

//			printf("rtask: %s\n", vic_tasks[i].name);
			char *tmp;
			tmp = (char *) malloc(
				(strlen(vic_tasks[i].name) + 2) * sizeof(char));

			*tmp = '\0';
			strcat(tmp, vic_tasks[i].name);
			strcat(tmp, ";\0");

//			printf("running: %s\n", tmp);

			vic_exec(tmp);
			vic_tasks[i].lastrun = millis();

			free(tmp);
		}
	}

}


void vic_func_ps(void)
{
	int i;
	for (i = 0; i < vic_taskcount; i++) {
		if (vic_tasks[i].name == NULL)
			continue;

		vic_print_int(i);
		vic_print(": ");
		vic_println(vic_tasks[i].name);
	}

}


#endif

