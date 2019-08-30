#include "Multiscale.h"
#include "Noise.h"


/*
The following will be inplemented to code the derivatives of the image. 
*/

/* Derivative Definitions:
u is a digital image

(D_+-x(u))_{i, j} := +- (u_{i+-1, j}  - u_{i, j})
(D_+-y(u))_{i, j} := +- (u_{i, j+-1} - u_{i, j})

D_0 := (D_+ + D_-) / 2

*/
// shifts image right one column, pads w/ 0's
shared_ptr<Mat> XOffMinus(shared_ptr<Mat> image)
{
	shared_ptr<Mat> xOff = make_shared<Mat>(Mat::zeros(image->size(), image->type()));
	int curPixel, rgb, row, col;

	for (rgb = 0; rgb < 3; rgb++) {
		for(row = 0; row < image->rows; row++) {
			for (col = 0; col < (image->cols-1); col++) { 

				curPixel = image->at<Vec3b>(row, col)[rgb];
				xOff->at<Vec3b>(row, col+1)[rgb] = curPixel;

			}
		}
	}

	return xOff;	
}

// shifts image left one colum, pads w/ 0's
shared_ptr<Mat> XOffPlus(shared_ptr<Mat> image)
{
	shared_ptr<Mat> xOff = make_shared<Mat>(Mat::zeros(image->size(), image->type()));
	int curPixel, nextPixel;

	for (int rgb = 0; rgb < 3; rgb++) {
		for(int row = 0; row < image->rows; row++) {
			for (int col = 1; col < image->cols; col++) { 

				curPixel = image->at<Vec3b>(row, col)[rgb];
				xOff->at<Vec3b>(row, col-1)[rgb] = curPixel;

			}
		}
	}

	return xOff;	
}

// shifts image down one row, pads w/ 0's
shared_ptr<Mat> YOffMinus(shared_ptr<Mat> image)
{
	shared_ptr<Mat> yOff = make_shared<Mat>(Mat::zeros(image->size(), image->type()));
	int curPixel, nextPixel, rgb, row, col;

	for (rgb = 0; rgb < 3; rgb++) {
		for(row = 0; row < image->rows - 1; row++) {
			for (col = 0; col < image->cols; col++) { 

				curPixel = image->at<Vec3b>(row, col)[rgb];
				yOff->at<Vec3b>(row + 1, col)[rgb] = curPixel;

			}
		}
	}

	return yOff;	
}

// shifts image up one row, pads w/ 0's
shared_ptr<Mat> YOffPlus(shared_ptr<Mat> image)
{
	shared_ptr<Mat> yOff = make_shared<Mat>(Mat::zeros(image->size(), image->type()));
	int curPixel, nextPixel, rgb, row, col;

	for (rgb = 0; rgb < 3; rgb++) {
		for(row = 1; row < image->rows; row++) {
			for (col = 0; col < image->cols; col++) { 

				curPixel = image->at<Vec3b>(row, col)[rgb];
				yOff->at<Vec3b>(row - 1, col)[rgb] = curPixel;

			}
		}
	}

	return yOff;	
}

// computes the positive x derivative
shared_ptr<Mat> posXDifference(shared_ptr<Mat> image)
{
	// return XDifference(image, 1);
	return make_shared<Mat>(*XOffPlus(image) - *image);
}

// computes the negative x derivative
shared_ptr<Mat> minusXDifference(shared_ptr<Mat> image)
{
	// return XDifference(image, -1);
	return make_shared<Mat>(*image  - *XOffMinus(image));
}

//computes the average of the two derivatives
shared_ptr<Mat> midXDifference(shared_ptr<Mat> image)
{
	// We loop through this to avoid effects of maxing out at 255, or minimizing at 0
	
	shared_ptr<Mat> midX = make_shared<Mat>(Mat::zeros(image->size(), image->type()));

	shared_ptr<Mat> xPosDiff = posXDifference(image);
	shared_ptr<Mat> xMinusDiff = minusXDifference(image);

	for (int rgb = 0; rgb < 3; rgb++) {
		for (int row = 0; row < image->rows; row++){
			for (int col = 0; col < image->cols; col++){

				int offPlus = xPosDiff->at<Vec3b>(row, col)[2-rgb];
				int offMinus = xMinusDiff->at<Vec3b>(row, col)[2-rgb];
				midX->at<Vec3b>(row, col)[2-rgb] = (int) ((0.5 * offPlus) + (0.5 * offMinus));

			}
		}
	}

	return midX;
}

// computes the positive y derivative
shared_ptr<Mat> posYDifference(shared_ptr<Mat> image)
{
	// return YDifference(image, 1);
	return make_shared<Mat>(*YOffPlus(image) - *image);
}

