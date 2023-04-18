#include "CustomGraphicsScene.h"
#include "ColorStructures.h"
#include "ImageException.h"
#include <qdebug.h>
#include <cmath>
#include <png.h>
#include <qcolordialog.h>
#include <vector>

CustomGraphScene::CustomGraphScene(Image* image, QObject* parent) : imageOnScene(image),
QGraphicsScene(parent), rectPen(Qt::black), rectBrush(Qt::white), noFill(0),
turnMode(false), turnModeIndex(0), outsideMode(false), outsideColor(Qt::white) {}

CustomGraphScene::~CustomGraphScene() = default;

void CustomGraphScene::setImage(Image* imageToSet) {
    imageOnScene = imageToSet;
}

void CustomGraphScene::setRectPenColor(QColor color) {
    rectPen.setColor(color);
}

void CustomGraphScene::setRectPenWidth(int width) {
    rectPen.setWidth(width);
}

void CustomGraphScene::setRectBrushColor(QColor color) {
    rectBrush.setColor(color);
}

void CustomGraphScene::setNoFill(int value) {
    noFill = value;
}

void CustomGraphScene::setTurnMode(bool newState) {
    turnMode = newState;
}

void CustomGraphScene::setTurnModeIndex(int currentIndex) {
    turnModeIndex = currentIndex;
}

void CustomGraphScene::setOutiseMode(bool newState) {
    outsideMode = newState;
}

void CustomGraphScene::mousePressEvent(QGraphicsSceneMouseEvent * event) {
    if (imageOnScene != nullptr) {
        qreal eventX = event->scenePos().x();
        qreal eventY = event->scenePos().y();
        qreal rightBound = this->sceneRect().center().x() + ((double)imageOnScene->getPixmap().width()) / 2;
        qreal leftBound = this->sceneRect().center().x() - ((double)imageOnScene->getPixmap().width()) / 2;
        qreal lowerBound = this->sceneRect().center().y() + ((double)imageOnScene->getPixmap().height()) / 2;
        qreal upperBound = this->sceneRect().center().y() - ((double)imageOnScene->getPixmap().height()) / 2;
        if (eventX <= rightBound && eventX >= leftBound && eventY <= lowerBound && eventY >= upperBound) {
            QGraphicsRectItem* rect = nullptr;
            if (!turnMode && !outsideMode) {
                rect = addRect(
                    event->scenePos().x(),
                    event->scenePos().y(),
                    0,
                    0,
                    rectPen,
                    noFill == 0 ? rectBrush : Qt::NoBrush
                );
            }
            else {
                rect = addRect(
                    event->scenePos().x(),
                    event->scenePos().y(),
                    0,
                    0
                );
            }
            
            figures.push_back(rect);
            startPoint = event->scenePos();
            update();
        }
    }
}

void CustomGraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
    if (imageOnScene != nullptr && !figures.empty()) {
        if (event->buttons() == Qt::LeftButton) {
            qreal eventX = event->scenePos().x();
            qreal eventY = event->scenePos().y();
            qreal rightBound = this->sceneRect().center().x() + ((double)imageOnScene->getPixmap().width()) / 2;
            qreal leftBound = this->sceneRect().center().x() - ((double)imageOnScene->getPixmap().width()) / 2;
            qreal lowerBound = this->sceneRect().center().y() + ((double)imageOnScene->getPixmap().height()) / 2;
            qreal upperBound = this->sceneRect().center().y() - ((double)imageOnScene->getPixmap().height()) / 2;
            qreal penWidth = figures.back()->pen().widthF();
            if (eventX > rightBound - penWidth) {
                if (eventY > lowerBound - penWidth) {
                    figures.back()->setRect(
                        startPoint.x(),
                        startPoint.y(),
                        rightBound - penWidth - startPoint.x(),
                        lowerBound - penWidth - startPoint.y()
                    );
                }
                else if (eventY < upperBound + penWidth) {
                    figures.back()->setRect(
                        startPoint.x(),
                        startPoint.y(),
                        rightBound - penWidth - startPoint.x(),
                        upperBound + penWidth - startPoint.y()
                    );
                }
                else {
                    figures.back()->setRect(
                        startPoint.x(),
                        startPoint.y(),
                        rightBound - penWidth - startPoint.x(),
                        event->scenePos().y() - startPoint.y()
                    );
                }
            } else if (eventX < leftBound + penWidth) {
                if (eventY > lowerBound - penWidth) {
                    figures.back()->setRect(
                        startPoint.x(),
                        startPoint.y(),
                        leftBound + penWidth - startPoint.x(),
                        lowerBound - penWidth - startPoint.y()
                    );
                }
                else if (eventY < upperBound + penWidth) {
                    figures.back()->setRect(
                        startPoint.x(),
                        startPoint.y(),
                        leftBound + penWidth - startPoint.x(),
                        upperBound + penWidth - startPoint.y()
                    );
                }
                else {
                    figures.back()->setRect(
                        startPoint.x(),
                        startPoint.y(),
                        leftBound + penWidth - startPoint.x(),
                        event->scenePos().y() - startPoint.y()
                    );
                }
            } else {
                figures.back()->setRect(
                    startPoint.x(),
                    startPoint.y(),
                    event->scenePos().x() - startPoint.x(),
                    event->scenePos().y() - startPoint.y()
                );
            }
            update();
        }
    }
}

void CustomGraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    if (imageOnScene != nullptr) {
        if (!figures.empty()) {
            if (!turnMode && !outsideMode) {
                qreal x1, y1, x2, y2;
                figures.back()->rect().getCoords(&x1, &y1, &x2, &y2);
                if (x1 > x2) {
                    qreal temp = x1;
                    x1 = x2;
                    x2 = temp;
                }
                if (y1 > y2) {
                    qreal temp = y1;
                    y1 = y2;
                    y2 = temp;
                }
                int int_x1 = (int)round(x1);
                int int_y1 = (int)round(y1);
                int int_x2 = (int)round(x2);
                int int_y2 = (int)round(y2);
                RGBA penColor = {};
                penColor.red = figures.back()->pen().color().red();
                penColor.green = figures.back()->pen().color().green();
                penColor.blue = figures.back()->pen().color().blue();
                penColor.alpha = figures.back()->pen().color().alpha();
                int penWidth = figures.back()->pen().width();
                RGBA brushColor = {};
                if (figures.back()->brush() != Qt::NoBrush) {
                    brushColor.red = figures.back()->brush().color().red();
                    brushColor.green = figures.back()->brush().color().green();
                    brushColor.blue = figures.back()->brush().color().blue();
                    brushColor.alpha = figures.back()->brush().color().alpha();
                }
                for (int y = int_y1; y < int_y2; y++) {
                    png_byte* row = imageOnScene->getStructure()->rowPointers[y];
                    int bytesForPixel = 0;
                    if (imageOnScene->getStructure()->colorType == PNG_COLOR_TYPE_RGB) {
                        bytesForPixel = 3;
                    }
                    else if (imageOnScene->getStructure()->colorType == PNG_COLOR_TYPE_RGBA) {
                        bytesForPixel = 4;
                    }
                    for (int x = int_x1; x < int_x2; x++) {
                        png_byte* pixel = &(row[x * bytesForPixel]);
                        if (y > int_y1 + penWidth && y < int_y2 - penWidth - 1 && x > int_x1 + penWidth && x < int_x2 - penWidth - 1) {
                            if (figures.back()->brush() == Qt::NoBrush) {
                                continue;
                            }
                            pixel[0] = brushColor.red;
                            pixel[1] = brushColor.green;
                            pixel[2] = brushColor.blue;
                            if (bytesForPixel == 4) {
                                pixel[3] = brushColor.alpha;
                            }
                        }
                        else {
                            pixel[0] = penColor.red;
                            pixel[1] = penColor.green;
                            pixel[2] = penColor.blue;
                            if (bytesForPixel == 4) {
                                pixel[3] = penColor.alpha;
                            }
                        }
                    }
                }
                try {
                    imageOnScene->saveChanges();
                }
                catch (ImageException e) {
                    qDebug() << e.what();
                }
                figures.pop_back();
            }
            else if (!outsideMode) {
                qreal x1, y1, x2, y2;
                figures.back()->rect().getCoords(&x1, &y1, &x2, &y2);
                if (x1 > x2) {
                    qreal temp = x1;
                    x1 = x2;
                    x2 = temp;
                }
                if (y1 > y2) {
                    qreal temp = y1;
                    y1 = y2;
                    y2 = temp;
                }
                int pieceWidth = (int)trunc(figures.back()->rect().width()) / 2;
                if (pieceWidth < 0) {
                    pieceWidth *= -1;
                }
                int pieceHeight = (int)trunc(figures.back()->rect().height()) / 2;
                if (pieceHeight < 0) {
                    pieceHeight *= -1;
                }
                int x1_1 = (int)round(x1);
                int y1_1 = (int)round(y1);
                int x1_2 = x1_1 + pieceWidth - 1;
                int y1_2 = y1_1 + pieceHeight - 1;
                int bytesForPixel = 0;
                if (imageOnScene->getStructure()->colorType == PNG_COLOR_TYPE_RGB) {
                    bytesForPixel = 3;
                }
                else if (imageOnScene->getStructure()->colorType == PNG_COLOR_TYPE_RGBA) {
                    bytesForPixel = 4;
                }
                for (int y = y1_1; y <= y1_2; y++) {
                    png_byte* row = imageOnScene->getStructure()->rowPointers[y];
                    for (int x = x1_1; x <= x1_2; x++) {
                        png_byte* currentPixel = &(row[x * bytesForPixel]);
                        png_byte* secondPiecePixel = &(row[(x + pieceWidth) * bytesForPixel]);
                        png_byte* thirdPiecePixel = &(imageOnScene->getStructure()->rowPointers[y + pieceHeight][(x + pieceWidth) * bytesForPixel]);
                        png_byte* fourthPiecePixel = &(imageOnScene->getStructure()->rowPointers[y + pieceHeight][x * bytesForPixel]);
                        if (turnModeIndex == 0) {
                            int tempR1 = secondPiecePixel[0];
                            int tempG1 = secondPiecePixel[1];
                            int tempB1 = secondPiecePixel[2];
                            int tempA1 = 0;
                            if (bytesForPixel == 4) {
                                tempA1 = secondPiecePixel[3];
                            }
                            secondPiecePixel[0] = currentPixel[0];
                            secondPiecePixel[1] = currentPixel[1];
                            secondPiecePixel[2] = currentPixel[2];
                            if (bytesForPixel == 4) {
                                secondPiecePixel[3] = currentPixel[3];
                            }
                            int tempR2 = thirdPiecePixel[0];
                            int tempG2 = thirdPiecePixel[1];
                            int tempB2 = thirdPiecePixel[2];
                            int tempA2 = 0;
                            if (bytesForPixel == 4) {
                                tempA2 = thirdPiecePixel[3];
                            }
                            thirdPiecePixel[0] = tempR1;
                            thirdPiecePixel[1] = tempG1;
                            thirdPiecePixel[2] = tempB1;
                            if (bytesForPixel == 4) {
                                thirdPiecePixel[3] = tempA1;
                            }
                            tempR1 = fourthPiecePixel[0];
                            tempG1 = fourthPiecePixel[1];
                            tempB1 = fourthPiecePixel[2];
                            if (bytesForPixel == 4) {
                                tempA1 = fourthPiecePixel[3];
                            }
                            fourthPiecePixel[0] = tempR2;
                            fourthPiecePixel[1] = tempG2;
                            fourthPiecePixel[2] = tempB2;
                            if (bytesForPixel == 4) {
                                fourthPiecePixel[3] = tempA2;
                            }
                            currentPixel[0] = tempR1;
                            currentPixel[1] = tempG1;
                            currentPixel[2] = tempB1;
                            if (bytesForPixel == 4) {
                                currentPixel[3] = tempA1;
                            }
                        }
                        else {
                            int tempR1 = thirdPiecePixel[0];
                            int tempG1 = thirdPiecePixel[1];
                            int tempB1 = thirdPiecePixel[2];
                            int tempA1 = 0;
                            if (bytesForPixel == 4) {
                                tempA1 = thirdPiecePixel[3];
                            }
                            thirdPiecePixel[0] = currentPixel[0];
                            thirdPiecePixel[1] = currentPixel[1];
                            thirdPiecePixel[2] = currentPixel[2];
                            if (bytesForPixel == 4) {
                                thirdPiecePixel[3] = currentPixel[3];
                            }
                            currentPixel[0] = tempR1;
                            currentPixel[1] = tempG1;
                            currentPixel[2] = tempB1;
                            if (bytesForPixel == 4) {
                                currentPixel[3] = tempA1;
                            }
                            tempR1 = fourthPiecePixel[0];
                            tempG1 = fourthPiecePixel[1];
                            tempB1 = fourthPiecePixel[2];
                            if (bytesForPixel == 4) {
                                tempA1 = fourthPiecePixel[3];
                            }
                            fourthPiecePixel[0] = secondPiecePixel[0];
                            fourthPiecePixel[1] = secondPiecePixel[1];
                            fourthPiecePixel[2] = secondPiecePixel[2];
                            if (bytesForPixel == 4) {
                                fourthPiecePixel[3] = secondPiecePixel[3];
                            }
                            secondPiecePixel[0] = tempR1;
                            secondPiecePixel[1] = tempG1;
                            secondPiecePixel[2] = tempB1;
                            if (bytesForPixel == 4) {
                                secondPiecePixel[3] = tempA1;
                            }
                        }
                    }
                }
                try {
                    imageOnScene->saveChanges();
                }
                catch (ImageException e) {
                    qDebug() << e.what();
                }
                figures.pop_back();
            } else {
            qreal x1, y1, x2, y2;
            figures.back()->rect().getCoords(&x1, &y1, &x2, &y2);
            if (x1 > x2) {
                qreal temp = x1;
                x1 = x2;
                x2 = temp;
            }
            if (y1 > y2) {
                qreal temp = y1;
                y1 = y2;
                y2 = temp;
            }
            int int_x1 = (int)round(x1);
            int int_y1 = (int)round(y1);
            int int_x2 = (int)round(x2);
            int int_y2 = (int)round(y2);
            int bytesForPixel = 0;
            if (imageOnScene->getStructure()->colorType == PNG_COLOR_TYPE_RGB) {
                bytesForPixel = 3;
            }
            else if (imageOnScene->getStructure()->colorType == PNG_COLOR_TYPE_RGBA) {
                bytesForPixel = 4;
            }
            for (int y = 0; y < imageOnScene->getStructure()->height; y++) {
                png_byte* row = imageOnScene->getStructure()->rowPointers[y];
                for (int x = 0; x < imageOnScene->getStructure()->width; x++) {
                    png_byte* currentPixel = &(row[x * bytesForPixel]);
                    if (y < int_y1 || y > int_y2) {
                        currentPixel[0] = outsideColor.red();
                        currentPixel[1] = outsideColor.green();
                        currentPixel[2] = outsideColor.blue();
                        if (bytesForPixel == 4) {
                            currentPixel[3] = outsideColor.alpha();
                        }
                    } else {
                        if (x < int_x1 || x > int_x2) {
                            currentPixel[0] = outsideColor.red();
                            currentPixel[1] = outsideColor.green();
                            currentPixel[2] = outsideColor.blue();
                            if (bytesForPixel == 4) {
                                currentPixel[3] = outsideColor.alpha();
                            }
                        }
                    }
                }
            }
            try {
                imageOnScene->saveChanges();
            }
            catch (ImageException e) {
                qDebug() << e.what();
            }
            figures.pop_back();
            }
        }
        QString pathToReload = imageOnScene->getPath();
        delete imageOnScene;
        imageOnScene = new Image(pathToReload);
        addPixmap(imageOnScene->getPixmap());
        setSceneRect(imageOnScene->getPixmap().rect());
        update();
    }
}

