#include <pthread.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <vector>
#include "philosophe.hpp"

int main()
{

    int total_p = 7;
    std::vector<Philosophe> table;
    for(int i = 0; i < total_p; ++i) {
        table.push_back(i, table);
    }
    return 0;
}
