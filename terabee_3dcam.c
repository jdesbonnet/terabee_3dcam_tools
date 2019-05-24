#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/**
 *
 * Convert terabee 3dcam raw frames into a depth map.
 * Joe Desbonnet 23 May 2019.
 */


/**
 * Help on how to use this utility.
 */ 
static void usage (char *cmd) 
{
	printf ("%s [options] \n", cmd);
	printf ("Options:\n");
	printf ("  -c <channel>: choose channel: 0=depth (default), 1=intensity\n");
}

static void version () 
{
}

int main (int argc, char **argv) {

	uint32_t v;

	int debug_level;


	// 0=depth, 1=intensity
	int channel = 0;

	int max = 0;


	int fd;

	// Parse command line arguments. See usage() for details.
	int c;
	while ((c = getopt(argc, argv, "b:d:f:g:hil:o:qs:tv")) != -1) {
		switch(c) {

			case 'l':
				channel = atoi (optarg);
				break;

			case 'd':
				debug_level = atoi (optarg);
				break;

			case 'h':
				version();
				usage(argv[0]);
				exit(EXIT_SUCCESS);

			case 'v':
				version();
				exit(EXIT_SUCCESS);
			case '?':	// case when a command line switch argument is missing
				if (optopt == 'd') {
					fprintf (stderr,"ERROR: debug level 0 .. 9 must be specified with -d\n");
					exit(-1);
				}
				break;
		}
	}


	if (channel == 0) {
		fprintf (stdout, "P5 80 60 4095\n");
	} else {
		fprintf (stdout, "P5 80 60 65535\n");
	}

	while (!feof(stdin)) {

		fread (&v, sizeof v, 1, stdin);

		//v &= 0x0fffffff;

		if (channel == 0) {
			v >>= 16;
		} else {
			v &= 0xffff;
		}

		if (v>max) {
			max = v;
		}

		fputc (v>>8,stdout);
		fputc (v&0xff,stdout);

	}

	fprintf (stderr, "max=%d\n", max);
}
