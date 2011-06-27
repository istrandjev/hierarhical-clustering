#ifndef SHAPE_VIS_H
#define SHAPE_VIS_H
class ShapeType {
public:
	ShapeType(unsigned code);
	void ChangeCurrentColor() const;
	int GetType() const;

private:
	int r, g, b;
	int type;

private:
	static void Initialize();

private:
	static const unsigned size = 8*8*8*8;
	static unsigned remap[size];
	static bool initialized;
};


class Shape {
public:
	Shape(double _x, double _y, double _z, double _r, int type_code);
	void Display() const;

private:
	double x,y,z;
	double r;
	ShapeType type;
};
#endif  // SHAPE_VIS_H
