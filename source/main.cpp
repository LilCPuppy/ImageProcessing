#include <iostream>
#include <memory>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Segmentation.h"
#include "Tools.h"
#include "Noise.h"
#include "Multiscale.h"

#define RED 0
#define GREEN 1
#define BLUE 2


using namespace cv;
using namespace std;

int main()
{

	// shared_ptr<Mat> im = make_shared<Mat>(imread("../inImages/Rice.png"));
	// shared_ptr<Mat> seg = make_shared<Mat>(Mat::zeros(im->size(), im->type()));
	// vector<vector<int>> seeds = {{0, 0}, {100, 100}, {200, 200}}

	shared_ptr<Mat> image = make_shared<Mat>(imread("../inImages/barbara.png"));

	printf("image->size {%d, %d}\n", image->rows, image->cols);
	// Mat saltyImage = saltPepperNoise(image, .50, 0, 255);

	vector<shared_ptr<Mat>> uIms, vIms;
	vector <vector<shared_ptr<Mat>>> out = MultiScaleDecomposition(image, 0.001, 12, 1, uIms, vIms);

	showImage(out[0][0], "no way this work");
	// seg = regionBuildingComp(im, seg, seeds, 5.0, RED);

	// showImage(im, "riceyboi");
	// showImage(seg, "riceyBoi");
	waitKey(0);
	
	return 1;
}

