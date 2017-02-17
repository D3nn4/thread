#include <pthread.h>
#include <iostream>
#include <time.h>
#include <unistd.h>


void* fun1(void* arg)
{
  int i = *((int*)arg);
  std::cout << "n1 = " << i << "\n";
  sleep(2);
  pthread_exit(0);
}

void* fun2(void* arg)
{
  int i = *((int*)arg);
  std::cout << "n2 = " << i << "\n";
  sleep(4);
  pthread_exit(0);
}

int size = 0;
pthread_mutex_t m_prod = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_cons = PTHREAD_MUTEX_INITIALIZER;

void* producer(void* arg)
{
  while(42) {
    pthread_mutex_lock(&m_prod);
    ++size;
    pthread_mutex_unlock(&m_cons);
  }
}

void* consumer(void* arg)
{
  int i = 0;
  int v = 0;
  while(i < 100) {
    pthread_mutex_lock(&m_cons);
    v = size;
    pthread_mutex_unlock(&m_prod);
    std::cout << i << " : " << v << "\n";
    ++i;
  }
}

int main()
{
  pthread_t th1;
  pthread_t th2;
  pthread_t th_producer;
  pthread_t th_consumer;
  int n1 = 42;
  int n2 = 86;

  pthread_create(&th1, NULL, fun1, &n1);
  pthread_create(&th2, NULL, fun2, &n2);
  pthread_mutex_lock(&m_cons);
  pthread_create(&th_producer, NULL, producer, NULL);
  pthread_create(&th_consumer, NULL, consumer, NULL);


  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th_consumer, NULL);
  return (0);
}
