#pragma once
#include <qgraphicsscene.h>
#include <png.h>

typedef struct {
	png_structp pngPtr;
	png_infop infoPtr;
	int width;
	int height;
	png_byte colorType;
	png_byte bitDepth;
	/*int numberOfPasses;*/
	png_bytep* rowPointers;
} imageStructure_t;

class Image {
public:
	Image(QString imgPath);

	~Image();

	QPixmap getPixmap();

	imageStructure_t* getStructure();

	QString getPath();

	void saveChanges();

private:
	QString path;
	QPixmap pixmap;
	imageStructure_t* structure;
};
