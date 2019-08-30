#include <cstdlib>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <random>
#include "Tools.h"
#include <tuple>

// using namespace shared_ptr<Mat>;
using namespace cv;
using namespace std;


shared_ptr<Mat> XDifference(shared_ptr<Mat> image, int sign);


shared_ptr<Mat> posXDifference(shared_ptr<Mat> image);


shared_ptr<Mat> minusXDifference(shared_ptr<Mat> image);


shared_ptr<Mat> YDifference(shared_ptr<Mat> image, int sign);


shared_ptr<Mat> posYDifference(shared_ptr<Mat> image);


shared_ptr<Mat> minusYDifference(shared_ptr<Mat> image);


shared_ptr<Mat> midXDifference(shared_ptr<Mat> image);


shared_ptr<Mat> midYDifference(shared_ptr<Mat> image);


shared_ptr<Mat> XOffMinus(shared_ptr<Mat> image);


shared_ptr<Mat> XOffPlus(shared_ptr<Mat> image);


shared_ptr<Mat> YOffMinus(shared_ptr<Mat> image);


shared_ptr<Mat> YOffPlus(shared_ptr<Mat> image);


shared_ptr<Mat> ceFunc(shared_ptr<Mat> image);


vector<shared_ptr<Mat>> firstDerivs(shared_ptr<Mat> image);


shared_ptr<Mat> rofIteration(shared_ptr<Mat> origImage, shared_ptr<Mat> curImage, vector<shared_ptr<Mat>> derivs, double lambda, int iteration);


shared_ptr<Mat> rofBVDecomposition(shared_ptr<Mat> image, shared_ptr<Mat> curImage, int iterations, int curIteration, double lambda);


vector <vector <shared_ptr<Mat>>> MultiScaleDecomposition(shared_ptr<Mat> image, double lambda, int iterations, int curIteration, vector <shared_ptr<Mat>> uIms, vector <shared_ptr<Mat>> vIms);





