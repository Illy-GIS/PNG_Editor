#pragma once
#include "CustomGraphicsScene.h"
#include "Image.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

    void updateUI();

    private
slots:

    void chooseTheImage();

    void choosePenColor();

    void choosePenThickness();

    void chooseBrushColor();

    void setNoFill();

    void setTurnModeIndex();

private:
    Ui::MainWindow* ui;
    CustomGraphScene* scene;
    Image* image = nullptr;
};
