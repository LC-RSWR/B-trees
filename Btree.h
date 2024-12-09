#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>  // 添加头文件
using namespace std;

#define M 3  // 设置B-Tree的阶数（M值可以修改来调节树的结构）

// B-Tree 节点定义
class BTreeNode {
public:
    vector<int> keys;  // 当前节点的键值
    vector<BTreeNode*> children;  // 子节点指针
    bool leaf;  // 是否为叶子节点

    // 构造函数
    BTreeNode(bool isLeaf) : leaf(isLeaf) {}

    // 判断节点是否满
    bool isFull() const {
        return keys.size() == 2 * M - 1;
    }

    // 打印当前节点内容（用于调试）
    void printNode() {
        for (int key : keys)
            cout << key << " ";
        cout << endl;
    }
};

// B-Tree 类定义
class BTree {
private:
    BTreeNode* root;

public:
    BTree() {
        root = new BTreeNode(true);  // 初始化时创建一个空的根节点
    }

    // 打印树的结构（层级展示）
    void printTree() {
        printTree(root, 0, -1); // 第三个参数是父节点的索引，根节点没有父节点
    }

    void printTree(BTreeNode* node, int level, int parentIndex) {
        if (node == nullptr) return;

        // 打印当前层级的节点及其父子关系
        printNode(node, level, parentIndex);

        // 如果当前节点不是叶子节点，则递归打印子节点
        if (!node->leaf) {
            for (int i = 0; i < node->children.size(); i++) {
                printTree(node->children[i], level + 1, i);  // 递归调用，递增层级
            }
        }
    }

    // 自定义实现 to_string
    string toString(int num) {
        stringstream ss;
        ss << num;
        return ss.str();
    }

    // 打印当前节点并标注父子关系
    void printNode(BTreeNode* node, int level, int parentIndex) {
        // 打印缩进，表示层级
        string indent(level * 4, ' ');

        // 打印父节点信息
        string parentInfo = (parentIndex == -1) ? "Root" : "Parent Index: " + toString(parentIndex);

        // 打印节点的键值和父节点信息
        cout << indent << parentInfo << " -> [ ";
        for (int key : node->keys) {
            cout << key << " ";
        }
        cout << "]" << endl;
    }


    // 中序遍历：访问树中的所有节点
    void inorderTraversal() {
        inorderTraversal(root);
    }

    void inorderTraversal(BTreeNode* node) {
        if (node == nullptr)
            return;

        int i = 0;
        // 先遍历当前节点的所有子树
        while (i < node->keys.size()) {
            if (!node->leaf)  // 如果不是叶子节点，先遍历子节点
                inorderTraversal(node->children[i]);
            cout << node->keys[i] << " ";  // 打印当前节点的key
            i++;
        }
        // 如果是最后一个子树，遍历该子树
        if (!node->leaf)
            inorderTraversal(node->children[i]);
    }

    // 前序遍历
    void preorderTraversal() {
        preorderTraversal(root);
    }

    void preorderTraversal(BTreeNode* node) {
        if (node == nullptr)
            return;

        cout << "(";  // 可选，用于分隔节点输出
        for (int key : node->keys)
            cout << key << " ";
        cout << ") ";

        if (!node->leaf) {
            for (auto child : node->children) {
                preorderTraversal(child);
            }
        }
    }

    // 后序遍历
    void postorderTraversal() {
        postorderTraversal(root);
    }

    void postorderTraversal(BTreeNode* node) {
        if (node == nullptr)
            return;

        if (!node->leaf) {
            for (auto child : node->children) {
                postorderTraversal(child);
            }
        }

        cout << "(";
        for (int key : node->keys)
            cout << key << " ";
        cout << ") ";
    }

    // 封装搜索入口
    BTreeNode* search(int key) {
        return search(root, key);
    }

