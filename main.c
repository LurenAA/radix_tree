#include "radix_tree.h"
#include "simpleQueue.h"
char *toadd[] = {"alligator", "all", "alien", "allibbc","baloon","chromodynamic","alligatorbbn","romane","romanus","romulus","rubens","ruber","rubicon","rubicundus","all","rub",NULL};
void string_had(void *);

int main(int argc, char* argv[]) {
  /*
  test tree
  */
  radix_tree* tree = radixNewTree();
  if(!tree) return 1;
  int num = 0;
  for(char **p = toadd; *p != NULL; ++p , ++num);
  fprintf(stderr, "num: %d\n", num);
  for(int i = 0; i < 10; ++i) {
    radixInsert(tree, toadd[i], strlen(toadd[i]), (void*)toadd[i]);
  }
  radixInsert(tree, "alligator", strlen("alligator"), (void*)"bbbb");
  traversalDebug(tree, string_had);
  /*
  test queue 
  */
  // simpleQueueDebug(toadd, 5);

  return 0;
}

void string_had(void *vid) {
  fprintf(stderr, "value:%s", (char*)vid);
}