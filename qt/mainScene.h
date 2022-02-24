//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_MAINSCENE_H
#define CPP_DEMO_MAINSCENE_H

#include "baseWarping.h"

#include <QGraphicsScene>
#include <QMouseEvent>
#include <vector>

class MainScene : public QGraphicsScene
{
public:
    enum class AlgorithmType
    {
        IDW,
        RBF
    };

public:
    MainScene();
    ~MainScene() override;

    void setAlgorithmType(AlgorithmType type);

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
    void deleteAll();

private:
    void mirror(bool h, bool v);

private:
    QImage m_image;
    QImage m_OriginImage;
    QPoint m_start, m_end;
    std::vector<QPoint> m_starts, m_ends;
    std::vector<std::unique_ptr<QGraphicsItem>> m_items;
    std::unique_ptr<QGraphicsPixmapItem> m_pixmapItem;
    std::unique_ptr<QGraphicsItemGroup> m_group;
    std::vector<BaseWarping> m_WarpingList;
    std::string m_path;
    AlgorithmType m_algorithmType = AlgorithmType::IDW;
};

#endif //CPP_DEMO_MAINSCENE_H
