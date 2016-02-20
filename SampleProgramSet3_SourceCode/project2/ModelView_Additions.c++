// Extracted from: ModelView.c++ - an Abstract Base Class for a combined Model and View for OpenGL

#include "ModelView.h"

void ModelView::addToGlobalRotationDegrees(double rx, double ry, double rz)
{
	std::cout << "For project 3, you must implement ModelView::addToGlobalRotationDegrees in ModelView_Additions.c++\n";
	// TODO: 1. UPDATE dynamic_view
	// TODO: 2. Use dynamic_view in ModelView::getMatrices
}

void ModelView::addToGlobalZoom(double increment)
{
	dynamic_zoomScale += increment;
	// TODO: Project 3: Use dynamic_zoomScale in ModelView::getMatrices
}

void ModelView::getMatrices(cryph::Matrix4x4& mc_ec, cryph::Matrix4x4& ec_lds)
{
	//	 TODO
	//    a) For project 2: mc_ec = M_ECu
	//    b) For project 3: mc_ec = dynamic_view * M_ECu

	cryph::Matrix4x4 M_ECu;
	M_ECu = cryph::Matrix4x4::lookAt(eye,center,up);
	mc_ec = M_ECu;


	double xmin = mcRegionOfInterest[0];
	double xmax = mcRegionOfInterest[1];
	double ymin = mcRegionOfInterest[2];
	double ymax = mcRegionOfInterest[3];
	double zmin = mcRegionOfInterest[4];
	double zmax = mcRegionOfInterest[5];

	double xavg = (xmin + xmax) / 2;
	double yavg = (ymin + ymax) / 2;
	double zavg = (zmin + zmax) / 2;

	double radius = sqrt(pow((xmax - xmin), 2) + pow((ymax - ymin), 2) + pow((zmax - zmin), 2));

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