    // 查找操作：在B-Tree中查找关键字
    BTreeNode* search(BTreeNode* node, int key) {
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i])
            i++;

        if (i < node->keys.size() && key == node->keys[i])
            return node;  // 找到关键字，返回当前节点

        if (node->leaf)
            return nullptr;  // 到达叶子节点未找到关键字

        return search(node->children[i], key);  // 递归查找子树
    }

    // 插入操作：将关键字插入到B-Tree
    void insert(int key) {
        if (root->isFull()) {  // 如果根节点满了，进行分裂
            BTreeNode* newRoot = new BTreeNode(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
        }
        insertNonFull(root, key);
    }

    void insertNonFull(BTreeNode* node, int key) {
        int i = node->keys.size() - 1;

        if (node->leaf) {  // 如果是叶子节点，直接插入
            node->keys.push_back(0);
            while (i >= 0 && node->keys[i] > key) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
        }
        else {  // 非叶子节点
            while (i >= 0 && node->keys[i] > key) i--;
            i++;

            // 如果子节点满，进行分裂
            if (node->children[i]->isFull()) {
                splitChild(node, i);
                if (node->keys[i] < key)
                    i++;
            }
            insertNonFull(node->children[i], key);
        }
    }

    void splitChild(BTreeNode* parent, int i) {
        BTreeNode* fullChild = parent->children[i];
        BTreeNode* newChild = new BTreeNode(fullChild->leaf);

        parent->children.insert(parent->children.begin() + i + 1, newChild);
        parent->keys.insert(parent->keys.begin() + i, fullChild->keys[M - 1]);

        // 将fullChild的后半部分移动到newChild
        newChild->keys.assign(fullChild->keys.begin() + M, fullChild->keys.end());
        fullChild->keys.resize(M - 1);

        if (!fullChild->leaf) {
            newChild->children.assign(fullChild->children.begin() + M, fullChild->children.end());
            fullChild->children.resize(M);
        }
    }

    // 删除操作：删除指定关键字
    void deleteNode(int key) {
        deleteNode(root, key);
    }

    void deleteNode(BTreeNode* node, int key) {
        int idx = findKey(node, key);

        if (idx < node->keys.size() && node->keys[idx] == key) {  // 找到要删除的键
            if (node->leaf) {
                node->keys.erase(node->keys.begin() + idx);
            }
            else {
                deleteNonLeafNode(node, idx);
            }
        }
        else {
            if (node->leaf) return;
            bool flag = (idx == node->keys.size());
            if (node->children[idx]->keys.size() < M) {
                fillChild(node, idx);
            }
            if (flag && idx > node->keys.size()) {
                deleteNode(node->children[idx - 1], key);
            }
            else {
                deleteNode(node->children[idx], key);
            }
        }
    }

    // 查找关键字的索引位置
    int findKey(BTreeNode* node, int key) {
        int idx = 0;
        while (idx < node->keys.size() && node->keys[idx] < key) {
            idx++;
        }
        return idx;
    }


    void merge(BTreeNode* node, int idx) {
        BTreeNode* child = node->children[idx];
        BTreeNode* sibling = node->children[idx + 1];

        // 将父节点的键值下移到合并后的子节点
        child->keys.push_back(node->keys[idx]);

        // 将兄弟节点的键值移动到子节点中
        child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());

        // 如果兄弟节点有子节点，也将兄弟节点的子节点移动到子节点中
        if (!child->leaf) {
            child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());
        }

        // 将父节点中对应的键值删除
        node->keys.erase(node->keys.begin() + idx);

        // 删除父节点中对应的子节点
        node->children.erase(node->children.begin() + idx + 1);

        // 删除兄弟节点（由于已经合并到子节点中，所以可以释放该兄弟节点）
        delete sibling;
    }


    // 删除非叶子节点的元素
    void deleteNonLeafNode(BTreeNode* node, int idx) {
        int key = node->keys[idx];
        if (node->children[idx]->keys.size() >= M) {
            BTreeNode* pred = node->children[idx];
            while (!pred->leaf) {
                pred = pred->children[pred->children.size() - 1];
            }
            int predKey = pred->keys[pred->keys.size() - 1];
            node->keys[idx] = predKey;
            deleteNode(node->children[idx], predKey);
        }
        else if (node->children[idx + 1]->keys.size() >= M) {
            BTreeNode* succ = node->children[idx + 1];
            while (!succ->leaf) {
                succ = succ->children[0];
            }
            int succKey = succ->keys[0];
            node->keys[idx] = succKey;
            deleteNode(node->children[idx + 1], succKey);
        }
        else {
            merge(node, idx);
            deleteNode(node->children[idx], key);
        }
    }

    void borrowFromNext(BTreeNode* node, int idx) {
        BTreeNode* child = node->children[idx];
        BTreeNode* sibling = node->children[idx + 1];

        // 将父节点的键值下移到当前子节点
        child->keys.push_back(node->keys[idx]);

        // 更新父节点的键值
        node->keys[idx] = sibling->keys.front();

        // 删除兄弟节点中的第一个键值
        sibling->keys.erase(sibling->keys.begin());

        // 如果兄弟节点不是叶子节点，从兄弟节点的子节点中借一个到当前子节点
        if (!child->leaf) {
            child->children.push_back(sibling->children.front());

            // 删除兄弟节点中相应的子节点指针
            sibling->children.erase(sibling->children.begin());
        }
    }


    // 填充子节点（借值或合并）
    void fillChild(BTreeNode* node, int idx) {
        if (idx > 0 && node->children[idx - 1]->keys.size() >= M) {
            borrowFromPrev(node, idx);
        }
        else if (idx < node->keys.size() && node->children[idx + 1]->keys.size() >= M) {
            borrowFromNext(node, idx);
        }
        else {
            if (idx < node->keys.size()) {
                merge(node, idx);
            }
            else {
                merge(node, idx - 1);
            }
        }
    }

    // 从前一个兄弟节点借一个元素
    void borrowFromPrev(BTreeNode* node, int idx) {
        BTreeNode* child = node->children[idx];
        BTreeNode* sibling = node->children[idx - 1];

        // 将父节点的键值下移到当前子节点的前面
        child->keys.insert(child->keys.begin(), node->keys[idx - 1]);

        // 更新父节点的键值
        node->keys[idx - 1] = sibling->keys.back();

        // 删除兄弟节点中的最后一个键值
        sibling->keys.pop_back();

        // 如果兄弟节点不是叶子节点，从兄弟节点的子节点中借一个到当前子节点
        if (!child->leaf) {
            // 将兄弟节点的最后一个子节点移动到当前子节点的前面
            child->children.insert(child->children.begin(), sibling->children.back());

            // 删除兄弟节点中的最后一个子节点指针
            sibling->children.pop_back();
        }
    }
};
