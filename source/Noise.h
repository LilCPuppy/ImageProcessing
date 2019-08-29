#pragma once

#include <cstdlib>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <random>
#include <memory>
#include "Tools.h"

#define RED 0
#define GREEN 1
#define BLUE 2


using namespace cv;
using namespace std;


shared_ptr<Mat> saltPepperNoise(shared_ptr<Mat> im, double percent = .5,
	int low = 0, int high = 255);

shared_ptr<Mat> BinDenoise(shared_ptr<Mat> image, vector <int> bins, double delta, int windowSize);