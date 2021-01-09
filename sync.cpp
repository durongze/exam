#include <unistd.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

class Detect{
    public:
        Detect(){}
        ~Detect(){}
        int loop()
        {
            std::unique_lock<std::mutex> lk(m);
            std::cout << __FUNCTION__ << "E" << std::endl;
            cv.wait(lk, [this]{return ready;});    
            sleep(1);
            std::cout << __FUNCTION__ << "X" << std::endl;
            processed=true; ready = false;
            cv.notify_one();
            return 0;
        }

        int process()
        {
            std::unique_lock<std::mutex> lk(m);
            std::cout << __FUNCTION__ << "E" << std::endl;
            cv.wait(lk, [this]{return processed;});
            std::cout << __FUNCTION__ << "X" << std::endl;
            ready = true; processed = false;
            cv.notify_one();
            return 0;
        }

        int l()
        {
            int ret = 0;
            do {
                ret = loop();
            } while(ret == 0);
            return 0;
        }

        int p()
        {
            int ret = 0;
            do {
                ret = process();
            } while(ret == 0);
            return 0;
        }

        int init()
        {
            std::thread ll(&Detect::l, this);
            ll.detach();
            return 0;
        }
        int proc()
        {
            std::thread pp(&Detect::p, this);
            pp.detach();
            return 0;
        }
    private:
        std::mutex m;
        std::condition_variable cv;
        std::string data;
        bool ready = false;
        bool processed = true;
};

int main(int argc, char** argv)
{
    Detect d;
    d.init();
    d.proc();
    getchar();
    return 0;    
}
