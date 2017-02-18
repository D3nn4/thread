#include <string>
#include <map>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <streambuf>
#include <vector>

struct Value
{
    Value():occu(0){mutex = PTHREAD_MUTEX_INITIALIZER;}
    void down(){
        pthread_mutex_lock(&mutex);
        occu -= 1;
        pthread_mutex_unlock(&mutex);
    }
    void up(){
        pthread_mutex_lock(&mutex);
        occu += 1;
        pthread_mutex_unlock(&mutex);
    }

    int occu;
    pthread_mutex_t mutex;
};


struct Result
{
    Result(std::string s, std::map<char, Value>&l, int b, int e)
        :str(s),
         list(l),
         start(b),
         stop(e){};
    std::string str;
    std::map<char, Value>& list;
    char ret;
    int start;
    int stop;
};


char getMax(std::map<char, Value>& list){
    std::map<char, Value>::iterator it = list.begin();
    char max = it->first;
    for(; it != list.end(); ++it) {
        if (list[max].occu < (it->second).occu)
            max = it->first;
    }
    return max;
};

// void downOccus(std::map<char, int>& list)
// {
//     std::map<char, int>::iterator it = list.begin();
//     for(; it != list.end(); ++it){
//         it->second -= 1;
//     }
// }

void* mostOccu(void* result)
{
    Result* res = (Result*) result;
    std::string str = res->str;
    for(int i = res->start; i < res->stop; ++i){
        while(str[i] == ' ' || str[i] == '\0' || str[i] == '\n')
            ++i;
        // if (str[i] == 't'){
        //     downOccus(res->list);
        // }
        (res->list[str[i]]).up();
    }
    return NULL;
}

void abcMap(std::map<char, Value>& abc)
{
    for(int i = 'a'; i <= 'z'; ++i){
        abc[i] = Value();
    }
}

int main(int argc, char** argv)
{
    if (argc == 1)
        exit(0);
    int num = atoi(argv[1]);
    std::map<char, Value> l;
    abcMap(l);
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
        std::cout << "C++ \n"<< l[ret].occu << " : " << ret << "\n\n";
        std::map<char, Value>::iterator it = l.begin();
        // for(; it != l.end(); ++it) {
        //     std::cout << it->first << " : " << it->second << "\n";
        // }
    return 0;
}

