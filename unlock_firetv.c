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

// Taken from online someplace
// Easy HexDump Function 
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

int main(int argc, char *argv[])
{	
	int file=0;
	int lock,unlock,check;

	// Magic	
	char lock_string[] = "ANDROID-BOOT!\0\0\0\0\0\0\0";
	char unlock_string[] = "ANDROID-BOOT!\0\0\0\1\0\0\0";

  	if (argv[1] == NULL)
	{
		fprintf(stderr, "Amazon FireTV Bootloader Unlock OpenSource Tool\n");
		fprintf(stderr, "By: rhcp011235\n");
		fprintf(stderr, "Usage: %s unlock|lock|check\n",argv[0]);
		exit(1);
	}

	if (strcmp(argv[1],"lock") == 0)
    {
	   lock = 1;
	}
	if (strcmp(argv[1],"unlock") == 0)
	{	   
	   unlock = 1;
	}
    if (strcmp(argv[1],"check") == 0)
    {      
       check = 1;
    }

	// open aboot for Amazon Fire Tv
	if((file=open("aboot",O_RDWR)) < -1)
                return 1;
    
    char buffer[17];
    char buffer2[17];
 
	//
	// Seek from the bottom of the file
    lseek(file,-512,SEEK_END); 
	
	
    if (check == 1)
    {   
        read(file,buffer,17);
        if (buffer[16] == 0)
            printf("You Have a locked Bootloader\n");
        if (buffer[16] == 1)
            printf("You have an Unlocked Bootloader - Congrats\n");
        exit(1);
    }
	if (unlock == 1)
	{
	    // Unlock the bootloader
	    write(file,&unlock_string,sizeof(unlock_string));
	}
	if (lock == 1)
    {
        // Unlock the bootloader
        write(file,&lock_string,sizeof(unlock_string));
    }
	
    // Seek from the bottom of the file
    // Show our Unlock or Lock status to the user
    lseek(file,-512,SEEK_END);
	read(file,buffer,17);
    hexDump ("after Patch buffer", &buffer, sizeof (buffer));
	
    if(unlock == 1){
        printf("You have a partially unlocked bootloader now\n");
        exit(1);
    }
    
    if(lock == 1)
    {
        printf("You have a Re-Locked Your bootloader now\n");
        exit(1);
    }
    return 0;
}
