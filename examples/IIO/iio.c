#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

struct data {
	uint32_t activity;
	uint64_t timestamp;
};

int main(int argc, char *argv[])
{
	int fd;
	struct data d;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s DEVICE\n", argv[0]);
		return EXIT_FAILURE;
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		perror(argv[1]);
		return EXIT_FAILURE;
	}

	while (read(fd, &d, sizeof(d)) == sizeof(d))
		printf("%llu: %u\n", d.timestamp, (d.activity >> 1) & 3);

	close(fd);

	return EXIT_SUCCESS;
}
