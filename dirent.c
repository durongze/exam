#include <dirent.h>
#include  <stdio.h>
/*
 *struct dirent {
     ino_t          d_ino;       //Inode number 
     off_t          d_off;       //Not an offset; see below 
     unsigned short d_reclen;    // Length of this record 
     unsigned char  d_type;      // Type of file; not supported by all filesystem types 
     char           d_name[256];  // Null-terminated filename 
  };
 *
 * */
int main()
{
    int idx = 0;
    struct dirent* pdir = NULL;
    DIR* handle = opendir(".");
    pdir = readdir(handle);
    while (pdir != NULL)
    {
        ++idx;
        printf("===========%d===========\n", idx);       
        printf("ino:%d,\n", pdir->d_ino);       
        printf("off:%d,\n", pdir->d_off);       
        printf("reclen:%d,\n", pdir->d_reclen);       
        printf("type:%d,\n", pdir->d_type);       
        printf("type:%s,\n", pdir->d_name);       
        pdir = readdir(handle);
    }
    closedir(handle);
}
