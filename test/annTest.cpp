//
// Created by william on 2022/2/28.
//

#include <ANN/ANN.h>     // basic ANN includes
#include <ANN/ANNperf.h> // performance evaluation

void annTest()
{
    constexpr size_t POINT_NUM = 4;
    constexpr size_t DIM = 2;
    constexpr size_t K = 2;
    ANNcoord coordinate[POINT_NUM * DIM]{
        0, 0,
        0, 1,
        1, 0,
        1, 1
    };
    ANNpointArray ptsArr = annAllocPts(POINT_NUM, DIM);
    for (size_t i = 0; i < POINT_NUM; ++i)
    {
        for (size_t j = 0; j < DIM; ++j)
        {
            ptsArr[i][j] = coordinate[i * DIM + j];
        }
    }
    ANNbd_tree tree(ptsArr, POINT_NUM, DIM);
    ANNpoint queryPt = annAllocPt(2);
    ANNidx idxArr[K];
    ANNdist distArr[K];
    queryPt[0] = 0.2;
    queryPt[1] = 0.1;
    tree.annkSearch(queryPt, K, idxArr, distArr);
    std::cout << "idx:" << std::endl;
    for (size_t i = 0; i < K; i++)
    {
        std::cout << "- ptsArr[" << idxArr[i] << "]:";
        for (size_t j = 0; j < DIM; j++)
        {
            std::cout << " " << ptsArr[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "dist: ";
    for (double i : distArr)
        std::cout << " " << i;
    std::cout << std::endl;

    annDeallocPts(ptsArr);
}