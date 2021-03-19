#include "pl0.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern unsigned char *cur;

void error(const char msg[])
{
    printf("%s\n", msg);
    exit(1);
}


/*
 * This function was taken from: https://stackoverflow.com/a/22515917/479532
 *
 * 'slurp' reads the file identified by 'path' into a character buffer
 * pointed at by 'buf', optionally adding a terminating NUL if
 * 'add_nul' is true. On success, the size of the file is returned; on
 * failure, -1 is returned and ERRNO is set by the underlying system
 * or library call that failed.
 *
 * WARNING: 'slurp' malloc()s memory to '*buf' which must be freed by
 * the caller.
 *
 */
long slurp(char const* path, char **buf, bool add_nul)
{
    FILE  *fp;
    size_t fsz;
    long   off_end;
    int    rc;

    /* Open the file */
    fp = fopen(path, "rb");
    if( NULL == fp ) {
        return -1L;
    }

    /* Seek to the end of the file */
    rc = fseek(fp, 0L, SEEK_END);
    if( 0 != rc ) {
        return -1L;
    }

    /* Byte offset to the end of the file (size) */
    if( 0 > (off_end = ftell(fp)) ) {
        return -1L;
    }
    fsz = (size_t)off_end;

    /* Allocate a buffer to hold the whole file */
    *buf = malloc( fsz+(int)add_nul );
    if( NULL == *buf ) {
        return -1L;
    }

    /* Rewind file pointer to start of file */
    rewind(fp);

    /* Slurp file into buffer */
    if( fsz != fread(*buf, 1, fsz, fp) ) {
        free(*buf);
        return -1L;
    }

    /* Close the file */
    if( EOF == fclose(fp) ) {
        free(*buf);
        return -1L;
    }

    if( add_nul ) {
        /* Make sure the buffer is NUL-terminated, just in case */
        buf[fsz] = (char*)'\0';
    }

    /* Return the file size */
    return (long)fsz;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: ./pl0 <filename>\n");
        exit(1);
    }
    long file_size;
    char *buf;
    file_size = slurp(argv[1], &buf, false);
    if (file_size < 0) {
        printf("File '%s' read failed.\n", argv[1]);
        exit(1);
    }
    cur = (unsigned char*)buf;
    nextsym();
    program();
    printf("OK\n");
    return 0;
}
