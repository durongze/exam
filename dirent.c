#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <io.h>
int ReadFileByDir(std::string dirName, std::string fileExt, std::vector<std::string>& files)
{
	struct _finddata_t s_file;
	std::string str = dirName + "\\*" + fileExt;
	bool withDir = true;
	intptr_t h_file = _findfirst(str.c_str(), &s_file);
	if (h_file != static_cast<intptr_t>(-1.0)) {
		do {
			if (withDir) {
				files.push_back(dirName + "\\" + s_file.name);
			} else {
				files.push_back(std::string(s_file.name));
			}
		} while (_findnext(h_file, &s_file) == 0);
	}
	_findclose(h_file);
	return 0;
}
#endif
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
    int idx = 0, ret = 0;
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
        printf("name:%s,\n", pdir->d_name);       
        if (8 == pdir->d_type)
        {
            FILE* fp = fopen(pdir->d_name, "r");
            if (fp != NULL)
            {
                ret = fseek(fp, -1, SEEK_END);
                printf("fseek:%d\n", ret);
                long fsize = ftell(fp);
                char* ctx = (char*) malloc(fsize+1);
                rewind(fp);
                //ret = fread(ctx, 1, fsize, fp);
                printf("fread:%d\n", ret);
                char* s = fgets(ctx, fsize, fp);
                printf("fgets:%s\n",s);
            }
            else
            {
                fclose(fp);
            }
        }
        pdir = readdir(handle);
    }
    closedir(handle);
}
