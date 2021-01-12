#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

int start_deamon(const char *workdir)
{
    pid_t pid, sid;
    
    // Fork off the parent process
    pid = fork();
    if (pid < 0)
    {
        exit(-1);
    }

    // If we got a good PID, then we can exit the parent process.
    if (pid > 0)
    {
        exit(-1);
    }

    // Change the file mode mask
    umask(0);

    // Create a new SID(Session ID) for the child process
    sid = setsid();
    if (sid < 0)
    {
        // Log any failure
        exit(-1);
    }

    // Change the current working directory
    if ((chdir(workdir)) < 0)
    {
        // Log any failure
        exit(-1);
    }

    // Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    return 0;
}

struct tm* get_cur_time()
{
    time_t timeNow;
    time(&timeNow);
    return localtime(&timeNow);
}

const std::string gen_cmd()
{
    std::string cmd;
    cmd += "dmake";
    cmd += " -h ";
    return cmd;
}

int main(int argc, char **argv)
{
    const char *workdir="./";
    const char *logfile="/var/log/daemon.log";
    struct tm run_tm = *get_cur_time();

    // Open any logs here
    ofstream fout(logfile);
    if (!fout)
    {
        exit(-1);
    }
    
    start_deamon(workdir);
    // The big loop
    while (true)
    {
        struct tm cur_tm = *get_cur_time();
        fout << asctime(&cur_tm) << endl;
        if (run_tm.tm_hour == cur_tm.tm_hour) {
            system(gen_cmd().c_str());
            break;
        }
        sleep(30);
    }
    cout << "over" << endl;        
    return 0;
}
