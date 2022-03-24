//
// Created by cwb on 2022/3/17.
//

#include "base.h"
#include "vec2.h"

#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>

static Timer timer;

// neighbor offsets in all directions
static const int offsets[4][2] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
extern cv::Mat computeLaplace(const cv::Mat& image);
extern cv::Mat computeGradient(const cv::Mat& image);

cv::Mat possionBlend(cv::InputArray _src, cv::InputArray _target, cv::InputArray _mask)
{
    const auto src = _src.getMat();
    auto target = _target.getMat();
    const auto mask = _mask.getMat();
    auto width = target.cols;
    auto height = target.rows;
    timer.reset();
    /// 计算原图的拉普拉斯（散度）
    auto srcLaplace = computeLaplace(src);
    /// 计算背景图的梯度拉普拉斯（散度）
    auto targetLaplace = computeLaplace(target);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (mask.at<cv::Vec3b>(i, j)[0] >= 128)
            {
                targetLaplace.at<cv::Vec3b>(i, j) = { 0, 0, 0 };
            }
        }
    }
    LOG_INFO("possionBlend:the run time is %f seconds-1", timer.elapsedSecond());
    timer.reset();
    cv::Mat div = targetLaplace + srcLaplace;
    /// 计算前景区域, 对mask内的点进行编号
    std::vector<Vec2i> indexToCoord;
    Eigen::MatrixXi coordToIndex(width, height);
    int count = 0;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (mask.at<cv::Vec3b>(i, j)[0] >= 128)
            {
                coordToIndex(j, i) = count++;
                indexToCoord.emplace_back(j, i);
            }
        }
    }
    if (count == 0)
    {
        return {};
    }
    std::cout << "count: " << count << std::endl;
    /// 构建系数阵并分解
    Eigen::SparseMatrix<double> a(count, count);
    std::vector<Eigen::Triplet<double>> coefficients;
    Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> llt;
    for (int i = 0; i < count; ++i)
    {
        auto pos = indexToCoord[i];
        auto x = pos.x;
        auto y = pos.y;
        int np = 4;
        /// 边界条件
        if (x == 0 || width - 1 == x)
        {
            np--;
        }
        if (y == 0 || height - 1 == y)
        {
            np--;
        }
        coefficients.emplace_back(i, i, np);
        if (x > 0 && mask.at<cv::Vec3b>(x - 1, y)[0] >= 128)
        {
            coefficients.emplace_back(i, coordToIndex(x - 1, y), -1);
        }
        if (x < width - 1 && mask.at<cv::Vec3b>(x + 1, y)[0] >= 128)
        {
            coefficients.emplace_back(i, coordToIndex(x + 1, y), -1);
        }
        if (y > 0 && mask.at<cv::Vec3b>(x, y - 1)[0] >= 128)
        {
            coefficients.emplace_back(i, coordToIndex(x, y - 1), -1);
        }
        if (y < height - 1 && mask.at<cv::Vec3b>(x, y + 1)[0] >= 128)
        {
            coefficients.emplace_back(i, coordToIndex(x, y + 1), -1);
        }
        a.setFromTriplets(coefficients.begin(), coefficients.end());
        llt.compute(a);
    }
    LOG_INFO("possionBlend:the run time is %f seconds-2", timer.elapsedSecond());
    timer.reset();
    /// 求解方程 Ax = B， x: 每个点都在前景区域
    Eigen::VectorXd b[3] = { Eigen::VectorXd(count), Eigen::VectorXd(count), Eigen::VectorXd(count) };
    Eigen::VectorXd result[3] = { Eigen::VectorXd(count), Eigen::VectorXd(count), Eigen::VectorXd(count) };
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (mask.at<cv::Vec3b>(i, j)[0] >= 128)
            {
                for (int k = 0; k < 3; ++k)
                {
                    b[k][coordToIndex(j, i)] = div.at<cv::Vec3b>(j, i)[k];
                }
            }
        }
    }
    LOG_INFO("possionBlend:the run time is %f seconds-3", timer.elapsedSecond());
    timer.reset();
    Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> solver;
    solver.compute(a);
    for (int k = 0; k < 3; ++k)
    {
        result[k] = solver.solve(b[k]);
    }
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (mask.at<cv::Vec3b>(i, j)[0] >= 128)
            {
                target.at<cv::Vec3b>(i, j) = { (unsigned char)result[0][coordToIndex(j, i)], (unsigned char)result[0][coordToIndex(j, i)], (unsigned char)result[0][coordToIndex(j, i)] };
            }
        }
    }
    LOG_INFO("possionBlend:the run time is %f seconds-5", timer.elapsedSecond());
    timer.reset();
    return target;
}
