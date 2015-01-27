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

//
// Function used to parse the build.prop to check if you can unlock or not
char *android_product_name_get(char product_name[256])
{
	FILE *fp = NULL;
	char line[256];
	char *pname;


	fp = fopen("/system/build.prop", "r");
	if (fp == NULL) {
		fprintf(stderr,
			"omapconf: %s(): could not open '/system/build.prop'?!\n", __func__);
		return NULL;
	}

	/* Retrieve pastry */
	while (fgets(line, 256, fp) != NULL) {
		/* Remove endind '\n' */
		line[strlen(line) - 1] = '\0';
		dprintf("%s(): line=%s len=%u\n", __func__, line, strlen(line));
		/* Looking for the "ro.build.version.release" property line */
		if (strstr(line, "ro.build.version.incremental=") == NULL)
			continue;
		fclose(fp);
		dprintf("%s(): ro.product.name line found.\n", __func__);
		pname = strchr(line, '=');
		pname += sizeof(char);
		if (pname == NULL) {
			dprintf("%s(): '=' not found?!\n", __func__);
			return NULL;
		}
		strncpy(product_name, pname, 256);
		dprintf("%s(): product_name='%s'\n", __func__, product_name);
		return product_name;
	}

	fclose(fp);
	dprintf("%s(): eof reached!\n", __func__);
	return NULL;
}


/* Load the aboot partition from emmc and return it */
int check_for_aboot_partition() {
        int file = open(FIRE_TV_ABOOT,O_RDWR);

        if (file == -1) {
                return 0;
        } else {
                return 1;
        }
}

/* Check the status of the bootloader and return it */
int check_bootloader_status() {
        int file = open(FIRE_TV_ABOOT,O_RDWR);
        char buffer[17];

        lseek(file,-512,SEEK_END);
        read(file,buffer,17);

        if (buffer[16] == 0) {
                return 0;
        } else if (buffer[16] == 1) {
                return 1;
        } else {
                return -1;
        }
}

int main(int argc, char *argv[]) {
        // Present the flags to the user
        if (argv[1] == NULL) {
                fprintf(stderr, "Amazon FireTV Bootloader Unlock OpenSource Tool\n");
                fprintf(stderr, "By: rhcp011235\n");
                fprintf(stderr, "Usage: %s unlock|lock|check\n",argv[0]);
                exit(1);
        }

        if (check_for_aboot_partition() == 0) {
                printf("No aboot partition is detected!\n");
                exit(1);
        }

        // Check Bootloader Status
        if (strcmp(argv[1],"check") == 0) {
                if (check_bootloader_status() == 0)
                        printf("You have a locked bootloader\n");
                else if (check_bootloader_status() == 1)
                        printf("You have an unlocked bootloader\n");
                else
                        printf("Unknown error occured\n");
                exit(1);
        }

        if (strcmp(argv[1],"unlock") == 0 || strcmp(argv[1],"lock") == 0) {
                int file = open(FIRE_TV_ABOOT,O_RDWR);
                char buffer[17];

                lseek(file,-512,SEEK_END);

                // Unlock the bootloader
                if (strcmp(argv[1],"unlock") == 0) {
                        write(file,&UNLOCK_STRING,sizeof(UNLOCK_STRING));
                }

                // Lock the bootloader
                if (strcmp(argv[1],"lock") == 0) {
                        write(file,&LOCK_STRING,sizeof(LOCK_STRING));
                }

                // Present the status of the bootloader
                if (check_bootloader_status() == 0) {
                        printf("You have a locked bootloader\n");
                } else if (check_bootloader_status() == 1) {
                        printf("You have a partially unlocked bootloader now\n");
                } else {
                        printf("Unknown error occured\n");
                }

        } else {
                printf("An invalid argument was provided\n");
        }

        // Close the program
        exit(1);
}
