/* Auto-generated: DO NOT MODIFY */

#include <lind_syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "grates.h"
#include "imfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern void grate_init(void);
extern void grate_destroy(void);

ssize_t read_impl(int, int, char *, ssize_t);
ssize_t write_impl(int, int, char *, ssize_t);
int open_impl(int, char *, int, mode_t);
int close_impl(int, int);

ssize_t read_impl_grate(uint64_t cageid, uint64_t arg1, uint64_t arg1cage,
                        uint64_t arg2, uint64_t arg2cage, uint64_t arg3,
                        uint64_t arg3cage, uint64_t arg4, uint64_t arg4cage,
                        uint64_t arg5, uint64_t arg5cage, uint64_t arg6,
                        uint64_t arg6cage) {

  int thiscage = getpid();

  int fd = arg1;
  ssize_t count = arg3;

  char *buf = malloc(count);

  if (buf == NULL) {
    perror("malloc failed");
    return -1;
  }

  ssize_t ret = read_impl(cageid, fd, buf, count);

  copy_data_between_cages(thiscage, arg2cage, (uint64_t)buf, thiscage, arg2,
                          arg2cage, count, 0);

  free(buf);

  return ret;
}

ssize_t write_impl_grate(uint64_t cageid, uint64_t arg1, uint64_t arg1cage,
                         uint64_t arg2, uint64_t arg2cage, uint64_t arg3,
                         uint64_t arg3cage, uint64_t arg4, uint64_t arg4cage,
                         uint64_t arg5, uint64_t arg5cage, uint64_t arg6,
                         uint64_t arg6cage) {

  int thiscage = getpid();

  int fd = arg1;
  ssize_t count = arg3;

  char *buf = malloc(count);

  if (buf == NULL) {
    perror("malloc failed");
    return -1;
  }

  copy_data_between_cages(thiscage, arg2cage, arg2, arg2cage, (uint64_t)buf,
                          thiscage, count, 0);

  ssize_t ret = write_impl(cageid, fd, buf, count);

  free(buf);

  return ret;
}

int open_impl_grate(uint64_t cageid, uint64_t arg1, uint64_t arg1cage,
                    uint64_t arg2, uint64_t arg2cage, uint64_t arg3,
                    uint64_t arg3cage, uint64_t arg4, uint64_t arg4cage,
                    uint64_t arg5, uint64_t arg5cage, uint64_t arg6,
                    uint64_t arg6cage) {

  int thiscage = getpid();

  int flags = arg2;
  mode_t mode = arg3;

  char *pathname = malloc(256);

  if (pathname == NULL) {
    perror("malloc failed");
    return -1;
  }

  copy_data_between_cages(thiscage, arg1cage, arg1, arg1cage,
                          (uint64_t)pathname, thiscage, 256, 0);

  int ret = open_impl(cageid, pathname, flags, mode);

  free(pathname);

  return ret;
}

int close_impl_grate(uint64_t cageid, uint64_t arg1, uint64_t arg1cage,
                     uint64_t arg2, uint64_t arg2cage, uint64_t arg3,
                     uint64_t arg3cage, uint64_t arg4, uint64_t arg4cage,
                     uint64_t arg5, uint64_t arg5cage, uint64_t arg6,
                     uint64_t arg6cage) {

  int thiscage = getpid();

  int fd = arg1;

  int ret = close_impl(cageid, fd);

  return ret;
}

void register_handlers(int cageid, int grateid) {

  register_handler(cageid, 0, 1, grateid,
                   (uint64_t)(uintptr_t)&read_impl_grate);

  register_handler(cageid, 1, 1, grateid,
                   (uint64_t)(uintptr_t)&write_impl_grate);

  register_handler(cageid, 2, 1, grateid,
                   (uint64_t)(uintptr_t)&open_impl_grate);

  register_handler(cageid, 3, 1, grateid,
                   (uint64_t)(uintptr_t)&close_impl_grate);
}

// Dispatcher function
int pass_fptr_to_wt(uint64_t fn_ptr_uint, uint64_t cageid, uint64_t arg1,
                    uint64_t arg1cage, uint64_t arg2, uint64_t arg2cage,
                    uint64_t arg3, uint64_t arg3cage, uint64_t arg4,
                    uint64_t arg4cage, uint64_t arg5, uint64_t arg5cage,
                    uint64_t arg6, uint64_t arg6cage) {
  if (fn_ptr_uint == 0) {
    return -1;
  }

  int (*fn)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
            uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
            uint64_t) =
      (int (*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
               uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
               uint64_t))(uintptr_t)fn_ptr_uint;

  return fn(cageid, arg1, arg1cage, arg2, arg2cage, arg3, arg3cage, arg4,
            arg4cage, arg5, arg5cage, arg6, arg6cage);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <cage_file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int grateid = getpid();

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    int cageid = getpid();
    register_handlers(cageid, grateid);

    if (execv(argv[1], &argv[1]) == -1) {
      perror("execv failed");
      exit(EXIT_FAILURE);
    }
  } else {
    grate_init();
  }

  int status;
  while (wait(&status) > 0) {
    grate_destroy();
  }

  return 0;
}
