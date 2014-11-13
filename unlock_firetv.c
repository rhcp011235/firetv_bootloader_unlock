/*
 * Simple Amazon fire tv bootloader unlock/lock tool
 * thanks strace :)
 * Orignal code and ideas from jcase/rbox
 * by rhcp011235
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Constants
#define FIRE_TV_ABOOT "/dev/block/platform/msm_sdcc.1/by-name/aboot"
#define LOCK_STRING "ANDROID-BOOT!\0\0\0\0\0\0\0"
#define UNLOCK_STRING "ANDROID-BOOT!\0\0\0\1\0\0\0"

void hexDump (char *desc, void *addr, int len) {
	int i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	// Output description if given.
	if (desc != NULL)
		printf ("%s:\n", desc);

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).
		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
				printf ("  %s\n", buff);

			// Output the offset.
			printf ("  %04x ", i);
		}

		// Now the hex code for the specific character.
		printf (" %02x", pc[i]);

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buff[i % 16] = '.';
		else
			buff[i % 16] = pc[i];

		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		printf ("   ");
		i++;
	}

	// And print the final ASCII bit.
	printf ("  %s\n", buff);
}

/* Check the status of the bootloader and return it */
int check_bootloader_status(int file, int buffer) {
	read(file,buffer,17);
	if (buffer[16] == 0)
		return 0;
	else if (buffer[16] == 1)
		return 1;
	else
		return -1;
} 

/* Load the aboot partition from emmc and return it */
int get_aboot_partition() {
	return open(FIRE_TV_ABOOT,O_RDWR);
}

int main(int argc, char *argv[]) {	
	int file = 0;
	char buffer[17] = NULL;

	if (file =< -1) {
		printf("Error: ABOOT partition not found!");
		exit(1);
	}
	
	// Present the flags to the user
  	if (argv[1] == NULL) {
		fprintf(stderr, "Amazon FireTV Bootloader Unlock OpenSource Tool\n");
		fprintf(stderr, "By: rhcp011235\n");
		fprintf(stderr, "Usage: %s unlock|lock|check\n",argv[0]);
		exit(1);
	}

	// Check Bootloader Status
	if (strcmp(argv[1],"check") == 0) {   
		if (check_bootloader_status(file, buffer) == 0)
			printf("You Have a locked Bootloader\n");
		else
			printf("You have an Unlocked Bootloader - Congrats\n");
		exit(1);
	}

	// Load ABOOT from its partition
	file = get_aboot_partition();
	lseek(file,-512,SEEK_END);
	
	// Unlock the bootloader
	if (strcmp(argv[1],"unlock") == 0) {
		write(file,&unlock_string,sizeof(unlock_string));
	}
	
	// Lock the bootloader
	if (strcmp(argv[1],"lock") == 0) {
		write(file,&lock_string,sizeof(unlock_string));
	}
	
	// Load ABOOT from its partition
	file = get_aboot_partition();
	lseek(file,-512,SEEK_END);
	
	// Present the status of the bootloader
	if (check_bootloader_status(file, buffer) == 0) {
		printf("You have a locked bootloader\n");
	} else {
		printf("You have a partially unlocked bootloader now\n");
	}
	
	// Close the program
	exit(1);
}
