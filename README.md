## 二叉搜索树
### 1.0 定义
- 每个节点包含属性left,right和p，分别指向左右子节点和父节点
- 二叉搜索树的性质: x.left<= x <= x.right； 左子树<父节点<右子树
- 基本操作与树的高度有关，即O(lgn)
 
|数据结构 |查找 | 插入,删除 |
|--|--|--|
|数组|O(n)|O(n) |
|有序数组|O(lgn)|O(n)|
|二叉树|O(lgn)|O(lgn)|

```c
struct Node {
    int val;
    struct Node* left;
    struct Node* right;
    Node(int x):
        val(x), left(nullptr), right(nullptr) {}
};

```

```c
//定义二叉树类型
typedef Node TNode;
typedef Node* pNode;

class AVL { // 平衡二叉树
public:
    pNode root;
    AVL() {root = nullptr;};
    ~AVL() {delete root;};
    int empty() {return (root==nullptr);};
    int maxDepth(pNode p); // 该节点的最大深度
    int minDepth(pNode p); // 该节点的最小深度
    int findMax(); // 二叉树最大值
    int findMin(); // 二叉树最小值
    pNode search(int x);
    pNode _search(pNode p, int x);
    void insert(int x);
    pNode findParent(pNode p); // 返回该节点的父节点
    pNode successor(int x); // 返回节点x的后继节点
    void Delete(int x); // 删除值为x的节点
    void levelOrder(); // 层次遍历，用换行表示层数递进
    void preOrder(pNode p); // 前序遍历
    void inOrder(pNode p); // 中序遍历
    void postOrder(pNode p); // 后序遍历
};
```
<br/>

### 2.0 查找二叉树的最大最小值
- 最小值一定在树的最左节点，最大值一定在树的最右节点
- 不停向左(右)子节点搜索直到NULL即可
```c
int AVL::findMax() {
    pNode p = root;
    while (p->right)
        p = p->right;
    return p->val;
}

int AVL::findMin() {
    pNode p = root;
    while (p->left)
        p = p->left;
    return p->val;
}
```
<br/>

### 2.1 返回二叉树的最大、最小深度
- 深度Depth定义：根节点root到叶子节点的距离
- 最大深度：即不断返回左右子树的深度中的较大值+1，递归实现
- 最小深度：即不断返回左右子树的深度中的较小值+1，递归实现
	
	- **特殊情况：根节点只有一个子节点的时候，最小深度不是1，因为根节点不是叶子节点！**
	- 处理方式：只有一个孩子，其最小深度就是该子节点的最小深度+1
```c
int AVL::maxDepth(pNode p){
    if (!p) return 0;
    return max(maxDepth(p->right), maxDepth(p->left)) + 1;
}

int AVL::minDepth(pNode p) {
    // 深度的定义：p到某个叶子节点的距离;
    // 因此当p只有一个孩子时,它的深度并不是1
    if (!p) return 0;
    if (p->right && p->left) // 有两个孩子，返回较小的深度
        return min(minDepth(p->left), minDepth(p->right)) + 1;
    else // 只有一个孩子，返回此孩子的深度
        return minDepth(p->right) + minDepth(p->left) + 1;
}
```
### 2.2 二叉树的查找
- search(x)：查找key为x的节点，返回其指针
- 先和根节点比较，小的话去左子树搜，大的话去右子树搜，等于则返回
```c
pNode AVL::_search(pNode p, int x) {
    if (p->val == x)
        return p;
    if (x > p->val)
        return _search(p->right, x);
    return _search(p->left, x);
}

pNode AVL::search(int x) {
    return _search(root, x);
}
```

<br>

