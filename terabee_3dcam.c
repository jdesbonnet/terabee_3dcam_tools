/**
 *
 * Convert terabee 3dcam raw frames into a depth map.
 * Joe Desbonnet 23 May 2019.
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


#define WIDTH 80
#define HEIGHT 60


/**
 * Help on how to use this utility.
 */ 
static void usage (char *cmd) 
{
	printf ("%s [options] \n", cmd);
	printf ("Options:\n");
	printf ("  -c <channel>: choose channel: 0=depth (default), 1=intensity\n");
	printf ("  -o <output-format>: eg frame-%%05d.pgm. Defaults to stdout\n");
}

static void version () 
{
}

int main (int argc, char **argv) {

	uint32_t v;

	int debug_level;


	// 0=depth, 1=intensity
	int channel = 0;

	char *output_format = NULL;

	int max = 0;
	int frame_byte_count = 0;
	int frame_count = 0;
	char filename[2048];


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

			case 'o':
				output_format = optarg;
				break;

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

/*
	if (channel == 0) {
		fprintf (stdout, "P5 80 60 4095\n");
	} else {
		fprintf (stdout, "P5 80 60 65535\n");
	}
*/

	FILE *out = stdout;

	while (!feof(stdin)) {

		if (frame_byte_count == 0) {

			if (out != NULL) {
				fclose(out);
				out = NULL;
			}

			if (output_format != NULL) {
				sprintf (filename, output_format, frame_count);
				out = fopen(filename, "w");
				frame_count++;
			}			

        		if (channel == 0) {
                		fprintf (out, "P5 80 60 4095\n");
        		} else {
                		fprintf (out, "P5 80 60 65535\n");
        		}
		}

		fread (&v, sizeof v, 1, stdin);
		frame_byte_count += 4;

		if (channel == 0) {
			v >>= 16;
		} else {
			v &= 0xffff;
		}

		if (v>max) {
			max = v;
		}

		fputc (v>>8,out);
		fputc (v&0xff,out);

		if (frame_byte_count == WIDTH * HEIGHT * 4) {
			frame_byte_count = 0;
		}

	}

	fprintf (stderr, "max=%d\n", max);
}
