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
            cv.wait(lk, [this]{if(!run)return true;return ready;});    
            if (run) {
                if (!data) {printf("exit");exit(1);}
                sleep(8);
            }
            std::cout << __FUNCTION__ << "X" << std::endl;
            processed=true; ready = false;
            cv.notify_one();
            return 0;
        }

        int process()
        {
            sleep(1);
            if(m.try_lock()) {
            // if(1) { m.lock();
                if (!run) {
                    std::cout << __FUNCTION__ << "Quit" << std::endl; 
                    m.unlock();
                    return 1;
                }
                std::cout << __FUNCTION__ << "E" << std::endl;
                data = (rand() % 10 == 0);
                if (!data) { 
                    std::cout << "\033[31m" << __FUNCTION__ << "Xe\033[0m" << std::endl;
                    m.unlock();return 0;
                } else {
                    std::cout << "\033[32m" << __FUNCTION__ << "Xf\033[0m" << std::endl;
                }
                ready = true; processed = false;
                
                m.unlock();
                cv.notify_one();
            } else {
                std::cout << __FUNCTION__ << "Skip" << std::endl; 
            }
            return 0;
        }

        int l()
        {
            int ret = 0;
            do {
                ret = loop();
                if (ret == 1) {
                    break;
                }
            } while(run);
            printf("lllll<<<\n");
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

        int d()
        {
            while (rand() % 99 != 0) {
                sleep(1);
            }
            m.lock();
            printf("run false\n");
            run = false;
            m.unlock();
            cv.notify_one();
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
        int deinit()
        {
            std::thread dd(&Detect::d, this);
            dd.detach();
            return 0;
        }
    private:
        std::mutex m;
        std::condition_variable cv;
        bool data = false;
        bool ready = false;
        bool processed = true;
        bool run = true;
};

int main(int argc, char** argv)
{
    Detect d;
    d.init();
    d.deinit();
    d.proc();
    getchar();
    return 0;    
}
