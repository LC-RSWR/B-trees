#include <iostream>
#include <chrono>


using namespace std::chrono;

#include "Btree.h"

// 主函数
int main() {
    BTree btree;

   
    std::vector<int> testData = { 10, 20, 5, 6, 12, 30, 7, 17, 35, 50, 3, 9, 15, 25, 40, 60, 2, 8, 13, 23 };

    cout << "准备好的数据: " << endl;
    // 输出所有数据
    for (int value : testData) {
        std::cout << value << " ";
    }

    // 记录开始时间
    auto start = high_resolution_clock::now();
    // 插入一些元素
    for (int key : testData) {
        btree.insert(key);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << endl;
    cout << "插入操作耗时: " << duration.count() << " 微秒" << endl;

    cout << endl;
    cout << "打印树结构 "<< endl;
    btree.printTree();

    // 中序遍历
    start = high_resolution_clock::now();
    cout << endl;


    cout << "中序遍历结果: ";
    btree.inorderTraversal();
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << endl;
    cout << "中序遍历操作耗时: " << duration.count() << " 微秒" << endl;
    cout << endl;


    // 前序遍历
    start = high_resolution_clock::now();
    cout << "前序遍历结果: ";
    btree.preorderTraversal();
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << endl;
    cout << "前序遍历操作耗时: " << duration.count() << " 微秒" << endl;
    cout << endl;


    // 后序遍历
    start = high_resolution_clock::now();
    cout << "后序遍历结果: ";
    btree.postorderTraversal();
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << endl;
    cout << "后序遍历操作耗时: " << duration.count() << " 微秒" << endl;
    cout << endl;


    // 查找元素
    int key = 6;
    start = high_resolution_clock::now();
    BTreeNode* result = btree.search(key);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    if (result) {
        cout << "找到关键字 " << key << endl;
    }
    else {
        cout << "没有找到关键字 " << key << endl;
    }
    cout << "查找操作耗时: " << duration.count() << " 微秒" << endl;
    cout << endl;


    // 删除元素
    start = high_resolution_clock::now();
    btree.deleteNode(6);
    cout << "删除元素后的中序遍历结果: ";
    btree.inorderTraversal();
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "删除操作耗时: " << duration.count() << " 微秒" << endl;

    return 0;
}