### 😳radix_tree
 
&emsp;&emsp;一个简单的radix tree（基数树）的实现。学习模仿redis的rax。目标是用于http的路由。

#### ☕️run 
```
cmake .
```

#### 🍔debug
  &emsp;&emsp;traversalDebug函数可用来显示树的详细信息：

```
**debug radix_tree**
level_1:
[ addr: 0x1160460|is_key: 0|is_cpd: 0|data:a|b|c|ptr: 0x1160400|0x1160420|0x1160300|]

level_2:
[ addr: 0x1160400|is_key: 0|is_cpd: 0|data:l|ptr: 0x1160360|]
[ addr: 0x1160420|is_key: 0|is_cpd: 1|data: aloon|ptr:0x1160440|]
[ addr: 0x1160300|is_key: 0|is_cpd: 1|data: hromodynamic|ptr:0x11602e0|]

level_3:
[ addr: 0x1160360|is_key: 0|is_cpd: 0|data:l|i|ptr: 0x11602a0|0x1160320|]
[ addr: 0x1160440|is_key: 1|is_cpd: 0|data: null|ptr: value:val3]
[ addr: 0x11602e0|is_key: 1|is_cpd: 0|data: null|ptr: value:val4]

level_4:
[ addr: 0x11602a0|is_key: 0|is_cpd: 0|data:i|ptr: 0x11603e0|]
[ addr: 0x1160320|is_key: 0|is_cpd: 1|data: en|ptr:0x1160340|]

level_5:
[ addr: 0x11603e0|is_key: 0|is_cpd: 0|data:g|b|ptr: 0x1160380|0x11603a0|]
[ addr: 0x1160340|is_key: 1|is_cpd: 0|data: null|ptr: value:val1]

level_6:
[ addr: 0x1160380|is_key: 0|is_cpd: 1|data: ator|ptr:0x11602c0|]
[ addr: 0x11603a0|is_key: 0|is_cpd: 1|data: bc|ptr:0x11603c0|]

level_7:
[ addr: 0x11602c0|is_key: 1|is_cpd: 1|data: bbn|ptr:0x1160490|value:val]
[ addr: 0x11603c0|is_key: 1|is_cpd: 0|data: null|ptr: value:val2]

level_8:
[ addr: 0x1160490|is_key: 1|is_cpd: 0|data: null|ptr: value:val5]

**End Debug**
```

&emsp;&emsp;debugf函数显示debug信息，注释掉CMakeLists里面的add_definitions("-D RADIX_DEBUG_MSG")可以取消debug信息。
```
debug: /home/ele/radix_tree/radix_tree.c:radixInsert:177:       insert_key_val: val2
debug: /home/ele/radix_tree/radix_tree.c:radixInsert:180:       insert_node->key_ptr: 0x10903c0
debug: /home/ele/radix_tree/radix_tree.c:radixNewNode:80:       padding: 2
debug: /home/ele/radix_tree/radix_tree.c:radixNewNode:92:       init a radix_node(24)
debug: /home/ele/radix_tree/radix_tree.c:linkLowWalk:303:       lcur->data: al
debug: /home/ele/radix_tree/radix_tree.c:radixInsert:109:       linkLowWalk(i:j): 0:0
debug: /home/ele/radix_tree/radix_tree.c:radixNewNode:80:       padding: 2
debug: /home/ele/radix_tree/radix_tree.c:radixNewNode:92:       init a radix_node(24)
debug: /home/ele/radix_tree/radix_tree.c:radixNewNode:80:       padding: 3
debug: /home/ele/radix_tree/radix_tree.c:radixNewNode:92:       init a radix_node(16)
```