#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define psleep(sec) sleep ((sec))
#define INITIAL_STOCK   20
#define NB_CLIENTS      5

typedef struct
{
   int stock;
   pthread_t thread_store;
   pthread_t thread_clients [NB_CLIENTS];
   pthread_mutex_t mutex_stock;
   pthread_cond_t cond_stock;
   pthread_cond_t cond_clients;
}
store_t;

static store_t store =
{
   .stock = INITIAL_STOCK,
   .mutex_stock = PTHREAD_MUTEX_INITIALIZER,
   .cond_stock = PTHREAD_COND_INITIALIZER,
   .cond_clients = PTHREAD_COND_INITIALIZER,
};

static int get_random (int max)
{
   double val;
   val = (double) max * rand ();
   val = val / (RAND_MAX + 1.0);
   return ((int) val);
}

static void * fn_store (void * p_data)
{
   while (1) {
      pthread_mutex_lock (& store.mutex_stock);
      pthread_cond_wait (& store.cond_stock, & store.mutex_stock);
      store.stock = INITIAL_STOCK;
      std::cout << "Remplissage du stock de " << store.stock <<  "articles !\n";
      pthread_cond_signal (& store.cond_clients);
      pthread_mutex_unlock (& store.mutex_stock);
   }
   return NULL;
}

static void * fn_clients (void * p_data)
{
   int nb = (int) p_data;
   while (1) {
      int val = get_random (6);
      psleep (get_random (3));
      pthread_mutex_lock (& store.mutex_stock);
      if (val > store.stock) {
         pthread_cond_signal (& store.cond_stock);
         pthread_cond_wait (& store.cond_clients, & store.mutex_stock);
      }
      store.stock = store.stock - val;
      std::cout << "Client " << nb << " prend " << val
                << " du stock, reste " << store.stock << "en stock !\n";
      pthread_mutex_unlock (& store.mutex_stock);
   }
   return NULL;
}printf

int main (void)
{
   int ret = pthread_create (&store.thread_store, NULL, fn_store, NULL);
   if (! ret) {
      for (int i = 0; i < NB_CLIENTS; i++) {
         ret = pthread_create (&store.thread_clients [i], NULL, fn_clients, (void *) i);
         if (ret)
            fprintf (stderr, "%s", strerror (ret));
      }
   }
   else
      fprintf (stderr, "%s", strerror (ret));
   for (int i = 0; i < NB_CLIENTS; i++) {
      pthread_join (store.thread_clients [i], NULL);
   }
   pthread_join (store.thread_store, NULL);
   return EXIT_SUCCESS;
}
