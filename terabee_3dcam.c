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
	printf ("%s [options] <spi-device>\n", cmd);
	printf ("Options:\n");
	printf ("  -a <freq>: set anti-alias filter cutoff frequency (Hz).\n");
	printf ("        Allowed values: 800, 400, 200, 50\n");
}

static void version () 
{
}

int main (int argc, char **argv) {

	uint32_t v;

	int debug_level;


	int fd;

	// Parse command line arguments. See usage() for details.
	int c;
	while ((c = getopt(argc, argv, "b:d:f:g:hio:qs:tv")) != -1) {
		switch(c) {


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





	fprintf (stdout, "P5 80 60 255\n");

	int i = 0;

	while (!feof(stdin)) {

		fread (&v, sizeof v, 1, stdin);

		//v = __builtin_bswap32 (v);
		v &= 0x0fffffff;
		//fprintf (stdout, "%02x ", v);

		v &= 0xff;

		fputc (v,stdout);

		i++;

		if (i%80 == 0) {
			//fprintf(stdout,"\n");
		}

	}
}
