#define _GNU_SOURCE
#include <dlfcn.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define stop {free(line); printf("\n"); break;}
#define DELIM " "
#define NUM 6
#define LOOKUP "./lookup"

int call(
	intptr_t a1,
	intptr_t a2,
	intptr_t a3,
	intptr_t a4,
	intptr_t a5,
	intptr_t a6,
	void* func
);

int main() {
	for(;;) {
		printf("C:\\> ");

		char* line = NULL;
		size_t buf = 0;
		ssize_t len = getline(&line, &buf, stdin);
		if(len < 0) stop;
		line[len - 1] = 0;

		char* cmd  = strtok(line, DELIM);
		if(cmd == NULL) continue;
		void* func = dlsym(RTLD_DEFAULT, cmd);

		intptr_t args[NUM] = {};
		size_t argc = 0;
		for(; argc < NUM; ++argc) {
			char* rawArg = strtok(NULL, DELIM);
			if(rawArg == NULL) break;

			if(rawArg[0] >= '0' && rawArg[0] <= '9') {
				args[argc] = atol(rawArg);
			} else {
				args[argc] = (intptr_t) rawArg;
			}
		}

		if(func == NULL) {
			switch(cmd[0]) {
				case '/':
					char*  lib  = (char*) args[0];
					size_t len  = strlen(lib);
					char*  load = malloc(len + 6);

					memcpy(load, "lib", 3);
					memcpy(load + 3, lib, len);
					memcpy(load + 3 + len, ".so", 3);

					if(dlopen(load, RTLD_LAZY | RTLD_GLOBAL) == NULL) {
						printf("Error: could not load %s: %s\n", load, dlerror());
					}
					break;

				case '?':
					switch(fork()) {
						case -1:
							warn("lookup: fork failed");
							break;

						case 0:
							char* argv[NUM + 2] = {};
							argv[0] = LOOKUP;

							for(size_t i = 0; i < argc; ++i) {
								argv[i + 1] = (char*) args[i];
							}
							argv[argc + 1] = 0;

							execv(LOOKUP, argv);
							warn("lookup: execv failed");
							break;

						default:
							wait(NULL);
					}
					break;

				default:
					printf("Error: function %s not found!\n", cmd);
			}
		} else {
			printf("%d\n", call(
				args[0],
				args[1],
				args[2],
				args[3],
				args[4],
				args[5],
				func
			));
		}

		free(line);
	}
}
