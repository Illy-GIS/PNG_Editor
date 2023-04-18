#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DialogUtil.h"
#include "ImageException.h"
#include <QFileDialog>
#include <qdebug.h>
#include <qradiobutton.h>
#include <qcolordialog.h>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), scene(new CustomGraphScene()) {
    this->setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    updateUI();
    connect(ui->chooseImage, &QAction::triggered, this, &MainWindow::chooseTheImage);
    connect(ui->drawRectButton, &QRadioButton::clicked, this, &MainWindow::updateUI);
    connect(ui->turnAreaButton, &QRadioButton::clicked, this, &MainWindow::updateUI);
    connect(ui->changeColorButton, &QRadioButton::clicked, this, &MainWindow::updateUI);
    connect(ui->penColorButton, &QPushButton::clicked, this, &MainWindow::choosePenColor);
    connect(ui->penThicknessBox, SIGNAL(currentIndexChanged(int)), this, SLOT(choosePenThickness()));
    connect(ui->brushColorButton, &QPushButton::clicked, this, &MainWindow::chooseBrushColor);
    connect(ui->noBrushCheckBox, &QCheckBox::clicked, this, &MainWindow::setNoFill);
    connect(ui->frequentColorButton, &QPushButton::clicked, scene, &CustomGraphScene::changeMostFrequentColor);
    connect(ui->turnOptionsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setTurnModeIndex()));
    connect(ui->paintOutsideButton, &QRadioButton::clicked, this, &MainWindow::updateUI);
    connect(ui->outsideChooseColorButton, &QPushButton::clicked, scene, &CustomGraphScene::chooseOutiseColor);
}

MainWindow::~MainWindow() {
    delete scene;
}

void MainWindow::chooseTheImage() {
    QString imgPath = QFileDialog::getOpenFileName(
        this,
        tr("Choose an image"),
        tr(R"(C:\)"),
        tr("Image (*.png)")
    );
    if (imgPath.isNull()) {
        DialogUtil::showError("Invalid path!");
        return;
    }
    Image* oldImage = image;
    try {
        image = new Image(imgPath);
        /*qDebug() << oldImage << tr("\n") << image << tr("\n");*/
    } catch (ImageException e) {
        if (oldImage != nullptr) {
            image = oldImage;
        } else {
            image = nullptr;
        }
        return;
    }
    delete oldImage;
    scene->clear();
    scene->setImage(image);
    scene->addPixmap(image->getPixmap());
    scene->setSceneRect(image->getPixmap().rect());
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    updateUI();
    update();
}

void MainWindow::choosePenColor() {
    QColor color = QColorDialog::getColor(Qt::black, this, tr("Choose color"));
    scene->setRectPenColor(color);
}

void MainWindow::choosePenThickness() {
    scene->setRectPenWidth(ui->penThicknessBox->currentIndex() + 1);
}

void MainWindow::chooseBrushColor() {
    QColor color = QColorDialog::getColor(Qt::white, this, tr("Choose color"));
    scene->setRectBrushColor(color);
}

void MainWindow::setNoFill() {
    scene->setNoFill(ui->noBrushCheckBox->isChecked() ? 1 : 0);
}

void MainWindow::setTurnModeIndex() {
    scene->setTurnModeIndex(ui->turnOptionsBox->currentIndex());
}

void MainWindow::updateUI() {
    if (image == nullptr) {
        ui->penColorButton->setEnabled(false);
        ui->penThicknessBox->setEnabled(false);
        ui->brushColorButton->setEnabled(false);
        ui->noBrushCheckBox->setEnabled(false);
        ui->turnOptionsBox->setEnabled(false);
        ui->frequentColorButton->setEnabled(false);
        ui->outsideChooseColorButton->setEnabled(false);
    } else {
        if (ui->drawRectButton->isChecked()) {
            ui->penColorButton->setEnabled(true);
            ui->penThicknessBox->setEnabled(true);
            ui->brushColorButton->setEnabled(true);
            ui->noBrushCheckBox->setEnabled(true);
            ui->turnOptionsBox->setEnabled(false);
            ui->frequentColorButton->setEnabled(false);
            ui->outsideChooseColorButton->setEnabled(false);
            scene->setTurnMode(false);
            scene->setOutiseMode(false);
        }
        else if (ui->turnAreaButton->isChecked()) {
            ui->penColorButton->setEnabled(false);
            ui->penThicknessBox->setEnabled(false);
            ui->brushColorButton->setEnabled(false);
            ui->noBrushCheckBox->setEnabled(false);
            ui->turnOptionsBox->setEnabled(true);
            ui->frequentColorButton->setEnabled(false);
            ui->outsideChooseColorButton->setEnabled(false);
            scene->setTurnMode(true);
            scene->setOutiseMode(false);
        }
        else if (ui->changeColorButton->isChecked()) {
            ui->penColorButton->setEnabled(false);
            ui->penThicknessBox->setEnabled(false);
            ui->brushColorButton->setEnabled(false);
            ui->noBrushCheckBox->setEnabled(false);
            ui->turnOptionsBox->setEnabled(false);
            ui->frequentColorButton->setEnabled(true);
            ui->outsideChooseColorButton->setEnabled(false);
            scene->setTurnMode(false);
            scene->setOutiseMode(false);
        }
        else if (ui->paintOutsideButton->isChecked()) {
            ui->penColorButton->setEnabled(false);
            ui->penThicknessBox->setEnabled(false);
            ui->brushColorButton->setEnabled(false);
            ui->noBrushCheckBox->setEnabled(false);
            ui->turnOptionsBox->setEnabled(false);
            ui->frequentColorButton->setEnabled(false);
            ui->outsideChooseColorButton->setEnabled(true);
            scene->setTurnMode(false);
            scene->setOutiseMode(true);
        }
    }
}
