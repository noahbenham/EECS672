// project3.c++: EECS 672 Project 3 main file

#include "Controller_Additions.h"
#include "Floor.h"
#include "Table.h"
#include "Umbrella.h"
#include "Glass.h"
#include "Napkin.h"
#include "Chair.h"
#include "Bowl.h"
#include "DogToy.h"

void set3DViewingInformation(double xyz[6]) {
	ModelView::setMCRegionOfInterest(xyz);

	cryph::AffPoint eye(0.0, 4.5, 10.0);
	cryph::AffVector up = cryph::AffVector::yu;

	cryph::AffPoint center(2.0, 1.25, 1.25);
	double distEyeCenter = eye.distanceTo(center);
	ModelView::setEyeCenterUp(eye, center, up);


	double ecZpp = -25.2;
	double ecZmin = -(distEyeCenter + 10.0);
	double ecZmax = -0.01;


	ModelView::setProjection(PERSPECTIVE);
	ModelView::setECZminZmax(ecZmin, ecZmax);
	ModelView::setProjectionPlaneZ(ecZpp);
}

int main(int argc, char* argv[]) {
	Controller_Additions c("Backyard Picnic", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	c.addModel(new Floor(-3.0, 0.0, -5.0, 16.0, 0.3, 10.0));
	c.addModel(new Table(0.0, 0.0, 0.5, 10.0, 1.0, 2.0));
	c.addModel(new Chair(0.5, 0.0, 0.0, 1.0, 1.0, 1.0));
	c.addModel(new Chair(2.5, 0.0, 0.0, 1.0, 1.0, 1.0));
	c.addModel(new Chair(4.5, 0.0, 0.0, 1.0, 1.0, 1.0));
	c.addModel(new Chair(6.5, 0.0, 0.0, 1.0, 1.0, 1.0));
	c.addModel(new Chair(8.5, 0.0, 0.0, 1.0, 1.0, 1.0));
	c.addModel(new Glass(0.5, 1.5, 2.0, 0.12));
	c.addModel(new Glass(2.5, 1.5, 2.0, 0.12));
	c.addModel(new Glass(4.5, 1.5, 2.0, 0.12));
	c.addModel(new Glass(6.5, 1.5, 2.0, 0.12));
	c.addModel(new Glass(8.5, 1.5, 2.0, 0.12));
	c.addModel(new Napkin(0.4, 1.6, 0.9, 1.0, 1.0, 1.0));
	c.addModel(new Napkin(2.4, 1.6, 0.9, 1.0, 1.0, 1.0));
	c.addModel(new Napkin(4.4, 1.6, 0.9, 1.0, 1.0, 1.0));
	c.addModel(new Napkin(6.4, 1.6, 0.9, 1.0, 1.0, 1.0));
	c.addModel(new Napkin(8.4, 1.6, 0.9, 1.0, 1.0, 1.0));
	c.addModel(new Bowl(1.0, 1.6, 1.2, 0.4));
	c.addModel(new Bowl(3.0, 1.6, 1.2, 0.4));
	c.addModel(new Bowl(5.0, 1.6, 1.2, 0.4));
	c.addModel(new Bowl(7.0, 1.6, 1.2, 0.4));
	c.addModel(new Bowl(9.0, 1.6, 1.2, 0.4));
	c.addModel(new Umbrella(-1.0, 0.0, 2.5, 2.5, 6.0));
	c.addModel(new DogToy(1.0, 0.0, 3.0, 0.5, 0.1));

	// Light grey color
	glClearColor(0.8, 0.8, 0.8, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);
	c.run();

	return 0;
}
