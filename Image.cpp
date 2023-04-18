#include "Image.h"
#include "DialogUtil.h"
#include "ImageException.h"

Image::Image(QString imgPath) {
	FILE* imageFile;
	fopen_s(&imageFile, imgPath.toStdString().c_str(), "rb+");
	if (!imageFile) {
		DialogUtil::showError("Error while opening the file!");
		throw ImageException("Opening file error exception");
	}
	png_byte* header = new png_byte[8];
	fread(header, 1, 8, imageFile);
	if (png_sig_cmp(header, 0, 8)) {
		DialogUtil::showError("The file is not recognized as PNG!");
		throw ImageException("File format error exception");
	}
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr) {
		DialogUtil::showError("Problem while reading image!");
		throw ImageException("PNG create struct error exception");
	}
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr) {
		DialogUtil::showError("Problem while reading image!");
		throw ImageException("PNG create info struct error exception");
	}
	if (setjmp(png_jmpbuf(pngPtr))) {
		DialogUtil::showError("Problem while reading image!");
		throw ImageException("Setjmp error exception");
	}
	structure = new imageStructure_t;
	structure->pngPtr = pngPtr;
	structure->infoPtr = infoPtr;
	png_init_io(structure->pngPtr, imageFile);
	png_set_sig_bytes(structure->pngPtr, 8);
	png_read_info(structure->pngPtr, structure->infoPtr);
	structure->width = png_get_image_width(structure->pngPtr, structure->infoPtr);
	structure->height = png_get_image_height(structure->pngPtr, structure->infoPtr);
	structure->colorType = png_get_color_type(structure->pngPtr, structure->infoPtr);
	structure->bitDepth = png_get_bit_depth(structure->pngPtr, structure->infoPtr);
	/*structure->numberOfPasses = png_set_interlace_handling(structure->pngPtr);*/
	png_read_update_info(structure->pngPtr, structure->infoPtr);
	if (setjmp(png_jmpbuf(pngPtr))) {
		DialogUtil::showError("Problem while reading image!");
		throw ImageException("Setjmp error exception");
	}
	structure->rowPointers = (png_bytep*)malloc(sizeof(png_bytep) * structure->height);
	for (int i = 0; i < structure->height; i++) {
		structure->rowPointers[i] = (png_byte*)malloc(png_get_rowbytes(structure->pngPtr, structure->infoPtr));
	}
	png_read_image(structure->pngPtr, structure->rowPointers);
	fclose(imageFile);
	pixmap.load(imgPath);
	path = imgPath;
}

Image::~Image() {
	for (int i = 0; i < structure->height; i++) {
		free(structure->rowPointers[i]);
	}
	free(structure->rowPointers);
	png_destroy_info_struct(structure->pngPtr, &structure->infoPtr);
	png_destroy_read_struct(&structure->pngPtr, nullptr, nullptr);
	delete structure;
}

QPixmap Image::getPixmap() {
	return this->pixmap;
}

imageStructure_t* Image::getStructure() {
	return this->structure;
}

QString Image::getPath() {
	return this->path;
}

void Image::saveChanges() {
	FILE* imageFile;
	fopen_s(&imageFile, path.toStdString().c_str(), "wb");
	if (!imageFile) {
		DialogUtil::showError("Error while saving the file!");
		throw ImageException("Opening file error exception");
	}
	structure->pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!structure->pngPtr) {
		DialogUtil::showError("Problem while saving image!");
		throw ImageException("PNG create write struct error exception");
	}
	structure->infoPtr = png_create_info_struct(structure->pngPtr);
	if (!structure->infoPtr) {
		DialogUtil::showError("Problem while saving image!");
		throw ImageException("PNG create info write struct error exception");
	}
	if (setjmp(png_jmpbuf(structure->pngPtr))) {
		DialogUtil::showError("Problem while saving image!");
		throw ImageException("Setjmp write error exception");
	}
	png_init_io(structure->pngPtr, imageFile);
	if (setjmp(png_jmpbuf(structure->pngPtr))) {
		DialogUtil::showError("Problem while saving image!");
		throw ImageException("Setjmp second call write error exception");
	}
	png_set_IHDR(structure->pngPtr, structure->infoPtr, structure->width, structure->height,
		structure->bitDepth, structure->colorType, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(structure->pngPtr, structure->infoPtr);
	if (setjmp(png_jmpbuf(structure->pngPtr))) {
		DialogUtil::showError("Problem while saving image!");
		throw ImageException("Setjmp third call write error exception");
	}
	png_write_image(structure->pngPtr, structure->rowPointers);
	if (setjmp(png_jmpbuf(structure->pngPtr))) {
		DialogUtil::showError("Problem while saving image!");
		throw ImageException("Setjmp fourth call write error exception");
	}
	png_write_end(structure->pngPtr, NULL);
	fclose(imageFile);
}
