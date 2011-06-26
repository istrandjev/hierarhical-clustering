#ifndef IMAGE_MOVER_H_
#define IMAGE_MOVER_H_
class ImageMover {
public:
	ImageMover();

	void DoMove();
private:
	double basex;
	double basey;
	double basez;
	double xrot;
	double yrot;
	double zrot;
	static const double rotationAngleStep;
	static const double translationStep;
};
#endif  // IMAGE_MOVER_H_