// computes the negative y derivative
shared_ptr<Mat> minusYDifference(shared_ptr<Mat> image)
{
	// return YDifference(image, -1);
	return make_shared<Mat>(*image - *YOffMinus(image));
}

// computes the average of the two derivatives
shared_ptr<Mat> midYDifference(shared_ptr<Mat> image)
{	

	// We loop through this to avoid effects of maxing out at 255, or minimizing at 0

	shared_ptr<Mat> midY = make_shared<Mat>(Mat::zeros(image->size(), image->type()));

	shared_ptr<Mat> yPosDiff = posYDifference(image);
	shared_ptr<Mat> yMinusDiff = minusYDifference(image);

	int rgb, row, col, offPlus, offMinus;

	for (rgb = 0; rgb < 3; rgb++) {
		for (row = 0; row < image->rows; row++){
			for (col = 0; col < image->cols; col++){

				offPlus = yPosDiff->at<Vec3b>(row, col)[2-rgb];
				offMinus = yMinusDiff->at<Vec3b>(row, col)[2-rgb];
				midY->at<Vec3b>(row, col)[2-rgb] = (int) ((0.5 * offPlus) + (0.5 * offMinus));

			}
		}
	}

	return midY;

}



vector <shared_ptr<Mat>> firstDerivs(shared_ptr<Mat> image)
{
	vector<shared_ptr<Mat>> derivs;

	derivs.push_back(minusXDifference(image)); // x- = [0]
	derivs.push_back(posXDifference(image));  // x+ = [1]
	derivs.push_back(midXDifference(image));  // x+- = [2]
	derivs.push_back(minusYDifference(image)); // y- = [3]
	derivs.push_back(posYDifference(image));  // y+ = [4]
	derivs.push_back(midYDifference(image));  // y+- = [5]

	return derivs;
}



