#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Required for definitions for __REGISTER, __IN, __OUT directives
#include "grates.h"

#include "imfs.h"

// To register a syscall handler, either the function declaration or the
// function body must have the `__REGISTER(syscall_num)` attribute attached.
//
// This is a macro that expands to __attribute__(syscall:syscall_num)
// Which are used to generate a compliant wrapper through a python script
// that uses `libclang`
__REGISTER(0) 
ssize_t read_impl(
	int cageid, 
	int fd, 
	// Parameters can be marked with __OUT(size), __IN(size), or __INOUT(size)
	// These are useful for automatically generating code 
	// that calls `copy_data_between_cages`
	char *buf __OUT(count), 
	ssize_t count
);

__REGISTER(1)
ssize_t write_impl(
	int cageid,
	int fd,
	char *buf __IN(count),
	ssize_t count	
);

__REGISTER(2)
int open_impl(
	int cageid, 
	char *pathname __IN(256), 
	int flags,
	mode_t mode
);

__REGISTER(3)
int close_impl(
	int cageid, 
	int fd
);

// Grates must provide grate_init and grate_destroy functions that are called
// during the beginning and end of a grate's lifecycle.
void grate_init(void) { 
	imfs_init();
}

void grate_destroy(void) { 
	printf("[imfs_grate:grate_destroy] ed!\n");
}

ssize_t read_impl(int cageid, int fd, char *buf, ssize_t count) {
	return imfs_read(cageid, fd, buf, count);
}

ssize_t write_impl(int cageid, int fd, char *buf, ssize_t count) {
	if (fd < 3) {
		printf("%s", buf);
	}
	return imfs_write(cageid, fd, buf, count);
}

int open_impl(int cageid, char *pathname, int flags, mode_t mode) {
	return imfs_open(cageid, pathname, flags, mode);
}

int close_impl(int cageid, int fd) {
	return imfs_close(cageid, fd);
}

