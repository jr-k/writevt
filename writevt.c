/*
 * Mostly ripped off of console-tools' writevt.c
 */

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

char *progname;

static int usage() {
	printf("Usage: %s ttydev text\n", progname);
	return 2;
}

int main(int argc, char **argv) {
	int fd, argi;
	char *term = NULL;
	char *text = NULL;

	progname = argv[0];

	argi = 1;

	if (argi < argc)
		term = argv[argi++];
	else {
		fprintf(stderr, "%s: no tty specified\n", progname);
		return usage();
	}

	if (argi < argc)
		text = argv[argi++];
	else {
		fprintf(stderr, "%s: no text specified\n", progname);
		return usage();
	}

	if (argi != argc) {
		fprintf(stderr, "%s: too many arguments\n", progname);
		return usage();
	}

	fd = open(term, O_RDONLY);
	if (fd < 0) {
		perror(term);
		fprintf(stderr, "%s: could not open tty\n", progname);
		return 1;
	}

	if (text[0] == '0' && text[1] == 'x') {
		char hex[2];
		hex[0] = (int) strtol(text, NULL, 16);
		hex[1] = '\0';
		ioctl(fd, TIOCSTI, hex);
	} else {
		while (*text) {
			if (ioctl(fd, TIOCSTI, text)) {
				perror("ioctl");
				return 1;
			}
			text++;
		}
	}

	return 0;
}
