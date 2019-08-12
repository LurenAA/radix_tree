#include "radix_tree.h"
#include "simpleQueue.h"
char *toadd[] = {"alligator","alien","baloon","chromodynamic","romane","romanus","romulus","rubens","ruber","rubicon","rubicundus","all","rub","ba",NULL};
void string_had(void *);

int main(int argc, char* argv[]) {
  /*
  test tree
  */
  radix_tree* tree = radixNewTree();
  if(!tree) return 1;
  radixInsert(tree, toadd[0], strlen(toadd[0]),(void*)"val");
  radixInsert(tree, toadd[1], strlen(toadd[1]),(void*)"val1");
  // radixInsert(tree, toadd[2], strlen(toadd[2]),(void*)"val2");
  traversalDebug(tree, string_had);
  /*
  test queue 
  */
  // simpleQueueDebug(toadd, 5);

  return 0;
}

void string_had(void *vid) {
  fprintf(stderr, "%s", (char*)vid);
}