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
	cryph::Matrix4x4 M_ECu;
	M_ECu = cryph::Matrix4x4::lookAt(eye,center,up);
	mc_ec = M_ECu;

	double ecXmin = -eye.z, ecXmax = eye.z;
	double ecYmin = -eye.z, ecYmax = eye.z;

	ec_lds = cryph::Matrix4x4::oblique(ecZpp, ecXmin, ecXmax, ecYmin, ecYmax,
		ecZmin, ecZmax, ModelView::obliqueProjectionDir);

	double xmin = mcRegionOfInterest[0];
	double xmax = mcRegionOfInterest[1];
	double ymin = mcRegionOfInterest[2];
	double ymax = mcRegionOfInterest[3];

	double vAR = Controller::getCurrentController()->getViewportAspectRatio();
	ModelView::matchAspectRatio(xmin, ymax, ymin, ymax, vAR);
}
