//
// Created by cwb on 2022/3/17.
//

#include "commonMacro.h"
#include "timer.h"

#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>

static Timer timer;

// neighbor offsets in all directions
static const int offsets[4][2] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };

cv::Mat possionBlend(const cv::Mat& src, const cv::Mat& target, const cv::Mat& mask)
{
    cv::Mat kernel = cv::Mat::zeros(1, 3, CV_8S);
    kernel.at<char>(0, 2) = 1;
    kernel.at<char>(0, 1) = -1;

    timer.reset();
    auto width = target.cols;
    auto height = target.rows;
    /// 计算前景区域
    int count = 0;
    std::vector<std::vector<int>> countNumber(width, std::vector<int>(height));
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (mask.at<cv::Vec3b>(i, j)[0] == 255)
            {
                countNumber[i][j] = count++;
            }
        }
    }
    if (count == 0)
    {
        return {};
    }
    /// 求解方程 Ax = B， x: 每个点都在前景区域
    //    Eigen::VectorXd  result[3] = {3, 3,3};
    Eigen::VectorXd b[3] = { Eigen::VectorXd(count), Eigen::VectorXd(count), Eigen::VectorXd(count) };
    Eigen::SparseMatrix<float> A(count, count);
    A.setZero();
    /// search neighbor
    /// 在域上循环一次。系数矩阵 a 对于所有域都是相同的通道，右边是通道相关的。
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            if (mask.at<cv::Vec3b>(i, j)[0] == 255)
            {
                int neighbor = 0;
                auto backgroundColor = target.at<cv::Vec3b>(i, j);
                auto srcColor = src.at<cv::Vec3b>(i, j);

                for (auto& k : b)
                {
                    k(countNumber[i][j]) = 0;
                }

                for (int k = 0; k < 4; ++k)
                {
                    int nx = i + offsets[k][0], ny = j + offsets[k][1];
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height)
                    {
                        neighbor++;
                        backgroundColor = target.at<cv::Vec3b>(nx, ny); //background.pixel(nx+diff_x, ny+diff_y);
                        srcColor = src.at<cv::Vec3b>(nx, ny);
                        if (mask.at<cv::Vec3b>(nx, ny)[0] == 255)
                        {
                            // p, p_neighbor both in interior of omiga
                            // Np * fp - \sigma fq = \sigma Vpq
                            A.insert(countNumber[i][j], countNumber[nx][ny]);
                        }
                        else
                        {
                            for (int n = 0; n < 3; ++n)
                            {
                                b[k](countNumber[i][j]) += backgroundColor[k];
                            }
                        }
                        // mixing gradients, choose max
                        for (int n = 0; n < 3; ++n)
                        {
                        }
                    }
                }
            }
        }
    }
    LOG_INFO("possionBlend:the run time is %f seconds", timer.elapsedSecond());
}
