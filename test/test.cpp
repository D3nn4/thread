#include <string>
#include <map>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <streambuf>
#include <vector>

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
    for(int i = res->start; i < res->stop; ++i){
        while(str[i] == ' ' || str[i] == '\0' || str[i] == '\n')
            ++i;
        pthread_mutex_lock(&m_char);
        std::map<char, int>::iterator it = res->list.find(str[i]);
        // if (str[i] == 't'){
        //     downOccus(res->list);
        // }
        if (it == res->list.end())
            res->list[str[i]] = 1;
        else
            res->list[str[i]] += 1;
        pthread_mutex_unlock(&m_char);
    }
    return NULL;
}

int main(int argc, char** argv)
{
    int num = atoi(argv[1]);
    std::map<char, int> l;
    std::ifstream file("text.txt");
    std::string str;
    std::vector<Result> argList;
    std::vector<pthread_t> threadList;
    for(int i = 0; i < num; ++i){
        pthread_t temp;
        threadList.push_back(temp);
    }
    while(std::getline(file, str)) {
        int gap =  str.size() / num;
        int max = gap;
        int min = 0;
        for (int i = 0; i < num; ++i){
            argList.push_back(Result(str, l, min,max));
            min = max;
            if (i == num - 2)
                max = str.size() - 1;
            else
                max = max + gap;
        }
        for(int i = 0; i < num; ++i){
            pthread_create(&threadList[i], NULL, mostOccu, &argList[i]);
        }
        pthread_join(threadList.back(), NULL);
    }
        char ret = getMax(l);
        std::cout << "C++ \n"<< l[ret] << " : " << ret << "\n\n";
        std::map<char, int>::iterator it = l.begin();
        // for(; it != l.end(); ++it) {
        //     std::cout << it->first << " : " << it->second << "\n";
        // }
    return 0;
}

