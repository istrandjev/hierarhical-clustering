#include "image_mover.h"

#include "utils.h"

const double ImageMover::rotationAngleStep = 2.5;
const double ImageMover::translationStep = 0.03;

ImageMover::ImageMover() {
	basex = 0;
	basey = 0;
	basez = -1.5;
	xrot = 0.0;
	yrot = 0.0;
	zrot = 0.0;
}

void ImageMover::DoMove() {
	//glLoadIdentity();
	if (GlVisualizer::keys[VK_LEFT]) {
		basex += translationStep;
	}
	if (GlVisualizer::keys[VK_RIGHT]){
		basex -= translationStep;
	}
	if (GlVisualizer::keys[VK_UP]) {
		basey -= translationStep;
	}
	if (GlVisualizer::keys[VK_DOWN]){
		basey += translationStep;
	}
	if (GlVisualizer::keys['Q']){
		basez += translationStep;
	}
	if (GlVisualizer::keys['A']){
		basez -= translationStep;
	}
	if (GlVisualizer::keys['W']){
		xrot += rotationAngleStep;
	}
	if (GlVisualizer::keys['S']){
		xrot -= rotationAngleStep;
	}
	if (GlVisualizer::keys['E']){
		yrot += rotationAngleStep;
	}
	if (GlVisualizer::keys['D']){
		yrot -= rotationAngleStep;
	}
	if (GlVisualizer::keys['R']){
		zrot += rotationAngleStep;
	}
	if (GlVisualizer::keys['F']){
		zrot -= rotationAngleStep;
	}
	if (GlVisualizer::rotated) {
		GlVisualizer::rotated = false;
		if (GlVisualizer::xrot > 0){
			yrot += rotationAngleStep;
		}
		if (GlVisualizer::xrot < 0){
			yrot -= rotationAngleStep;
		}
		if (GlVisualizer::yrot > 0){
			xrot += rotationAngleStep;
		}
		if (GlVisualizer::yrot < 0){
			xrot -= rotationAngleStep;
		}
		//glRotatef((GLfloat)rotationAngleStep, (GLfloat)GlVisualizer::xrot, (GLfloat)GlVisualizer::yrot, 0.0f);
	}

	glTranslatef((GLfloat)basex, (GLfloat)basey, (GLfloat)basez);
	glRotatef((GLfloat)xrot, 1.0f, 0.0f, 0.0f);
	glRotatef((GLfloat)yrot, 0.0f, 1.0f, 0.0f);
	glRotatef((GLfloat)zrot, 0.0f, 0.0f, 1.0f);
}

