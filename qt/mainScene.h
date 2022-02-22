//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_MAINSCENE_H
#define CPP_DEMO_MAINSCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <vector>

class MainScene : public QGraphicsScene
{
public:
    MainScene();
    ~MainScene() override;

    void press(QMouseEvent* event);
    void move(QMouseEvent* event);
    void release(QMouseEvent* event);

    void open();
    void save() const;
    void invert();
    void grayScale();
    void restore();
    void mirrorH();
    void mirrorV();
    void deleteAll();
    void process();

private:
    QPoint m_start, m_end;
    std::vector<QPoint> m_starts, m_ends;
    std::vector<std::unique_ptr<QGraphicsItem>> m_items;
    std::unique_ptr<QGraphicsPixmapItem> m_pixmapItem;
    std::unique_ptr<QGraphicsItemGroup> m_group;
    std::unique_ptr<QImage> m_image;
    std::string m_path;
    bool editable = false;
};

#endif //CPP_DEMO_MAINSCENE_H
