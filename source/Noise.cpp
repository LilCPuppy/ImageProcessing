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


shared_ptr<Mat> BinDenoise(shared_ptr<Mat> image, vector <int> bins, double delta, int windowSize)
{
	// this is attempting to apply a median filter to only the affected pixels, don't think it is currently working
	int rgb, rows, cols;

	shared_ptr<Mat> denoisedIm = make_shared<Mat>(Mat::zeros(image->size(), image->type()));

	for (rgb = 0; rgb < 3; rgb++)
	{
		for (rows = windowSize; rows < image->rows - windowSize; rows++)
		{
			for(cols = windowSize; cols < image->cols - windowSize; cols++)
			{

				int curPix = image->at<Vec3b>(rows, cols)[2 - rgb];
				int newVal;

				if ((curPix = bins[0]) or (curPix = bins[1]))
				{
					int counter = 0;
					int newVal;

					for (int windowRow = rows-windowSize; windowRow < rows + windowSize; windowRow++)
					{
						for (int windowCol = cols-windowSize; windowCol < cols + windowSize; windowCol++) 
						{	
							int neighbor = image->at<Vec3b>(windowRow, windowCol)[2-rgb];
							if ((neighbor == bins[0]) or (neighbor == bins[1]))
							{
								newVal += 0;
							}

							else 
							{
								newVal += image->at<Vec3b>(windowRow, windowCol)[2-rgb];
								counter += 1;
							}
							
						}
					}

					if (counter != 0)
					{
						newVal = (int) (newVal / counter);
					}
					else
					{
						newVal = curPix;
					}

					denoisedIm->at<Vec3b>(rows, cols)[2 - rgb] = newVal;

				}

				else 
				{
					denoisedIm->at<Vec3b>(rows, cols)[2 - rgb] = curPix;
				}

			}
		}
	}

	return denoisedIm;
}
