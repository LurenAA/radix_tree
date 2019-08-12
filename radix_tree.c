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
  (h->is_compressed ? sizeof(radix_node*) : h->size * sizeof(radix_node*)) + \
  (h->is_key ? sizeof(void*) : 0))

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
 * 第n个子节点
 **/
#define radixNthChild(h, n) \
  (radix_node**)((char*)&h->data + h->size + padding(h->size) + n * sizeof(void*))

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
    (is_compressed ? sizeof(radix_node*) : sizeof(radix_node*) * children);
debugf("padding: %ld", padding(children));
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
  i = linkLowWalk(tree, key, len, &cur, &plink, &j);

debugf("linkLowWalk(i:j): %d:%d", i, j);
  /**
   * 1.
   *  alligator             l -> igator -> []
   *              -> al ->
   *  alien                 i -> en -> []        
   * 2.
   *  alligator          a  ->   lligator -> []   
   *              ->        
   *  pilipili           p  ->  ilipili  -> []
   * 3.
   *  abc                c  -> []
   *       ->     ab -> 
   *  abe                e -> []
   **/ 
  if(cur->is_compressed && i != len) {
    radix_node** p2next = radixFirstChild(cur),
      *next;
    memcpy(&next, p2next, sizeof(void*)); //save next

    bool isBegin = (i == 0);
    //创建cur代替节点
    radix_node* new_cur = radixNewNode(isBegin ? 2 : 1, cur->is_key, isBegin ? false : true);
    if(!new_cur) {
debugf("%s", strerror(errno));
      return -1;        
    }
    if(cur->is_key) {
      void* data = radixGetData(cur);
      void** new_cur_data = radixGetVal(new_cur);
      memcpy(new_cur_data, &data, sizeof(void*));
debugf("new_cur_data: %s", (char*)*new_cur_data);
    }
    memcpy(plink, &new_cur, sizeof(void*)); //代替cur

    //创建cur后缀节点  
    int suffix_len = cur->size - 1 - j;
    radix_node* new_cur_suffix = radixNewNode(suffix_len, false, suffix_len > 1);
    if(!new_cur_suffix) {
debugf("%s", strerror(errno));
      return -1;      
    }
    memcpy(&new_cur_suffix->data, cur->data + j + 1, suffix_len);
debugf("new_cur_suffix: %s", (char*)new_cur_suffix->data);
    memcpy(radixGetVal(new_cur_suffix), p2next, sizeof(void*));
debugf("new_cur_suffix->key: %p", *radixFirstChild(new_cur_suffix));

    //创建插入节点
    int insert_dif_len = len - i - 1;
    radix_node* insert_node = radixNewNode(insert_dif_len,false, insert_dif_len > 1);
    if(!insert_node) {
debugf("%s", strerror(errno));
      return -1;
    }
    memcpy(insert_node->data, key + i + 1, insert_dif_len);
debugf("insert_node->data: %s", (char*)&insert_node->data);  
    //创建插入节点后继的key值节点
    radix_node* insert_key = radixNewNode(0, true, false);
    if(!insert_key) {
debugf("%s", strerror(errno));
      return -1;
    }
    void** insert_key_val = radixGetVal(insert_key);
    memcpy(insert_key_val, &val, sizeof(void *));
debugf("insert_key_val: %s", (char*)radixGetData(insert_key));
    radix_node** insert_pos = radixFirstChild(insert_node);
    memcpy(insert_pos, &insert_key, sizeof(radix_node*)); //放入父节点insert_node
debugf("insert_node->key_ptr: %p", *radixFirstChild(insert_node));

    //创建middleware节点
    radix_node* midwar;
    if(i != 0) {
      memcpy(new_cur->data, cur->data, i );
      midwar = radixNewNode(2, false, false);
      if(!midwar) {
  debugf("%s", strerror(errno));
        return -1;              
      }
      midwar->data[0] = cur->data[j];
      midwar->data[1] = key[i];
      memcpy(radixNthChild(midwar, 0), &new_cur_suffix, sizeof(void*));
      memcpy(radixNthChild(midwar, 1), &insert_node, sizeof(void*));
      memcpy(radixFirstChild(new_cur), &midwar, sizeof(void*));
    } else {
      new_cur->data[0] = cur->data[j];
      new_cur->data[1] = key[i];
      memcpy(radixNthChild(new_cur, 0), &new_cur_suffix, sizeof(void*));
      memcpy(radixNthChild(new_cur, 1), &insert_node, sizeof(void*));
    }
    free(cur);
    return 1;
  } 
  /**
   * 
   **/ 
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
int linkLowWalk(radix_tree* tree, const char* key,int len,
  radix_node** cur,radix_node*** plink, int* j)
{
  int i = 0, lj = 0;
  radix_node *lcur = tree->root,
    **lplink = &tree->root; 
  while(lcur->size && i < len) {
debugf("lcur->data: %.*s", lcur->size, lcur->data);
    if(lcur->is_compressed) {
      for(;lj < lcur->size && i < len; lj++) {
        if(lcur->data[lj] == key[i]) {
          i++;
        } else 
          break;
      }
      if(i == len || lcur->size != lj) break;
    } else {
      for(; lj < lcur->size && i < len; ++lj) {
        if(lcur->data[lj] == key[i])
          break;
      }
    }
    radix_node** node = lcur->is_compressed ? 
      radixFirstChild(lcur) :
      radixNthChild(lcur, lj);
    lplink = node;
    memcpy(&lcur, node, sizeof(void*));
    lj = 0;
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
debugf("%ld:%p", radixLength(node), *valPtr);
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
 *  @param: had 处理key值的函数
 **/ 
void traversalDebug(radix_tree* tree, handle had) {
#define RAX_LEN 100
  fprintf(stderr, "**debug radix_tree**\n");
  if(!tree) return;
  simpleQueue* q = simpleQueueFactory(RAX_LEN);
  radix_node* tar = NULL;
  push(q, tree->root);
  int num = 1, ch_num = 0, count = 1;
  fprintf(stderr, "level_%d:\t", count);
  while(size(q)) {
    tar = (radix_node*)front(q);
    pop(q);
    --num;
    fprintf(stderr, "%.*s\t", tar->size, tar->data);
    if(tar->is_key && had) {
      fprintf(stderr,"is_key: 1\t");
      had(radixGetData(tar));
    }
    if(tar->is_compressed) {
      fprintf(stderr,"is_compressed: 1\t");
      push(q, *radixFirstChild(tar));
      ++ch_num;
    }
    else{
      for(int i = 0; i < tar->size; i++){
        push(q, *radixNthChild(tar, i));
        ++ch_num;
      } 
    }
    if(!num && ch_num) {
      fprintf(stderr, "\nlevel_%d:\t", ++count);
      num = ch_num;
      ch_num = 0;
    }
  }
  fprintf(stderr, "\n**End Debug**\n");
  free(q);
}