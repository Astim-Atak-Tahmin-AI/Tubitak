#include "logger.h"
#include <FS.h>
#include <LittleFS.h>

bool logger_init(bool formatIfFail){
    if(!LittleFS.begin(formatIfFail)){
        return false;
    }
    return true;
}

bool logger_appendLine(const char* path, const String& line){
    File f=LittleFS.open(path,"a");
    if(!f) return false;

    bool ok=(f.print(line)&& f.print("\n"));
    f.close();
    return ok;

}


size_t logger_fileSize(const char* path){
    File f=LittleFS.open(path,"r");
    if(!f) return 0;
    size_t sz=f.size();
    f.close();
    return sz;
}


bool logger_rotateIfTooBig(const char* path,size_t maxBytes, const char* rotatedPath){
    size_t sz=logger_fileSize(path);
    if(sz<=maxBytes) return true;


    if(LittleFS.exists(rotatedPath)) LittleFS.remove(rotatedPath);
    if(!LittleFS.rename(path,rotatedPath)){
        return false;
    }

    return true;

}