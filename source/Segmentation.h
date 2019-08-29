#pragma once

#include <cstdlib>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <random>
#include <memory>

#define RED 0
#define GREEN 1
#define BLUE 2


using namespace cv;
using namespace std;

shared_ptr<Mat> regionBuildingComp(shared_ptr<Mat> im, shared_ptr<Mat> seg,
	vector<vector<int>> seeds, double delta, int comp);