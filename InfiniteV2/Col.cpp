#include "Col.h"

Col::Col() {
	Color[0] = 255;
	Color[1] = 255;
	Color[2] = 255;
	Color[3] = 255;
}
Col::Col(int r, int g, int b) {
	Color[0] = r;
	Color[1] = g;
	Color[2] = b;
	Color[3] = 255;
}
Col::Col(int r, int g, int b, int a) {
	Color[0] = r;
	Color[1] = g;
	Color[2] = b;
	Color[3] = a;
}

void Col::Reset() {
	Color[0] = 255;
	Color[1] = 255;
	Color[2] = 255;
	Color[3] = 255;
}
