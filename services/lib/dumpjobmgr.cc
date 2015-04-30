#include <stdio.h>
#include "jobmgr.ph"

void Usage() {
    printf("dumpsysinfo <contact>\n");
}
int main(int argc, char** argv) {
    if (argc != 2) {
        Usage();
        return 1;
    }
    try {
        pop_accesspoint job;
        job.SetAccessString(argv[1]);
        JobMgr tmp(job);
        tmp.dump();
    } catch (int e) {
        errno = e;
        pop_system::perror("Exception...");
    }
    return 0;
}
