#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
 
const char* journal_filename = "journal.log";
 
void write_journal_entry (void *filename, char* entry)
{
        int fd = open (filename, O_WRONLY | O_CREAT | O_APPEND, 0660);
        write (fd, entry, strlen (entry));
        write (fd, "\n", 1);
        fsync (fd);
        close (fd);
}

void * read_thread(void *filename)
{
	char buf[4096];
	FILE * fp = fopen(filename, "r");

	while(fread(buf, 1, 1, fp) != 0) {
	    // do stuff
	}
	printf("Finished reading 1!\n");
	fclose(fp);
/*	
	fp = fopen(filename, "r");

	while(fread(buf, 4096, 1, fp) != 0) {
	    // do stuff
	}
	printf("Finished reading 2!\n");
	fclose(fp);
	
	fp = fopen(filename, "r");

	while(fread(buf, 4096, 1, fp) != 0) {
	    // do stuff
	}
	printf("Finished reading 3!\n");
	fclose(fp);
*/
	return filename;
}

void * start_thread_fsync(void *filename)
{
	printf("%s\n", (const char *) filename);
	int f = open(filename, O_RDWR | O_CREAT);
	char *buf = NULL;
	int i;
	buf = malloc(1024 * 4);
	memset (buf, 0xcafe, 1024 *4);

        //lseek (f, 0, SEEK_SET);
        //int records = 10*1000;
	//while(1) {
	        for(i = 0; i < 2000; i++) {
        	        write(f, buf, 1024 * 4);
                	fsync(f);
        	}
	//}
	close (f);
	return filename;
}

void * start_thread_fdatasync(void *filename)
{
	printf("%s\n", (const char *) filename);
	
	int f = open(filename, O_RDWR | O_CREAT | O_APPEND);
	char *buf = NULL;
	int i;
	buf = malloc(1024 * 4);
	memset (buf, 0xcafe, 1024 *4);
        //lseek (f, 0, SEEK_SET);
        //int records = 10*1000;
        for(i = 0; i < 2; i++) {
                write(f, buf, 1024 *4);
                fdatasync(f);
        }
	
	close (f);
	return filename;
}

int main(void)
{
	pthread_t thing1, thing2, thing3, thing4, thing5, thing6, thing7;

	const char *filename1 = "./Thing1.txt";
	//const char *filename2 = "./Thing2.txt";
	//const char *filename3 = "./Thing3.txt";
	//const char *filename4 = "./Thing4.txt";
	//const char *filename5 = "./Thing5.txt";
	const char *read_filename1 = "./sample1.txt";
	const char *read_filename2 = "./sample2.txt";
	const char *read_filename3 = "./sample3.txt";
	const char *read_filename4 = "./sample4.txt";

	/*Create two threads and each with different messages*/
	pthread_create (&thing1, NULL, read_thread, (void *)read_filename1);
	pthread_create (&thing2, NULL, read_thread, (void *)read_filename2);
	pthread_create (&thing3, NULL, read_thread, (void *)read_filename3);
	pthread_create (&thing4, NULL, read_thread, (void *)read_filename4);
	pthread_create (&thing5, NULL, start_thread_fsync, (void *)filename1);

	//pthread_create (&thing2, NULL, start_thread_fsync, (void *)filename2);
	//pthread_create (&thing3, NULL, start_thread_fsync, (void *)filename3);
	//pthread_create (&thing4, NULL, start_thread_fsync, (void *)filename4);
	//pthread_create (&thing5, NULL, start_thread_fsync, (void *)filename5);

	/*
	* wait for the threads to exit.
	*/
	pthread_join(thing1, NULL);
	pthread_join(thing2, NULL);
	pthread_join(thing3, NULL);
	pthread_join(thing4, NULL);
	pthread_join(thing5, NULL);
//	pthread_join(thing6, NULL);

//	pthread_join(thing7, NULL);

	
	//sync();
	return 0;
}
