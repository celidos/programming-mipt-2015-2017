#include <thread_pool.h>
#include <iostream>

int main(int argc, char *argv[])
{
    //std::cout << "Hi !!!!l!" << std::endl;

    thread_pool<int> pool(5);
    pool.shutdown();
    std::cout << "Now waiting " << std::endl;
    return 0;
}
