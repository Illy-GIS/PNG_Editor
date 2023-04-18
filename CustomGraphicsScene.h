#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QAbstractGraphicsShapeItem>
#include <vector>
#include "Image.h"

class CustomGraphScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit CustomGraphScene(Image* imageReference = nullptr, QObject* parent = nullptr);

    ~CustomGraphScene() override;

    void setImage(Image* imageToSet);

    void setRectPenColor(QColor color);

    void setRectPenWidth(int width);

    void setRectBrushColor(QColor color);

    void setNoFill(int value);

    void setTurnMode(bool newState);

    void setTurnModeIndex(int currentIndex);

    void setOutiseMode(bool newState);

    void changeMostFrequentColor();

    void chooseOutiseColor();

private:
    QPointF startPoint;
    std::vector<QGraphicsRectItem*> figures;
    Image* imageOnScene;
    QPen rectPen;
    QBrush rectBrush;
    int noFill;
    bool turnMode;
    int turnModeIndex;
    bool outsideMode;
    QColor outsideColor;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};
