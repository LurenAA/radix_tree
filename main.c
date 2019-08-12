#include "radix_tree.h"
char *toadd[] = {"alligator","alien","baloon","chromodynamic","romane","romanus","romulus","rubens","ruber","rubicon","rubicundus","all","rub","ba",NULL};

int main(int argc, char* argv[]) {
  radix_tree* tree = radixNewTree();
  radixInsert(tree, toadd[0], strlen(toadd[0]),(void*)"val");
  if(!tree) return 1;

  return 0;
}
