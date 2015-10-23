#ifndef __FILE_H_
#define __FILE_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <vector>


//0-success others-failed
int GetChildDirList(char *basedir_path, std::vector< std::string > dir_list)
{
    DIR     *base;
    struct  dirent *entry;
    struct  stat    statbuf;
    char    temp[1024];
    if (getcwd(temp, sizeof(temp)-1) == NULL) {
        //ERROR_LOG("%s cannot get current work dir", __func__);
        return -1;
    }
    if ((base = opendir(basedir_path)) == NULL) {
        //ERROR_LOG("%s cannot open dir %s", __func__,
        //       basedir_path);
        return -2;
    }
    chdir(basedir_path);
    while ((entry = readdir(base)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if (S_IFDIR & statbuf.st_mode) {
            if (strcmp(".", entry->d_name) == 0 ||
                    strcmp("..", entry->d_name) == 0)
            {
                continue;
            }
            dir_list.push_back(std::string(entry->d_name));
        }
    }
    chdir(temp);
    return 0;
}


#endif
