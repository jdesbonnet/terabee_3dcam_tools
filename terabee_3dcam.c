/**
 *
 * Convert terabee 3dcam raw frames into a depth map.
 * Joe Desbonnet 23 May 2019.
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>


#define PROGRAM "Terabee 3Dcam utilily"
#define VERSION "0.1.1"
#define WIDTH 80
#define HEIGHT 60

#define CHANNEL_DEPTH 0
#define CHANNEL_INTENSITY 1

#define FORMAT_RAW 1
#define FORMAT_PGM 2

/**
 * Help on how to use this utility.
 */ 
static void usage (char *cmd) 
{
	printf ("%s [options] \n", cmd);
	printf ("Options:\n");
	printf ("  -c <channel>: choose channel: 0=depth (default), 1=intensity\n");
	printf ("  -f <format> : output format: pgm | raw | pxval\n");
	printf ("  -m : output min, mean, max to stderr\n");
	printf ("  -o <output-format>: eg frame-%%05d.pgm. Defaults to stdout\n");
	printf ("  -t <time-format>: eg frame-%%d.pgm\n");
}

static void version () 
{
	printf ("%s version %s\n", PROGRAM, VERSION);
}

int main (int argc, char **argv) {

	uint32_t v;
	uint64_t sum_v = 0;


	int minmax_flag = 0;
	int debug_level = 0;


	// 0=depth, 1=intensity
	int channel = 0;

	char *output_format = NULL;
	char *time_output_format = NULL;

	int max = 0;
	int min = 1<<31;

	int format = 0;
	int frame_in_byte_count = 0;
	int frame_out_byte_count = 0;
	int frame_count = 0;

	// Buffer used to constuct frame file name (output-format length + space for frame number)
	char filename[512+64];

	struct timespec ts;

	// Parse command line arguments. See usage() for details.
	int c;
	while ((c = getopt(argc, argv, "b:c:d:f:g:himo:qs:t:v")) != -1) {
		switch(c) {

			case 'c':
				channel = atoi (optarg);
				break;

			case 'd':
				debug_level = atoi (optarg);
				break;

			case 'f':
				if (strcmp("pgm",optarg)==0) {
					format = FORMAT_PGM;
				} else if (strcmp("raw",optarg)==0) {
					format = FORMAT_RAW;
				}
				break;

			case 'h':
				version();
				usage(argv[0]);
				exit(EXIT_SUCCESS);
			case 'm':
				minmax_flag = 1;
				break;

			case 'o':
				output_format = optarg;
				// Protect against buffer overflow 
				if (strlen(output_format)>512) {
					fprintf(stderr,"error: output format too long, max 512 characters\n");
					exit(EXIT_FAILURE);
				}
				break;

			case 't':
				time_output_format = optarg;
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


	// Send output to here
	FILE *out = NULL;

	// Write output to stdout if no file format specified
	if (output_format == NULL) {
		out = stdout;
	}


	while (!feof(stdin)) {

		if (frame_in_byte_count == 0) {

			// System clock timestamp of start of frame
			clock_gettime(CLOCK_REALTIME, &ts);

			if (debug_level > 1) {
				fprintf (stderr,"starting frame %d\n", frame_count);
			}

			// If writing frames to files, close at end of each frame
			if (out != NULL && output_format != NULL) {
				fclose(out);
				out = NULL;
			}

			if (output_format != NULL) {
				// TODO: buffer overflow check on filename required
				sprintf (filename, output_format, frame_count);
				out = fopen (filename, "w");
				frame_count++;
			}
			if (time_output_format != NULL) {
				// timestamp in ms
				long timestamp = ts.tv_sec*1000 + (ts.tv_nsec/1000000);
				sprintf (filename, time_output_format, timestamp);
				out = fopen (filename, "w");
			}

			if (format == FORMAT_PGM) {
        			if (channel == CHANNEL_DEPTH) {
                			fprintf (out, "P5 80 60 4095\n");
        			} else {
                			fprintf (out, "P5 80 60 8191\n");
        			}
			}
		}

		fread (&v, sizeof v, 1, stdin);
		frame_in_byte_count += 4;


		// first 16 bits seem to be depth
		// next 16 bits intensity
		// but not clear

		// Print raw value of center pixel
		if (frame_in_byte_count == (80*30+40)*4) {
			fprintf (stderr,"v=%08x %04x %04x\n", v, v>>16, v&0xffff);
		}

		if (channel == CHANNEL_DEPTH) {
			v = v>>16;
			v &= 0xfff;

			// Not sure how to make sense of this?!
			//v >>= 4;
			v &= 0xff;      // 8 bit greyscale
		} else if (channel == CHANNEL_INTENSITY) {
			//v = (v & 0x0fff) | ( ((v&(1<<31))==0) ? 0:(1<<12));
			v &= 0xffff;
			v >>= 4;
		}


		v &= 0xffff;

		sum_v += v;

		if (v>max) {
			max = v;
		}
		if (v<min) {
			min = v;
		}

		fputc (v>>8,out);
		fputc (v&0xff,out);
		frame_out_byte_count += 2;

		// Test for end of frame
		if (frame_in_byte_count == WIDTH * HEIGHT * 4) {
			if (debug_level > 1) {
				fprintf (stderr,"frame_in_byte_count=%d frame_out_byte_count=%d\n",
					frame_in_byte_count, frame_out_byte_count);
			}	
			frame_in_byte_count = 0;
			frame_out_byte_count = 0;
			frame_count++;

			if (minmax_flag) {
				fprintf (stderr, "min=%6d max=%6d mean=%6d\n", min, max, (int)(sum_v/(WIDTH*HEIGHT)));
				sum_v = 0;
				max = 0;
				min = 1<<31;
			}

		}

	}

	if (minmax_flag) {
		fprintf (stderr, "min=%d max=%d mean=%d\n", min, max, (int)(sum_v/(WIDTH*HEIGHT)));
	}
}
