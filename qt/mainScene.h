//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_MAINSCENE_H
#define CPP_DEMO_MAINSCENE_H

#include "imageWarping/baseWarping.h"

#include <QGraphicsScene>
#include <QMouseEvent>
#include <vector>
class IdwWarping;

namespace cv
{
class Mat;
}

class MainScene : public QGraphicsScene
{
public:
    MainScene();
    ~MainScene() override;

public:
    void press(QMouseEvent* event);
    void move(QMouseEvent* event);
    void release(QMouseEvent* event);

    void open();
    void save() const;
    void invert();
    void gray();
    void origin();
    void mirrorH();
    void mirrorV();
    void colorTransform();
    void deleteAll();
    void calcIDW();
    void calcRBF();
    void flushImage(const std::function<cv::Mat(const cv::Mat&)>& func);

private:
    void mirror(bool h, bool v);
    inline int getFrameBufferIndex(int i, int j) const
    {
        assert(i >= 0 && i < m_width && j >= 0 && j < m_height);
//        std::clamp(i, 0, m_width);
//        std::clamp(j, 0, m_height);
        return j * m_width + i;
    }

private:
    QImage m_image;
    QImage m_originImage;
    std::vector<Vector2> m_starts, m_ends;
    std::unique_ptr<QGraphicsPixmapItem> m_pixmapItem;
    std::unique_ptr<QGraphicsItemGroup> m_group;
    std::vector<std::unique_ptr<QGraphicsLineItem>> m_lines;
    std::unique_ptr<BaseWarping> m_warping;
    std::string m_path;
    int32_t m_width = 0;
    int32_t m_height = 0;
    std::vector<bool> m_filled;
    bool m_editable = false;
};

#endif //CPP_DEMO_MAINSCENE_H