shared_ptr<Mat> rofIteration(shared_ptr<Mat> origImage, shared_ptr<Mat> curImage, vector <shared_ptr<Mat>> derivs, double lambda, int iteration)
{	
	double h = 1.0 / (curImage->rows * curImage->cols);
	// double h = 1.0;
	double epsilon = 0.01;

	shared_ptr<Mat> DxMinus = derivs[0];
	shared_ptr<Mat> DxPlus = derivs[1];
	shared_ptr<Mat> DxMid = derivs[2];
	shared_ptr<Mat> DyMinus = derivs[3];
	shared_ptr<Mat> DyPlus = derivs[4];
	shared_ptr<Mat> DyMid = derivs[5];

	shared_ptr<Mat> xOPresult  = XOffPlus(curImage); 
	shared_ptr<Mat> xOMresult= XOffMinus(curImage);
	shared_ptr<Mat> yOPresult = YOffPlus(curImage);
	shared_ptr<Mat> yOMresult = YOffMinus(curImage);

	vector<shared_ptr<Mat>> xOffMinusDerivs = firstDerivs(xOMresult);
	shared_ptr<Mat> xOffDyMid = xOffMinusDerivs[5];

	vector<shared_ptr<Mat>> yOffMinusDerivs = firstDerivs(yOMresult);
	shared_ptr<Mat> yOffDxMid = yOffMinusDerivs[2];

	shared_ptr<Mat> result = make_shared<Mat>(Mat::zeros(curImage->size(), curImage->type()));

	double dval = (2 * lambda * pow(h, 2));
	double nval = (2 * lambda * pow(h, 2));

	int rgb, row, col;

	if (iteration % 4 == 0)
	{
		for(rgb = 0; rgb < 3; rgb++)
		{
			for(row = 0; row < curImage->rows; row++)
			{
				for(col = 0; col < curImage->cols; col++)
				{
					int DxPlusVal = DxPlus->at<Vec3b>(row, col)[2-rgb];
					int DyMidVal = DyMid->at<Vec3b>(row, col)[2-rgb];
					int DxMinusVal = DxMinus->at<Vec3b>(row, col)[2-rgb];
					int xOffDyMidVal = xOffDyMid->at<Vec3b>(row, col)[2-rgb];
					int DxMidVal = DxMid->at<Vec3b>(row, col)[rgb];
					int DyPlusVal = DyPlus->at<Vec3b>(row, col)[2-rgb];
					int yOffDxMidVal = yOffDxMid->at<Vec3b>(row, col)[2-rgb];
					int DyMinusVal = DyMinus->at<Vec3b>(row, col)[2-rgb];

					double cedenom = sqrt( pow(epsilon, 2) + pow(DxPlusVal, 2) + pow( DyMidVal, 2) );
					double ce = 1.0 / cedenom;

					double cwdenom = sqrt( pow(epsilon, 2) + pow(DxMinusVal, 2) + pow( xOffDyMidVal, 2));
					double cw = 1.0 / cwdenom; 

					double csdenom = sqrt( pow(epsilon, 2) + pow(DxMidVal, 2) + pow(DyPlusVal, 2));
					double cs = 1.0 / csdenom;

					double cndenom = sqrt( pow(epsilon, 2) + pow(yOffDxMidVal, 2) + pow(DyMinusVal, 2));
					double cn = 1.0 / cndenom;

					double denom =  dval + ce + cw + cs + cn;
					double numer = (nval * origImage->at<Vec3b>(row, col)[2-rgb]) + (ce * xOPresult->at<Vec3b>(row, col)[2-rgb]) 
						+ (cw * xOMresult->at<Vec3b>(row, col)[2-rgb]) + (cs * yOPresult->at<Vec3b>(row, col)[2-rgb]) 
						+ (cn * yOMresult->at<Vec3b>(row, col)[2-rgb]);

					int newVal = (int) (numer/denom); 

					result->at<Vec3b>(row, col)[2-rgb] = newVal;


					
				}
			}
		} 
	}


	if (iteration % 4 == 1)
	{
		for(rgb = 0; rgb < 3; rgb++)
		{
			for(row = (curImage->rows - 1); row > -1; row--)
			{
				for(col = 0; col < curImage->cols; col++)
				{
					int DxPlusVal = DxPlus->at<Vec3b>(row, col)[2-rgb];
					int DyMidVal = DyMid->at<Vec3b>(row, col)[2-rgb];
					int DxMinusVal = DxMinus->at<Vec3b>(row, col)[2-rgb];
					int xOffDyMidVal = xOffDyMid->at<Vec3b>(row, col)[2-rgb];
					int DxMidVal = DxMid->at<Vec3b>(row, col)[rgb];
					int DyPlusVal = DyPlus->at<Vec3b>(row, col)[2-rgb];
					int yOffDxMidVal = yOffDxMid->at<Vec3b>(row, col)[2-rgb];
					int DyMinusVal = DyMinus->at<Vec3b>(row, col)[2-rgb];

					double cedenom = sqrt( pow(epsilon, 2) + pow(DxPlusVal, 2) + pow( DyMidVal, 2) );
					double ce = 1.0 / cedenom;

					double cwdenom = sqrt( pow(epsilon, 2) + pow(DxMinusVal, 2) + pow( xOffDyMidVal, 2));
					double cw = 1.0 / cwdenom; 

					double csdenom = sqrt( pow(epsilon, 2) + pow(DxMidVal, 2) + pow(DyPlusVal, 2));
					double cs = 1.0 / csdenom;

					double cndenom = sqrt( pow(epsilon, 2) + pow(yOffDxMidVal, 2) + pow(DyMinusVal, 2));
					double cn = 1.0 / cndenom;

					double denom =  dval + ce + cw + cs + cn;
					double numer = (nval * origImage->at<Vec3b>(row, col)[2-rgb]) + (ce * xOPresult->at<Vec3b>(row, col)[2-rgb]) 
						+ (cw * xOMresult->at<Vec3b>(row, col)[2-rgb]) + (cs * yOPresult->at<Vec3b>(row, col)[2-rgb]) 
						+ (cn * yOMresult->at<Vec3b>(row, col)[2-rgb]);

					int newVal = (int) (numer/denom); 

					result->at<Vec3b>(row, col)[2-rgb] = newVal;			
				}
			}
		} 
	}


	if (iteration % 4 == 2)
	{
		for(rgb = 0; rgb < 3; rgb++)
		{
			for(row = (curImage->rows - 1); row > -1; row--)
			{
				for(col = (curImage->cols - 1); col > -1; col--)
				{
					int DxPlusVal = DxPlus->at<Vec3b>(row, col)[2-rgb];
					int DyMidVal = DyMid->at<Vec3b>(row, col)[2-rgb];
					int DxMinusVal = DxMinus->at<Vec3b>(row, col)[2-rgb];
					int xOffDyMidVal = xOffDyMid->at<Vec3b>(row, col)[2-rgb];
					int DxMidVal = DxMid->at<Vec3b>(row, col)[rgb];
					int DyPlusVal = DyPlus->at<Vec3b>(row, col)[2-rgb];
					int yOffDxMidVal = yOffDxMid->at<Vec3b>(row, col)[2-rgb];
					int DyMinusVal = DyMinus->at<Vec3b>(row, col)[2-rgb];

					double cedenom = sqrt( pow(epsilon, 2) + pow(DxPlusVal, 2) + pow( DyMidVal, 2) );
					double ce = 1.0 / cedenom;

					double cwdenom = sqrt( pow(epsilon, 2) + pow(DxMinusVal, 2) + pow( xOffDyMidVal, 2));
					double cw = 1.0 / cwdenom; 

					double csdenom = sqrt( pow(epsilon, 2) + pow(DxMidVal, 2) + pow(DyPlusVal, 2));
					double cs = 1.0 / csdenom;

					double cndenom = sqrt( pow(epsilon, 2) + pow(yOffDxMidVal, 2) + pow(DyMinusVal, 2));
					double cn = 1.0 / cndenom;

					double denom =  dval + ce + cw + cs + cn;
					double numer = (nval * origImage->at<Vec3b>(row, col)[2-rgb]) + (ce * xOPresult->at<Vec3b>(row, col)[2-rgb]) 
						+ (cw * xOMresult->at<Vec3b>(row, col)[2-rgb]) + (cs * yOPresult->at<Vec3b>(row, col)[2-rgb]) 
						+ (cn * yOMresult->at<Vec3b>(row, col)[2-rgb]);

					int newVal = (int) (numer/denom); 

					result->at<Vec3b>(row, col)[2-rgb] = newVal;		
				}
			}
		} 
	}
	

	if (iteration % 4 == 3)
	{
		for(rgb = 0; rgb < 3; rgb++)
		{
			for(col = (curImage->cols - 1); col > -1; col--)
			{
				for(row = 0; row < curImage->rows; row++)
				{
					int DxPlusVal = DxPlus->at<Vec3b>(row, col)[2-rgb];
					int DyMidVal = DyMid->at<Vec3b>(row, col)[2-rgb];
					int DxMinusVal = DxMinus->at<Vec3b>(row, col)[2-rgb];
					int xOffDyMidVal = xOffDyMid->at<Vec3b>(row, col)[2-rgb];
					int DxMidVal = DxMid->at<Vec3b>(row, col)[rgb];
					int DyPlusVal = DyPlus->at<Vec3b>(row, col)[2-rgb];
					int yOffDxMidVal = yOffDxMid->at<Vec3b>(row, col)[2-rgb];
					int DyMinusVal = DyMinus->at<Vec3b>(row, col)[2-rgb];

					double cedenom = sqrt( pow(epsilon, 2) + pow(DxPlusVal, 2) + pow( DyMidVal, 2) );
					double ce = 1.0 / cedenom;

					double cwdenom = sqrt( pow(epsilon, 2) + pow(DxMinusVal, 2) + pow( xOffDyMidVal, 2));
					double cw = 1.0 / cwdenom; 

					double csdenom = sqrt( pow(epsilon, 2) + pow(DxMidVal, 2) + pow(DyPlusVal, 2));
					double cs = 1.0 / csdenom;

					double cndenom = sqrt( pow(epsilon, 2) + pow(yOffDxMidVal, 2) + pow(DyMinusVal, 2));
					double cn = 1.0 / cndenom;

					double denom =  dval + ce + cw + cs + cn;
					double numer = (nval * origImage->at<Vec3b>(row, col)[2-rgb]) + (ce * xOPresult->at<Vec3b>(row, col)[2-rgb]) 
						+ (cw * xOMresult->at<Vec3b>(row, col)[2-rgb]) + (cs * yOPresult->at<Vec3b>(row, col)[2-rgb]) 
						+ (cn * yOMresult->at<Vec3b>(row, col)[2-rgb]);

					int newVal = (int) (numer/denom); 

					result->at<Vec3b>(row, col)[2-rgb] = newVal;
				}
			}
		} 
	}

	return result;

}

