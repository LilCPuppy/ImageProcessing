#include "Segmentation.h"


shared_ptr<Mat> regionBuildingComp(shared_ptr<Mat> im, shared_ptr<Mat> seg,
	vector<vector<int>> seeds, double delta, int comp)
{
	vector<int> seed;
	int row, col;
	int side = 2;
	int lowerX, upperX, lowerY, upperY;
	int curPixel, adjPixel;
	vector<vector<int>> nextIndices;

	for(int i = 0; i < seeds.size(); i++) {
		seed = seeds[i];
		lowerX = max(0, seed[0]-side);
		upperX = min(im->rows, seed[0]+side+1);
		lowerY = max(0, seed[1]-side);
		upperY = min(im->cols, seed[1]+side+1);

		for(row = lowerX; row < upperX; row++) {
			for(col = lowerY; col < upperY; col++) {

				if(seg->at<Vec3b>(row, col)[2-comp]) {
					continue;
				}

				curPixel = im->at<Vec3b>(seed[0], seed[1])[2-comp];
				adjPixel = im->at<Vec3b>(row, col)[2-comp];

				if(abs(curPixel - adjPixel) < delta) {
					seg->at<Vec3b>(row, col)[2-comp] = 255;
					nextIndices.push_back({row, col});
				}
			}
		}
	}

	if(nextIndices.size() == 0) {
		return seg;
	}

	return regionBuildingComp(im, seg, nextIndices, delta, comp);
}