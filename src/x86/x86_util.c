#include <stdlib.h>
#include <time.h>       /* time */

#include "common.h"

int iot_init() {
	srand (time(NULL));

	return 0;
}

// Assumes 0 <= max <= RAND_MAX
// Returns in the half-open interval [0, max]
int iot_random_in_range(int min, int max) {
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    unsigned long num_bins = (unsigned long) (max-min) + 1;
    unsigned long num_rand = (unsigned long) RAND_MAX + 1;
    unsigned long bin_size = num_rand / num_bins;
    unsigned long defect   = num_rand % num_bins;

  	long x;
  	do {
   		x = rand();
  	}
  	// This is carefully written not to overflow
  	while (num_rand - defect <= (unsigned long)x);

  	// Truncated division is intentional
  	return (x/bin_size) + min;
}

void iot_error(char* msg) {
#ifdef WIN32
	wchar_t *s = NULL;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
               NULL, WSAGetLastError(),
               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
               (LPWSTR)&s, 0, NULL);
	fprintf(stderr, "%S : %d\n", s, WSAGetLastError());
	LocalFree(s);

	perror(msg);
#else
	perror(msg);
#endif
}

void iot_error_exit(char* msg) {
	perror(msg);
    exit(0);
}