shared_ptr<Mat> rofBVDecomposition(shared_ptr<Mat> image, shared_ptr<Mat> curImage, int iterations, int curIteration, double lambda)
{	
	if (curIteration == iterations) {
		return curImage;
	}

	vector <shared_ptr<Mat>> derivs = firstDerivs(curImage);
	
	shared_ptr<Mat> newIm = rofIteration(image, curImage, derivs, lambda, curIteration);
	printf("Current Iteration: %d\n", curIteration);

	return rofBVDecomposition(image, newIm, iterations, (curIteration + 1), lambda);
}


vector <vector<shared_ptr<Mat>>> MultiScaleDecomposition(shared_ptr<Mat> image, double lambda, int iterations, int curIteration, vector <shared_ptr<Mat>> uIms, vector <shared_ptr<Mat>> vIms)
{
	if (curIteration == iterations)
	{
		vector<vector <shared_ptr<Mat>>> result;
		result.push_back(uIms);
		result.push_back(vIms);
		return result;
	}

	shared_ptr<Mat> newIm = rofBVDecomposition(image, image, 10, 0, lambda);
	shared_ptr<Mat> newOrig = make_shared<Mat>(*image - *newIm);
	uIms.push_back(newIm);
	vIms.push_back(newOrig);
	return MultiScaleDecomposition(newOrig, (2 * lambda), iterations, (curIteration + 1), uIms, vIms);

}