### 2.2 二叉树的后继节点
> x的后继节点(successor node): key值大于x的节点中key最小的一个
> x的前驱节点(prior node): key值小于x的节点中key最大的一个
- 查找x的后继节点，要对x进行讨论：
- x有右子节点，那么后继节点是右子节点的最左子节点
- x没有右子节点，再分两种讨论

	- x自身是左子节点，那么后继节点是它的父节点
	- x自身是右子节点，那么后继节点是根节点，且必须满足x小于根节点，否则x没有后继节点
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019051102103576.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x1aGFvMTk5ODA5MDk=,size_5,color_FFFFFF,t_70)
```c
pNode AVL::successor(int x) {
    // 后继节点，大于x中最下的那个
    pNode p = search(x);
    pNode tmp;
    if (p == root && !p->right) // 根节点且没有右孩子
        return nullptr;
    if (p->right) { // 如果p有右孩子，那么后继节点就是右孩子的最左孩子
        tmp = p->right;
        while (tmp->left)
            tmp = tmp->left;
        return tmp;
    } else { // p没有右孩子
        pNode par = findParent(p);
        if (p == par->left) { // p自身是左孩子
            return par;
        } else { // p自身是右孩子, 返回层数最靠近p的有左孩子de节点
            if (p->val > root->val)
                return nullptr;
            pNode ppar = findParent(par);
            while (ppar) {
                if (par == ppar->left)
                    return ppar;
                par = ppar;
                ppar = findParent(ppar);
            }
            return nullptr;
        }
    }
}
```


<br>

### 2.3 二叉树的插入和删除
- insert(x):插入新节点，同时维持二叉树的性质
  
  - 若树空，直接插入root
  - 若树不空，通过不断和左右孩子比较，往下移动直到找出合适的位置
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190507225047515.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x1aGFvMTk5ODA5MDk=,size_16,color_FFFFFF,t_70)
```c
void AVL::insert(int x) {
    pNode p = new TNode(x);
    if (!root) {
        root = p;
        return;
    }
    pNode tmp = root;
    pNode par;
    while (tmp) {
        par = tmp;
        if (tmp->val < x) {
            tmp = tmp->right;
        } else {
            tmp = tmp->left;
        }
    }
    if (par->val > x) {
        par->left = p;
    } else {
        par->right = p;
    }
}
```
- delete(x): 删除已存在的某个节点x，同时维持二叉树的性质
	
	- 若x是根节点，直接删除
	- 若x只有一个孩子，用该子节点替换x
	- 若x有两个孩子，找到x的**后继节点nex**并替换x
	
	   - 先用nex的key替换x的key，接着删除nex节点即可    
	   - 若nex是它父节点的左孩子，那么把nex的左孩子顶上来
	   - 若nex是它父节点的右孩子，那么把nex的右孩子顶上来

	<br/>
	

```c
void AVL::Delete(int x) {
    pNode p = search(x);
    if (!p) {
        cout << "Not Existed" << endl;
        return;
    }
    // case 1: p 没有孩子节点， 直接删除
    if (!p->left && !p->right) {
        if (p == root)
            root = nullptr;
        else {
            pNode par = findParent(p);
            if (par->right == p)
                par->right = nullptr;
            else
                par->left = nullptr;
        }
    } else if (p->left && !p->right) {
        // case 2: 只有一个左孩子，替换即可
        if (p == root)
            root = p->left;
        else {
            pNode par = findParent(p);
            if (par->right == p)
                par->right = p->left;
            else
                par->left = p->left;
        }
    } else if (p->right && !p->left) {
        // case 3: 只有一个右孩子，替换即可
        if (p == root)
            root = p->right;
        else {
            pNode par = findParent(p);
            if (par->right == p)
                par->right = p->right;
            else
                par->left = p->right;
        }
    } else {
        // case 4: 有两个孩子, 把它的后继节点替换到该位置即可
        pNode suc = successor(p->val);
        int key = suc->val;
        Delete(key);
        p->val = suc->val;
    }
}
```
<br>

