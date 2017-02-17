#include <string>
#include <map>
#include <iostream>
#include <pthread.h>

pthread_mutex_t m_char = PTHREAD_MUTEX_INITIALIZER;

struct Result
{
    Result(std::string s, std::map<char, int>&l, int b, int e)
        :str(s),
         list(l),
         start(b),
         stop(e){};
    std::string str;
    std::map<char, int>& list;
    char ret;
    int start;
    int stop;
};

char getMax(std::map<char, int>& list){
    std::map<char, int>::iterator it = list.begin();
    char max = it->first;
    for(; it != list.end(); ++it) {
        if (list[max] < it->second)
            max = it->first;
    }
    return max;
};

void downOccus(std::map<char, int>& list)
{
    std::map<char, int>::iterator it = list.begin();
    for(; it != list.end(); ++it){
        it->second -= 1;
    }
}

void* mostOccu(void* result)
{
    Result* res = (Result*) result;
    std::string str = res->str;
    for(int i = res->start; i <= res->stop; ++i){
        while(str[i] == ' ' || str[i] == '\0' || str[i] == '\n')
            ++i;
        pthread_mutex_lock(&m_char);
        std::map<char, int>::iterator it = res->list.find(str[i]);
        if (str[i] == 't'){
            downOccus(res->list);
        }
        if (it == res->list.end())
            res->list[str[i]] = 1;
        else
            res->list[str[i]] += 1;
        pthread_mutex_unlock(&m_char);
    }
}

int main(int argc, char** argv)
{
    if(argc == 2) {
        std::string str = argv[1];
        std::map<char, int> l;
        int mid = str.size() / 2;
        Result res_th1(str, l, 0, mid);
        Result res_th2(str, l, mid + 1, str.size());
        pthread_t th1;
        pthread_t th2;
        pthread_create(&th1, NULL, mostOccu, &res_th1);
        pthread_create(&th2, NULL, mostOccu, &res_th2);
        pthread_join(th1, NULL);
        pthread_join(th2, NULL);
        char ret = getMax(l);
        std::cout << l[ret] << " : " << ret << "\n\n";
        std::map<char, int>::iterator it = l.begin();
        for(; it != l.end(); ++it) {
            std::cout << it->first << " : " << it->second << "\n";
        }
    }
    return 0;
}

