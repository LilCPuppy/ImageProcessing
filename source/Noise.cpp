#include "Noise.h"

shared_ptr<Mat> saltPepperNoise(shared_ptr<Mat> im, double percent, int low, int high)
{
	shared_ptr<Mat> saltedIm = make_shared<Mat>(im->clone());

	for(int row = 0; row < im->rows; row++) {
		for(int col = 0; col < im->cols; col++) {
			if(coinFlip(percent)) {
				if(coinFlip(.5)) {
					saltedIm->at<Vec3b>(row, col)[0] = high;
					saltedIm->at<Vec3b>(row, col)[1] = high;
					saltedIm->at<Vec3b>(row, col)[2] = high;
				} else {
					saltedIm->at<Vec3b>(row, col)[0] = low;
					saltedIm->at<Vec3b>(row, col)[1] = low;
					saltedIm->at<Vec3b>(row, col)[2] = low;

				}
			}
		}
	}
	return saltedIm;
}

