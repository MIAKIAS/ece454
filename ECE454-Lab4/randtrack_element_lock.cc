
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "defs.h"
#include "hash_list_lock.h"

#define SAMPLES_TO_COLLECT   10000000
#define RAND_NUM_UPPER_BOUND   100000
#define NUM_SEED_STREAMS            4

/* 
 * ECE454 Students: 
 * Please fill in the following team struct 
 */
team_t team = {
    "杨波修脚",                  /* Team name */

    "Weizhou Wang",                    /* Member full name */
    "1004421262",                 /* Member student number */
    "weizhou.wang@mail.utoronto.ca",                 /* Member email address */
};

unsigned num_threads;
unsigned samples_to_skip;

class sample;

class sample {
  unsigned my_key;
 public:
  sample *next;
  unsigned count;
  pthread_mutex_t lock;

  sample(unsigned the_key) {
    my_key = the_key; 
    count = 0;
    if (pthread_mutex_init(&lock, NULL) != 0) {
      printf("Mutex initialization failed...\n");
      exit(1);
    }
  };
  unsigned key(){return my_key;}
  void print(FILE *f){printf("%d %d\n",my_key,count);}
};

// This instantiates an empty hash table
// it is a C++ template, which means we define the types for
// the element and key value here: element is "class sample" and
// key value is "unsigned".  
hash<sample,unsigned> h;

/* Thread routine parameter */
typedef struct seed_stream {
  int start;
  int end;
} seed_stream;

/* Thread routine */
void* process_stream(void* param) {
  int i, j, k;
  int rnum;
  unsigned key;
  sample *s;
  seed_stream* stream = (seed_stream*)param;

  // process streams starting with different initial numbers
  for (i = stream->start; i <= stream->end; ++i) {
    rnum = i;

    // collect a number of samples
    for (j = 0; j < SAMPLES_TO_COLLECT; ++j) {
      // skip a number of samples
      for (k=0; k<samples_to_skip; k++) {
        rnum = rand_r((unsigned int*)&rnum);
      }
      // force the sample to be within the range of 0..RAND_NUM_UPPER_BOUND-1
      key = rnum % RAND_NUM_UPPER_BOUND;
      // if this sample has not been counted before
      h.lock_list(key);
      if (!(s = h.lookup(key))) {
        // insert a new element for it into the hash table
        s = new sample(key);
        h.insert(s);
      }
      h.unlock_list(key);

      // increment the count for the sample
      pthread_mutex_lock(&(s->lock));
      s->count++;
      pthread_mutex_unlock(&(s->lock));
      
    }
  }
  return NULL;
}

int  
main (int argc, char* argv[]){
  int i,j,k;
  int rnum;
  unsigned key;
  sample *s;

  // Print out team information
  printf( "Team Name: %s\n", team.team );
  printf( "\n" );
  printf( "Student 1 Name: %s\n", team.name1 );
  printf( "Student 1 Student Number: %s\n", team.number1 );
  printf( "Student 1 Email: %s\n", team.email1 );
  printf( "\n" );

  // Parse program arguments
  if (argc != 3){
    printf("Usage: %s <num_threads> <samples_to_skip>\n", argv[0]);
    exit(1);  
  }
  sscanf(argv[1], " %d", &num_threads); // not used in this single-threaded version
  sscanf(argv[2], " %d", &samples_to_skip);

  // initialize a 16K-entry (2**14) hash of empty lists
  h.setup(14);

  // Initialize threads
  pthread_t* thread = new pthread_t[num_threads];

  seed_stream* params = NULL;
  switch (num_threads)
  {
  case 1:
  {
    params = new seed_stream[1];
    params[0].start = 0;
    params[0].end = 3;
    pthread_create(&thread[0], NULL, process_stream, (void*)&params[0]);
    break;
  }
  case 2:
  {
    params = new seed_stream[2];
    params[0].start = 0;
    params[0].end = 1;
    params[1].start = 2;
    params[1].end = 3;
    pthread_create(&thread[0], NULL, process_stream, (void*)&params[0]);
    pthread_create(&thread[1], NULL, process_stream, (void*)&params[1]);
    break;
  }
  case 4:
  {
    params = new seed_stream[4];
    for (int i = 0; i < num_threads; ++i) {
      params[i].start = i;
      params[i].end = i;
      pthread_create(&thread[i], NULL, process_stream, (void*)&params[i]);
    }
    break;
  }
  default:
    break;
  }

  // Wait for all threads to complete
  for (int i = 0; i < num_threads; ++i) {
    if (pthread_join(thread[i], NULL) != 0) {
      printf("Error pthread_join()...\n");
      exit(1);
    }
  }

  delete []params;
  delete []thread;
  // print a list of the frequency of all samples
  h.print();
}
