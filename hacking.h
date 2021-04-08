#ifndef HACKING_H
#define HACKING_H

// A function to display an error message and then exit
void fatal(char *message) {
	char error_message[100] = {0};

	strcpy(error_message, "[!!] Fatal Error ");
	strncat(error_message, message, 83);
	perror(error_message);
	exit(-1);
} // end fatal

// An error-checked malloc() wrapper function
void *ec_malloc(unsigned int size) {
	void *ptr = NULL;

	ptr = malloc(size);
	if (ptr == NULL) {
		fatal("in ec_malloc() on memory allocation");
	} // end if

	return ptr;
} // end ec_malloc

// Dumps raw memory in hex byte and printable split format
void dump(const unsigned char *data_buffer, const unsigned int length) {
	unsigned char byte;
	unsigned int i = 0;
	unsigned int j = 0;

	for (i = 0; i < length; ++i) {
		byte = data_buffer[i];
		printf("%02x ", data_buffer[i]);	// Display byte in hex

		if (((i % 16) == 15) || (i == length - 1)) {
			for (j = 0; j < 15 - (i % 16); ++j) {
				printf("   ");
			} // end for

			printf("%s ", "|");

			for (j = (i - (i % 16)); j <= i; ++j) {	// Display printable bytes from line
				byte = data_buffer[j];

				if ((byte > 31) && (byte < 127)) {	// Outside printable char range
					printf("%c", byte);
				} else {
					printf("%s", ".");
				} // end if
			} //end for

			puts("");	// End of the dump line (each line is 16 bytes)
		} // end if
	} // end for
} // end dump

#endif // end HACKING_H