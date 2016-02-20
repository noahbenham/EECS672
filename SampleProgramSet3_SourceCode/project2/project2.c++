// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "Floor.h"
#include "Table.h"
#include "Umbrella.h"
#include "Glass.h"
#include "Chair.h"

void set3DViewingInformation(double xyz[6]) {
	ModelView::setMCRegionOfInterest(xyz);

	cryph::AffPoint eye(6.0, 4.5, 10.0);
	cryph::AffVector up = cryph::AffVector::yu;

	cryph::AffPoint center(5.0, 1.25, 1.25);
	double distEyeCenter = eye.distanceTo(center);
	ModelView::setEyeCenterUp(eye, center, up);


	double ecZpp = -15.2;
	double ecZmin = -(distEyeCenter + 10.0);
	double ecZmax = -0.01;


	ModelView::setProjection(PERSPECTIVE);
	ModelView::setECZminZmax(ecZmin, ecZmax);
	ModelView::setProjectionPlaneZ(ecZpp);
}

int main(int argc, char* argv[]) {
	GLFWController c("Backyard Picnic", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	// Could combine these into one class as complexity increases in future iterations
	c.addModel(new Floor(-3.0, 0.0, -5.0, 16.0, 0.0, 10.0));
	c.addModel(new Table(0.0, 0.0, 0.5, 10.0, 1.0, 2.0));
	c.addModel(new Chair(0.5, 0.0, 0.0, 1.0, 1.0, 1.0));
	c.addModel(new Chair(2.5, 0.0, 0.0, 1.0, 1.0, 1.0));
	c.addModel(new Chair(4.5, 0.0, 0.0, 1.0, 1.0, 1.0));
	c.addModel(new Chair(6.5, 0.0, 0.0, 1.0, 1.0, 1.0));
	c.addModel(new Chair(8.5, 0.0, 0.0, 1.0, 1.0, 1.0));
	c.addModel(new Glass(0.5, 1.5, 2.0, 0.15));
	c.addModel(new Glass(2.5, 1.5, 2.0, 0.15));
	c.addModel(new Glass(4.5, 1.5, 2.0, 0.15));
	c.addModel(new Glass(6.5, 1.5, 2.0, 0.15));
	c.addModel(new Glass(8.5, 1.5, 2.0, 0.15));
	c.addModel(new Umbrella(-1.0, 0.0, 2.5, 2.5, 6.0));

	glClearColor(1.0, 1.0, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);

	set3DViewingInformation(xyz);

	c.run();

	return 0;
}
