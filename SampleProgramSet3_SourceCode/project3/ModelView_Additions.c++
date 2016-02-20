// Extracted from: ModelView.c++ - an Abstract Base Class for a combined Model and View for OpenGL

#include "ModelView.h"

void ModelView::addToGlobalRotationDegrees(double rx, double ry, double rz) {
	cryph::Matrix4x4 toCenter = cryph::Matrix4x4::translation(cryph::AffVector(4.75, 3.0, -10.0));
	cryph::Matrix4x4 fromCenter = cryph::Matrix4x4::translation(cryph::AffVector(-4.75, -3.0, 10.0));

	cryph::Matrix4x4 rotX = toCenter * cryph::Matrix4x4::xRotationDegrees(-ry * .1) * fromCenter;
	cryph::Matrix4x4 rotY = toCenter * cryph::Matrix4x4::yRotationDegrees(-rx * .1) * fromCenter;
	cryph::Matrix4x4 rotZ = toCenter * cryph::Matrix4x4::zRotationDegrees(rz * .1) * fromCenter;

	dynamic_view = rotX * rotY * rotZ * dynamic_view;
}

void ModelView::addToGlobalZoom(double increment) {
	if (dynamic_zoomScale + increment > 0)
		dynamic_zoomScale += increment;
	else
		std::cout << "Max zoom level reached.\n";
}

void ModelView::getMatrices(cryph::Matrix4x4& mc_ec, cryph::Matrix4x4& ec_lds) {
	cryph::Matrix4x4 M_ECu = cryph::Matrix4x4::lookAt(eye,center,up);
	mc_ec = dynamic_view * M_ECu;

	double xmin = mcRegionOfInterest[0];
	double xmax = mcRegionOfInterest[1];
	double ymin = mcRegionOfInterest[2];
	double ymax = mcRegionOfInterest[3];
	double zmin = mcRegionOfInterest[4];
	double zmax = mcRegionOfInterest[5];

	double xavg = (xmin + xmax) / 2;
	double yavg = (ymin + ymax) / 2;
	double zavg = (zmin + zmax) / 2;

	double radius = sqrt(pow((xmax - xmin), 2) + pow((ymax - ymin), 2) + pow((zmax - zmin), 2))
		* dynamic_zoomScale;

	double ecXmin = xavg - radius;
	double ecXmax = xavg + radius;
	double ecYmin = yavg - radius;
	double ecYmax = yavg + radius;

	double vAR = Controller::getCurrentController()->getViewportAspectRatio();
	ModelView::matchAspectRatio(xmin, ymax, ymin, ymax, vAR);

	if (projType == ORTHOGONAL) {
		ec_lds = cryph::Matrix4x4::orthogonal(ecXmin, ecXmax,
			ecYmin, ecYmax, ecZmin, ecZmax);
	} else if (projType == PERSPECTIVE) {
		ec_lds = cryph::Matrix4x4::perspective(ecZpp, ecXmin,
			ecXmax, ecYmin, ecYmax, ecZmin, ecZmax);
	} else if (projType == OBLIQUE) {
		ec_lds = cryph::Matrix4x4::oblique(ecZpp, ecXmin, ecXmax, ecYmin, ecYmax,
			ecZmin, ecZmax, ModelView::obliqueProjectionDir);
	}
}
