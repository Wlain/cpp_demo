//
// Created by william on 2021/5/23.
//
/**
 * 题目描述:
 * 给定N个整数的序列{A1， A2, ..., An},求Ai,到Aj连续的一段子列和，如何这个和是负数的话，返回0.
 * 输入输出样例：
 * Input: [1, -2, 3, 5, -2, 6, -1]
 * Output: 12
 */

/// 解法1：暴力解法(O(N^3))
int maxSubSeqSum1(int a[], int N)
{
    int thisSum = 0, maxSum = 0;
    for (int i = 0; i < N; ++i) // i是子列左端位置
    {
        for (int j = 0; j < N; ++j) // j是子列右端位置
        {
            for (int k = i; k < j; ++k)
            {
                thisSum += a[k];
            }
            if (thisSum > maxSum)
            {
                maxSum = thisSum;
            }
        }
    }
    return maxSum;
}

/// 解法2：：暴力解法(O(N^2))
int maxSubSeqSum2(int a[], int N)
{
    int thisSum = 0, maxSum = 0;
    for (int i = 0; i < N; ++i) // i是子列左端位置
    {
        thisSum = 0;                /* ThisSum是从A[i]到A[j]的子列和 */
        for (int j = 0; j < N; ++j) // j是子列右端位置
        {
            thisSum += a[j];      /*对于相同的i，不同的j，只要在j-1次循环的基础上累加1项即可*/
            if (thisSum > maxSum) /* 如果刚得到的这个子列和更大 */
            {
                maxSum = thisSum;
            }
        }
    }
    return maxSum;
}

/// 解法3：：分而治之(O(NLogN))
int Max3(int A, int B, int C)
{ /* 返回3个整数中的最大值 */
    return A > B ? A > C ? A : C : B > C ? B :
                                           C;
}

int divideAndConquer(int List[], int left, int right)
{                                            /* 分治法求List[left]到List[right]的最大子列和 */
    int MaxLeftSum, MaxRightSum;             /* 存放左右子问题的解 */
    int MaxLeftBorderSum, maxRightBorderSum; /*存放跨分界线的结果*/

    int LeftBorderSum, rightBorderSum;
    int center, i;

    if (left == right)
    { /* 递归的终止条件，子列只有1个数字 */
        if (List[left] > 0)
            return List[left];
        else
            return 0;
    }

    /* 下面是"分"的过程 */
    center = (left + right) / 2; /* 找到中分点 */
    /* 递归求得两边子列的最大和 */
    MaxLeftSum = divideAndConquer(List, left, center);
    MaxRightSum = divideAndConquer(List, center + 1, right);

    /* 下面求跨分界线的最大子列和 */
    MaxLeftBorderSum = 0;
    LeftBorderSum = 0;
    for (i = center; i >= left; i--)
    { /* 从中线向左扫描 */
        LeftBorderSum += List[i];
        if (LeftBorderSum > MaxLeftBorderSum)
            MaxLeftBorderSum = LeftBorderSum;
    } /* 左边扫描结束 */

    maxRightBorderSum = 0;
    rightBorderSum = 0;
    for (i = center + 1; i <= right; i++)
    { /* 从中线向右扫描 */
        rightBorderSum += List[i];
        if (rightBorderSum > maxRightBorderSum)
            maxRightBorderSum = rightBorderSum;
    } /* 右边扫描结束 */

    /* 下面返回"治"的结果 */
    return Max3(MaxLeftSum, MaxRightSum, MaxLeftBorderSum + maxRightBorderSum);
}

int MaxSubseqSum3(int List[], int N)
{ /* 保持与前2种算法相同的函数接口 */
    return divideAndConquer(List, 0, N - 1);
}

/// 解法4：在线处理(O(N))
int maxSubseqSum4(int a[], int N)
{
    int thisSum, MaxSum;
    int i;
    thisSum = MaxSum = 0;
    for (i = 0; i < N; i++)
    {
        thisSum += a[i]; /* 向右累加 */
        if (thisSum > MaxSum)
            MaxSum = thisSum; /* 发现更大和则更新当前结果 */
        else if (thisSum < 0) /* 如果当前子列和为负 */
            thisSum = 0;      /* 则不可能使后面的部分和增大，抛弃之 */
    }
    return MaxSum;
}
