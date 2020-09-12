//
// Created by william on 2020/9/2.
//

/**
 * 使用std::unique_ptr管理具备专属所有权的资源,unique_ptr一定是首选，unique_ptr不允许复制（因为如果赋值了一个unique_ptr，就会涉及到两个指向同一资源的unique_ptr），unique_ptr只能被移动，不允许被复制。
 */

#include <iostream>



