//
// Created by william on 2021/11/20.
//

#include "bezierCurve.h"
namespace graphics
{
std::vector<cv::Point2f> BezierCurve::m_controlPoints;
BezierCurve::BezierCurve(int width, int height) :
    m_width(width), m_height(height)
{
    m_window = std::make_unique<cv::Mat>(width, height, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(*m_window, *m_window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Bezier Curve", mouseHandler, nullptr);
}

BezierCurve::~BezierCurve() = default;

void BezierCurve::mouseHandler(int event, int x, int y, int, void*)
{
    if (event == cv::EVENT_LBUTTONDOWN && m_controlPoints.size() < 10)
    {
        std::cout << "left button of the mouse is clicked - position (" << x << ", "
                  << y << ")" << std::endl;
        m_controlPoints.emplace_back(x, y);
    }
}

void BezierCurve::nativeBezier()
{
    auto& p0 = m_controlPoints[0];
    auto& p1 = m_controlPoints[1];
    auto& p2 = m_controlPoints[2];
    auto& p3 = m_controlPoints[3];
    for (double t = 0.0; t < 1.0f; t += 0.001)
    {
        auto point = std::pow(1 - t, 3) * p0 + 3 * t * std::pow(1 - t, 2) * p1 +
            3 * std::pow(t, 2) * (1 - t) * p2 + std::pow(t, 3) * p3;
        m_window->at<cv::Vec3b>(point.y, point.x) = 255;
    }
}

cv::Point2f BezierCurve::recursiveBezier(const std::vector<cv::Point2f>& controlPoints, float t)
{
    if (controlPoints.size() == 1)
    {
        return controlPoints[0];
    }
    std::vector<cv::Point2f> point;
    for (int i = 0; i < controlPoints.size() - 1; ++i)
    {
        auto& p0 = controlPoints[i];
        auto& p1 = controlPoints[i + 1];
        point.emplace_back((1.0 - t) * p0 + t * p1);
    }
    return recursiveBezier(point, t);
}

void BezierCurve::fillBezier()
{
    for (double t = 0.0; t <= 1.0; t += 0.001)
    {
        auto point = recursiveBezier(m_controlPoints, t);
        m_window->at<cv::Vec3b>(point.y, point.x)[1] = 255;
        //anti-aliasing
        float dx = point.x - std::floor(point.x);
        float dy = point.y - std::floor(point.y);
        int xFlag = dx < 0.5f ? -1 : 1;
        int yFlag = dy < 0.5f ? -1 : 1;
        // 距离采样点最近的4个坐标点
        cv::Point2f p00 = cv::Point2f(std::floor(point.x) + 0.5f, std::floor(point.y) + 0.5f);
        cv::Point2f p01 = cv::Point2f(std::floor(point.x + xFlag * 1.0f) + 0.5f, std::floor(point.y) + 0.5f);
        cv::Point2f p10 = cv::Point2f(std::floor(point.x) + 0.5f, std::floor(point.y + yFlag * 1.0f) + 0.5f);
        cv::Point2f p11 = cv::Point2f(std::floor(point.x + xFlag * 1.0f) + 0.5f, std::floor(point.y + yFlag * 1.0f) + 0.5f);
        std::vector<cv::Point2f> vec;
        vec.push_back(p01);
        vec.push_back(p10);
        vec.push_back(p11);
        // 计算最近的坐标点与采样点距离
        cv::Point2f distance = p00 - point;
        float len = sqrt(distance.x * distance.x + distance.y * distance.y);
        // 对边缘点进行着色
        for (auto& p : vec)
        {
            // 根据距离比, 计算边缘点影响系数
            cv::Point2f d = p - point;
            float l = sqrt(d.x * d.x + d.y * d.y);
            float percent = len / l;
            cv::Vec3d color = m_window->at<cv::Vec3b>(p.y, p.x);
            // 取最大值
            m_window->at<cv::Vec3b>(p.y, p.x)[1] = std::max(color[1], (double)255 * percent);;
        }
    }
}

const std::vector<cv::Point2f>& BezierCurve::controlPoints()
{
    return m_controlPoints;
}

const std::unique_ptr<cv::Mat>& BezierCurve::window() const
{
    return m_window;
}

} // namespace graphics