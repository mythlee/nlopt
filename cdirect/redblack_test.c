#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "redblack.h"

static int comp(int k1, int k2, void *dummy)
{
     (void) dummy;
     return k1 - k2;
}

int main(int argc, char **argv)
{
     int N, M;
     int *k;
     rb_tree t;
     rb_node *n;
     int i, j;

     if (argc != 2) {
	  fprintf(stderr, "Usage: redblack_test Ntest\n");
	  return 1;
     }

     N = atoi(argv[1]);
     k = (int *) malloc(N * sizeof(int));
     if (!rb_tree_init(&t, comp, NULL)) {
	  fprintf(stderr, "error in rb_tree_init\n");
	  return 1;
     }

     srand((unsigned) time(NULL));
     for (i = 0; i < N; ++i) {
	  if (!rb_tree_insert(&t, k[i] = rand() % N)) {
	       fprintf(stderr, "error in rb_tree_insert\n");
	       return 1;
	  }
	  if (!rb_tree_check(&t)) {
	       fprintf(stderr, "rb_tree_check_failed after insert!\n");
	       return 1;
	  }
     }
     
     for (i = 0; i < N; ++i)
	  if (!rb_tree_find(&t, k[i]) || !rb_tree_find_exact(&t, k[i])) {
	       fprintf(stderr, "rb_tree_find lost %d!\n", k[i]);
	       return 1;
	  }
     
     n = rb_tree_min(&t);
     for (i = 0; i < N; ++i) {
	  if (!n) {
	       fprintf(stderr, "not enough successors %d\n!", i);
	       return 1;
	  }
	  printf("%d: %d\n", i, n->k);
	  n = rb_tree_succ(&t, n);
     }
     if (n) {
	  fprintf(stderr, "too many successors!\n");
	  return 1;
     }
     
     n = rb_tree_max(&t);
     for (i = 0; i < N; ++i) {
	  if (!n) {
	       fprintf(stderr, "not enough predecessors %d\n!", i);
	       return 1;
	  }
	  printf("%d: %d\n", i, n->k);
	  n = rb_tree_pred(&t, n);
     }
     if (n) {
	  fprintf(stderr, "too many predecessors!\n");
	  return 1;
     }
     
     for (M = N; M > 0; --M) {
	  int knew = rand() % N; /* random new key */
	  j = rand() % M; /* random original key to replace */
	  for (i = 0; i < N; ++i)
	       if (k[i] >= 0)
		    if (j-- == 0)
			 break;
	  if (i >= N) abort();
	  if (!(n = rb_tree_find(&t, k[i])) || !rb_tree_find_exact(&t, k[i])) {
               fprintf(stderr, "rb_tree_find lost %d!\n", k[i]);
               return 1;
          }
	  n->k = knew;
	  if (!rb_tree_resort(&t, n)) {
	       fprintf(stderr, "error in rb_tree_resort\n");
	       return 1;
	  }
	  if (!rb_tree_check(&t)) {
	       fprintf(stderr, "rb_tree_check_failed after change!\n");
	       return 1;
	  }
	  k[i] = -1 - knew;
     }

     for (i = 0; i < N; ++i)
	  k[i] = -1 - k[i];
     
     for (M = N; M > 0; --M) {
	  j = rand() % M;
	  for (i = 0; i < N; ++i)
	       if (k[i] >= 0)
		    if (j-- == 0)
			 break;
	  if (i >= N) abort();
	  if (!(n = rb_tree_find(&t, k[i])) || !rb_tree_find_exact(&t, k[i])) {
	       fprintf(stderr, "rb_tree_find lost %d!\n", k[i]);
	       return 1;
	  }
	  rb_tree_remove(&t, n);
	  if (!rb_tree_check(&t)) {
	       fprintf(stderr, "rb_tree_check_failed after remove!\n");
	       return 1;
	  }
	  k[i] = -1 - k[i];
     }
     
     rb_tree_destroy(&t);
     free(k);
     return 0;
}
