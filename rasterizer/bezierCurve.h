//
// Created by william on 2021/11/20.
//

#ifndef CPP_DEMO_BEZIERCURVE_H
#define CPP_DEMO_BEZIERCURVE_H
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

/**
 * De Casteljau 算法说明如下:
 * 1. 考虑一个 p0, p1, ... pn 为控制点序列的 Bézier 曲线。首先，将相邻的点连接 起来以形成线段。
 * 2. 用 t : (1 − t) 的比例细分每个线段，并找到该分割点。
 * 3. 得到的分割点作为新的控制点序列，新序列的长度会减少一。
 * 4. 如果序列只包含一个点，则返回该点并终止。否则，使用新的控制点序列并 转到步骤 1。
 * 使用 [0,1] 中的多个不同的 t 来执行上述算法，你就能得到相应的 Bézier 曲线。
 */

// 曲面的理解：曲线的曲线
namespace graphics
{
class BezierCurve
{
public:
    BezierCurve(int width, int height);
    ~BezierCurve();
    BezierCurve(const BezierCurve&) = delete;
    BezierCurve& operator=(const BezierCurve&) = delete;
    /**
     * 3阶贝塞尔曲线，传入四个点
     */
    void nativeBezier();
    /**
     * 该函数使用一个控制点序列和一个浮点数 t 作为输入， 实现 de Casteljau 算法来返回 Bézier 曲线上对应点的坐标。
     */
    cv::Point2f recursiveBezier(const std::vector<cv::Point2f>& controlPoints, float t);
    /**
     * 该函数实现绘制 Bézier 曲线的功能,它使用一个控制点序列和一个 OpenCV::Mat 对象作为输入，没有返回值。
     * 它会使 t 在 0 到 1 的范围内进 行迭代，并在每次迭代中使 t 增加一个微小值。对于每个需要计算的 t，
     * 将调用另一个函数 recursiveBezier,然后该函数将返回在 Bézier 曲线上 t 处的点。最后，将返回的点绘制在 OpenCV::Mat 对象上
     */
    void fillBezier();
    const std::unique_ptr<cv::Mat>& window() const;
    static void mouseHandler(int event, int x, int y, int flags, void* userdata);
    static const std::vector<cv::Point2f>& controlPoints();

private:
    static std::vector<cv::Point2f> m_controlPoints;
    std::unique_ptr<cv::Mat> m_window;
    int m_width = 0;
    int m_height = 0;
};
} // namespace graphics

#endif //CPP_DEMO_BEZIERCURVE_H