### 二叉树的各种遍历
|遍历方式| 遍历顺序 |应用和意义| 
|--|--|--|
| 层次遍历 | 按一层一层遍历 | 符合人的直观感受 |
| 前序遍历  | 根结点 ---> 左子树 ---> 右子树 | **打印目录结构**：如每个文件夹下的文件 |
| 中序遍历 | 左子树---> 根结点 ---> 右子树  | **编译底层**：实现表达式树 |
| 后序遍历 | 左子树 ---> 右子树 ---> 根结点 | **回溯法:** 实则就是采用后序遍历的形式 |
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190511005459953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x1aGFvMTk5ODA5MDk=,size_1,color_FFFFFF,t_70)

```c
层次遍历：1 2 3 4 5 6 7 8
前序遍历：1 2 4 5 7 8 3 6
中序遍历：4 2 7 5 8 1 3 6
后序遍历：4 7 8 5 2 6 3 1
```
**层次遍历**

- 通过队列实现，每搜索一个节点，将它的孩子按先左后右的顺序加入队列；
- 这样保证了  1：层数越低的越先搜索； 2：左子树先于右子树搜索
```c
void AVL::levelOrder(){
    if (empty()) {
        cout << "empty Tree" << endl;
        return;
    }
    queue<pNode> Queue;
    queue<int> Level;
    Queue.push(root); // 记录节点
    Level.push(0); // 记录层数
    pNode p;
    int h;
    int last_h = 0;
    while (!Queue.empty()) {
        p = Queue.front();
        h = Level.front();
        if (last_h < h )
            cout << endl;
        cout << p->val << ' ';
        if (p->left) {
            Queue.push(p->left);
            Level.push(h+1);
        }
        if (p->right) {
            Queue.push(p->right);
            Level.push(h+1);
        }
        Queue.pop();
        Level.pop();
        last_h = h;
    }
    cout << endl;
}
```

**前序遍历**

- 搜索顺序：根结点 ---> 左子树 ---> 右子树
- 算法实现：1.访问根节点 2.前序遍历左节点 3.前序遍历右节点
```c
void AVL::preOrder(pNode p) {
    // 先序遍历：根->左右节点
    if (!p)
        return;
    cout << p->val << ' ';
    preOrder(p->left);
    preOrder(p->right);
}
```

**中序遍历**

- 搜索顺序：左子树 ---> 根结点 ---> 右子树
- 算法实现：1.前序遍历左节点 2.访问根节点 3.后序遍历右节点
```c
void AVL::inOrder(pNode p) {
    // 中序遍历：左->根->右
    if (!p)
        return;
    inOrder(p->left);
    cout << p->val << ' ';
    inOrder(p->right);
}
```

**后序遍历**

- 搜索顺序：左子树 ---> 右子树 ---> 根节点
- 算法实现：1.前序遍历左节点 2.后序遍历右节点 3.访问根节点
```c
void AVL::postOrder(pNode p) {
    // 后序遍历：左右节点->根
    if (!p)
        return;
    postOrder(p->left);
    postOrder(p->right);
    cout << p->val << ' ';
}
```

### 3.0 扩展概念
#### 满二叉树
- 顾名思义：每层铺满的二叉树，n层满二叉树有2^n-1个节点
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190511013218832.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x1aGFvMTk5ODA5MDk=,size_16,color_FFFFFF,t_70)

#### 完全二叉树
- 即倒数第二层以上都铺满，最后一层紧左边铺的二叉树
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190511013303474.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x1aGFvMTk5ODA5MDk=,size_16,color_FFFFFF,t_70)

#### 平衡二叉树（AVL）
递归地满足：左右子树高度差不超过1的树
**为什么需要AVL?**
- 因为当二叉树形似链表时，查找效率从O(lgn)变成O(n)，所以想办法使得树的高度尽量平衡
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190921224122215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x1aGFvMTk5ODA5MDk=,size_16,color_FFFFFF,t_70)
**平衡因子：** 某节点的左右子树高度之差
**二叉树的整个插入过程，分为四种形态：**

