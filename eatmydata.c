#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/limits.h>

#include <sys/prctl.h>
#include <sys/syscall.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static struct sock_filter filter[] = {
#include "filter.h"
};
static struct sock_fprog prog = {
    .len = (unsigned short)ARRAY_SIZE(filter),
    .filter = filter,
};

int main(int argc, char *argv[argc]) {
    int ret;

    if(argc < 2) {
        errno = ENOENT;
        goto error;
    }

    ret = prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog);
    if(ret)
        goto error;

    execvp(argv[1], &argv[1]);
error:
    fprintf(stderr, "%s\n", strerror(errno));
    exit(errno);
}
