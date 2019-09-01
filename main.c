#include "radix_tree.h"
#include "simpleQueue.h"
char *toadd[] = {
"/$/123", 
"/abc/sss", 
"/", 
"/one/two/three",
"/ppp",
NULL};
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
  for(int i = 0; i < num; ++i) {
    radixInsert(tree, toadd[i], strlen(toadd[i]), (void*)toadd[i]);
  }
  // traversalDebug(tree, string_had);
  fprintf(stderr, "%s\n", (char*)route(tree, "/abc/sss"));

  return 0;
}

void string_had(void *vid) {
  fprintf(stderr, "value:%s", (char*)vid);
}

