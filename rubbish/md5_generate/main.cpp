#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "md5.h"

#ifndef MD5_DIGEST_LENGTH
#define MD5_DIGEST_LENGTH 16
#endif

static int md5_ADD[MD5_DIGEST_LENGTH * 2] = {1, 2, 3, 2, 0, 3, 4, 6,
                                             0, 4, 1, 5, 2, 4, 1, 4,
                                             6, 2, 0, 5, 1, 3, 5, 2,
                                             0, 2, 4, 3, 5, 2, 3, 1};

static int do_md5(const char *path, char *outMD5)
{
    unsigned int i;
    int fd;
    MD5_CTX md5_ctx;
    unsigned char md5[MD5_DIGEST_LENGTH];

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr,"could not open %s, %s\n", path, strerror(errno));
        return -1;
    }

    /* Note that bionic's MD5_* functions return void. */
    MD5_Init(&md5_ctx);

    while (1) {
        char buf[4096];
        ssize_t rlen;
        rlen = read(fd, buf, sizeof(buf));
        if (rlen == 0)
            break;
        else if (rlen < 0) {
            (void)close(fd);
            fprintf(stderr,"could not read %s, %s\n", path, strerror(errno));
            return -1;
        }
        MD5_Update(&md5_ctx, buf, rlen);
    }
    if (close(fd)) {
        fprintf(stderr,"could not close %s, %s\n", path, strerror(errno));
        return -1;
    }

    MD5_Final(md5, &md5_ctx);

    memset(outMD5, 0, MD5_DIGEST_LENGTH*2 + 1);

    for(i=0;i<MD5_DIGEST_LENGTH;i++){
        char tmp[3];
        sprintf(tmp, "%02x", md5[i]);
        strcat(outMD5, tmp);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int i;
    char temp;
    char file_path[256] = {0};
    char cmd_buf[256] = {0};
    char file_md5[MD5_DIGEST_LENGTH * 2 + 1] = {0};

    if (argc != 3) {
        printf("Usage: %s md5_input_file md5_output_file\n", argv[0]);
        return -1;
    }

    sprintf(file_path, "%s", argv[1]);

    if (do_md5(file_path, file_md5) == 0) {
        printf("md5: %s\n", file_md5);
        for (i = 0; i < MD5_DIGEST_LENGTH * 2; i++) {
            temp = file_md5[i] + md5_ADD[i];
            if (!((temp > '9') && (temp < 'A')))
                file_md5[i] = temp;
        }
        printf("md5: %s\n", file_md5);
        sprintf(cmd_buf, "echo %s > %s", file_md5, argv[2]);
        system(cmd_buf); 
    }

    return 0;
}