void CustomGraphScene::changeMostFrequentColor() {
    QColor color = QColorDialog::getColor(Qt::white, nullptr, "Choose color");
    int bytesForPixel = 0;
    if (imageOnScene->getStructure()->colorType == PNG_COLOR_TYPE_RGB) {
        bytesForPixel = 3;
    } else if (imageOnScene->getStructure()->colorType == PNG_COLOR_TYPE_RGBA) {
        bytesForPixel = 4;
    }
    std::vector<RGBACount> imageColors;
    for (int y = 0; y < imageOnScene->getStructure()->height; y++) {
        png_byte* row = imageOnScene->getStructure()->rowPointers[y];
        for (int x = 0; x < imageOnScene->getStructure()->width; x++) {
            png_byte* pixel = &(row[x * bytesForPixel]);
            if (imageColors.empty()) {
                RGBACount elem = {};
                elem.red = pixel[0];
                elem.green = pixel[1];
                elem.blue = pixel[2];
                if (bytesForPixel == 4) {
                    elem.alpha = pixel[3];
                }
                elem.count = 1;
                imageColors.push_back(elem);
            } else {
                bool elemFound = false;
                for (RGBACount elem : imageColors) {
                    if (elem.red == pixel[0] && elem.green == pixel[1] && elem.blue == pixel[2]) {
                        if (bytesForPixel == 4) {
                            if (elem.alpha == pixel[3]) {
                                elem.count++;
                                elemFound = true;
                                break;
                            }
                        } else {
                            elem.count++;
                            elemFound = true;
                            break;
                        }
                    }
                }
                if (!elemFound) {
                    RGBACount elem = {};
                    elem.red = pixel[0];
                    elem.green = pixel[1];
                    elem.blue = pixel[2];
                    if (bytesForPixel == 4) {
                        elem.alpha = pixel[3];
                    }
                    elem.count = 1;
                    imageColors.push_back(elem);
                }
            }
        }
    }
    RGBACount* mostFrequent = &imageColors.front();
    for (RGBACount elem : imageColors) {
        if (elem.count > mostFrequent->count) {
            mostFrequent = &elem;
        }
    }
    for (int y = 0; y < imageOnScene->getStructure()->height; y++) {
        png_byte* row = imageOnScene->getStructure()->rowPointers[y];
        for (int x = 0; x < imageOnScene->getStructure()->width; x++) {
            png_byte* pixel = &(row[x * bytesForPixel]);
            if (mostFrequent->red == pixel[0] && mostFrequent->green == pixel[1] && mostFrequent->blue == pixel[2]) {
                if (bytesForPixel == 4) {
                    if (mostFrequent->alpha == pixel[3]) {
                        pixel[0] = color.red();
                        pixel[1] = color.green();
                        pixel[2] = color.blue();
                        pixel[3] = color.alpha();
                    }
                } else {
                    pixel[0] = color.red();
                    pixel[1] = color.green();
                    pixel[2] = color.blue();
                }
            }
        }
    }
    imageOnScene->saveChanges();
    QString pathToReload = imageOnScene->getPath();
    delete imageOnScene;
    imageOnScene = new Image(pathToReload);
    addPixmap(imageOnScene->getPixmap());
    setSceneRect(imageOnScene->getPixmap().rect());
    update();
}

void CustomGraphScene::chooseOutiseColor() {
    outsideColor = QColorDialog::getColor(Qt::white, nullptr, "Choose color");
}
