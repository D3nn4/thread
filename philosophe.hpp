#ifndef __PHILOSOPHE_H__
#define __PHILOSOPHE_H__
#include <string>
#include <stdbool.h>


class Philosophe
{

	public:

    class enum Status{
        SLEEP,
            EAT,
            THINK,
            HUNGRY
    }
        Philosophe(int num, std::vector<Philosophe>& tab);
    void eat();
    void think();
    void sleep();

    int number;
    std::vector<Philosophe> table;
    Status state;
    pthread_mutex_t mutex;
    bool isEating;
};

#endif // __PHILOSOPHE_H__
