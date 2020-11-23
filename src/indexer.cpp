#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include "../include/includes.h"

int main(int argc, char* argv[]) {

	char fifo[] = "/tmp/index_fifo";
	mkfifo(fifo,0666);

	int fd = open(fifo, O_RDONLY);
	char buffer[max_m];

	while (1) {

		read(fd, buffer, max_m);
		printf("%s",buffer);
	}

	close(fd);

}
