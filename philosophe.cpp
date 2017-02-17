#include "philosophe.hpp"

Philosophe::Philosophe(int num, std::vector<Philosophe>& tab)
    :number(num),
     table(tab),
     mutex(PTHREAD_MUTEX_INITIALIZER),
     isEating(false),
     state(SLEEP){}

void Philosophe::eat()
{
    state = Status::EAT;
    std::cout << "Philosophe " << number << " is eating.\n";
    sleep(4);
}

void Philosophe::sleep()
{
    state = Status::SLEEP;
    std::cout << "Philosophe " << number << " is sleeping.\n";
}

void Philosophe::think()
{
    state = Status::THINK;
    std::cout << "Philosophe " << number << " is thinking.\n";
}

void Philosophe::putCS(int i)
{
    pthread_mutex_lock(&mutex);
    state = Status::SLEEP;
    int left = ((i - 1) >= 0) ? (i - 1)%6 : 6;
    int right = (i == 6) ? 0 : i + 1;
    test(left);
    test(right);
    pthread_mutex_unlock(&mutex);
}

void test (int i)
{
    int left = ((i - 1) >= 0) ? (i - 1)%6 : 6;
    int right = (i == 6) ? 0 : i + 1;
    if(state != Status::HUNGRY){
        // if(table[left].state != Status::THINK)
        //     test(left);
        // if(table[right].state != Status::THINK)
        //     test(right);
        if(table[left].state != Status::EAT
           && table[right].state != Status::EAT)
            state = Status::EAT;
        isEating = true
        //condition varable : send ok ca eat
    }
}
void Philosophe::takeCS(int i)
{
    pthread_mutex_lock(&mutex);
    state = Status::HUNGRY;
    test(i);
    pthread_mutex_unlock(&mutex);
    while(isEating == false)
        //do stuff
     
    //condition variable : attendre ok
}
void Philosophe::doStuff()
{
    while(true) {
        sleep();
        takeCS(number);
        eat();
        putCS(number);
    }
}
