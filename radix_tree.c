#include "radix_tree.h"

#define RADIX_DEBUG_MSG
/**
 * 是否显示debug信息
 **/
#ifdef RADIX_DEBUG_MSG
#define debugf(...) \
  fprintf(stderr, "debug: %s:%s:%d:\t", __FILE__, __FUNCTION__, __LINE__); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  fflush(NULL);
#else
#define debugf(...)
#endif

/**
 * 充填字节
 **/
#define padding(n) \
  ((sizeof(void*) - (sizeof(radix_node) + n) % sizeof(void*)) & (sizeof(void*) - 1))

/**
 * 获取当前长度
 **/
#define radixLength(h) \
  (sizeof(radix_node) + padding(h->size) + h->size + \
  h->is_compressed ? sizeof(radix_node*) : h->size * sizeof(radix_node*) + \
  h->is_key ? sizeof(void*) : 0)

/**
 * 获取值
 **/
#define radixGetVal(h) \
  (void**)((char*)h + radixLength(h) - sizeof(void*)) 

/**
 * 第一个子节点
 **/
#define radixFirstChild(h) \
  (radix_node**)((char*)&h->data + h->size + padding(h->size))

/**
 * 初始化一个基数树
 * @param null
 **/
radix_tree* radixNewTree() {
  radix_tree* tree = (radix_tree*)malloc(sizeof(radix_tree));
  if(!tree) {
    debugf("%s", strerror(errno));
    return NULL;
  }
  tree->root = radixNewNode(0, false, false);
  if(!tree->root) {
    debugf("%s", strerror(errno));
    return NULL;
  }
  tree->key_numbers = 0;
  tree->node_numbers = 0;

  debugf("init a radix_tree");
  return tree;
}

/**
 * 初始化一个基数树的节点
 *  @param：children 子节点数量
 *  @param: is_key 是否有键
 *  @param: is_compressed 
 **/ 
radix_node* radixNewNode(int children, bool is_key, bool is_compressed) {
  int size = sizeof(radix_node) + children + 
    padding(children) + 
    is_compressed ? sizeof(radix_node*) : sizeof(radix_node*) * children;
  if(is_key) size += sizeof(void*);
  radix_node* node = (radix_node*)malloc(size);
  if(!node) {
    debugf("%s", strerror(errno));
    return NULL;
  }
  node->is_compressed = is_compressed;
  node->size = children;
  node->is_null = 0;
  node->is_key = is_key;

  debugf("init a radix_node(%d)", size);
  return node;
}

/**
 * 插入一个节点
 * @param: key 键
 * @param: len 键长
 * @param: val 值
 * @return: 1插入成功，0插入失败,2覆盖原来的
 **/
uint8_t radixInsert(radix_tree* tree, char* key, int len,void* val) {
  int i = 0; //当前key的结束点
  int j = 0; //在树中节点的结束处
  radix_node *cur, **plink; //cur当前节点， plink父节点
  i = linkLowWalk(tree, key, &cur, &plink, &j);

  debugf("linkLowWalk(i): %d", i);

  if(cur->is_compressed && i != len) {

  } 
  else if (cur->is_compressed && i == len){

  }
  else {
    if(!cur->size) { //避免形成链式
      bool pIsKey = cur->is_key ? true : false;
      radix_node *node = radixNewNode(len, pIsKey, true);
      if(!node) {
        debugf("insert error: %s", strerror(errno));
        return 0;
      }

      if(cur->is_key) {
        // char *val = 
      }
      memcpy(&node->data, key, len); 
    debugf("node->data: %.*s", len, (char*)&node->data);
      memcpy(plink, &node, sizeof(void*)); 
      radix_node *end = radixNewNode(0, true, false);
      if(!end) {
        debugf("insert error: %s", strerror(errno));
        return 0;
      }
      radixSetData(end, val);
    debugf("end->val: %s", (char*)radixGetData(end));
      radix_node **firstChild = radixFirstChild(node);
      memcpy(firstChild, &end, sizeof(radix_node*));
    debugf("first_child: 0x%p", *firstChild);
      return 1;
    } 
    else {

    }
  }

  return 0;
}

/**
 *  遍历到不匹配的地方
 *  @param：tree
 *  @param：key 键
 *  @param：cur 当前遍历节点
 *  @param：plink 指向cur的指针
 *  @param：j 记录在cur中的索引
 *  @return: i key的索引
 **/ 
int linkLowWalk(radix_tree* tree, const char* key,
  radix_node** cur,radix_node*** plink, int* j)
{
  int i = 0, lj = 0;
  radix_node *lcur = tree->root,
    **lplink = &tree->root; 
  while(lcur->size) {
    //todo
  }
  *cur = lcur;
  *plink = lplink; 
  if(lcur->is_compressed) *j = lj;
  return i;
}

/**
 * 设置值
 **/ 
void radixSetData(radix_node* node, void* val) {
  if(!node->is_key) return ;
  void** valPtr = radixGetVal(node);
  memcpy(valPtr, &val, sizeof(void*));
}

/**
 * 获取值
 **/
void* radixGetData(radix_node* node) {
  if(!node || !node->is_key) return NULL;
  void **data = radixGetVal(node);
  void *res;
  memcpy(&res, data, sizeof(void*));
  // res = *data;  
  return res;
}

/**
 *  debug
 **/ 
void traversalDebug(radix_tree* tree) {
#define RAX_LEN 100
  if(!tree) return;
  radix_node* nodes[RAX_LEN], *tar;
  memset(nodes, 0, sizeof(radix_node*) * RAX_LEN);
  int size = 1, ch_size = 0;
  nodes[0] = tree->root;
  while(size) {
    
  }
}