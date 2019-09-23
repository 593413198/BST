#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

struct Node {
    int val;
    struct Node* left;
    struct Node* right;
    Node(int x): // 初始化函数
        val(x), left(nullptr), right(nullptr) {}
};

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

pNode AVL::findParent(pNode p) {
    if (p == root) // 根节点没有父节点
        return nullptr;
    queue<pNode> Q;
    Q.push(root);
    while (!Q.empty()) {
        pNode tmp = Q.front();
        if (tmp->left == p || tmp->right == p)
            return tmp;
        if (tmp->right)
            Q.push(tmp->right);
        if (tmp->left)
            Q.push(tmp->left);
        Q.pop();
    }
    return nullptr;
}

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

void AVL::preOrder(pNode p) {
    // 先序遍历：根->左右节点
    if (!p)
        return;
    cout << p->val << ' ';
    preOrder(p->left);
    preOrder(p->right);
}

void AVL::inOrder(pNode p) {
    // 中序遍历：左->根->右
    if (!p)
        return;
    inOrder(p->left);
    cout << p->val << ' ';
    inOrder(p->right);
}

void AVL::postOrder(pNode p) {
    // 后序遍历：左右节点->根
    if (!p)
        return;
    postOrder(p->left);
    postOrder(p->right);
    cout << p->val << ' ';
}

int main(){
    AVL tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    tree.insert(20);
    tree.insert(4);
    tree.insert(6);
    tree.levelOrder();
    cout << endl;
    tree.preOrder(tree.root);
    cout << endl;
    cout << endl;
    tree.inOrder(tree.root);
    cout << endl;
    cout << endl;
    tree.postOrder(tree.root);
    return 0;

}
