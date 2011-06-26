#include "shape_vis.h"
#include "utils.h"
#include <algorithm>

bool ShapeType::initialized = false;
unsigned ShapeType::remap[size];

ShapeType::ShapeType(unsigned code) {
	if (!initialized) {
		Initialize();
	}
	code %= size;
	
	unsigned remapped = remap[code];
	r = (remapped%8)*32;remapped/=8;
	g = (remapped%8)*32;remapped/=8;
	b = (remapped%8)*32;remapped/=8;
	type = remapped;
}

//static 
void ShapeType::Initialize() {
	initialized = true;
	for (int i = 0;i < size; ++i) {
		remap[i] = i;
	}
	std::random_shuffle(remap, remap + size);
}
int ShapeType::GetType() const {
	return type;
}

void ShapeType::ChangeCurrentColor() const {
	glColor3f((GLfloat)r/255.0f, (GLfloat)g/255.0f,(GLfloat)b/255.0f);
}

Shape::Shape(double _x, double _y, double _z, double _r, int type_code) : 
		type(type_code), x(_x), y(_y), z(_z), r(_r){
}
void Shape::Display() const {
	glPushMatrix();
	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
	type.ChangeCurrentColor();
	switch(type.GetType()){
		case 2:	auxSolidCube(r);break;
		case 1: auxSolidTorus(r*0.2, r);break;
		case 0: auxSolidSphere(r);break;
		case 3: auxSolidIcosahedron(r);break;
		case 4: auxSolidDodecahedron(r);break;
		case 5: auxSolidOctahedron(r);break;
		case 6: auxSolidTetrahedron(r);break;
		case 7: auxSolidCone(r*0.66,r);break;
	}
	glPopMatrix();
}
