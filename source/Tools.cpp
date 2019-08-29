#include "Tools.h"

void showImage(shared_ptr<Mat> im, string figName)
{
	namedWindow(figName, WINDOW_AUTOSIZE);
	imshow(figName, *im);
}

void vPrint(vector<double> vec)
{
	int m = vec.size();
	printf("(");
	for(int i = 0; i<m; i++)
	{
		if(i == m - 1) {
			printf("%f", vec[i]);
			break;
		}

		printf("%f, ", vec[i]);
	}
	printf(")\n");
}



void vPrint(vector<int> vec)
{
	int m = vec.size();
	printf("(");
	for(int i = 0; i<m; i++)
	{
		if(i == m - 1) {
			printf("%d", vec[i]);
			break;
		}
		printf("%d, ", vec[i]);
	}
	printf(")\n");
}


bool coinFlip(double percent)
{
	double threshold = 100 * percent;
	int random = rand();
	
	if((random % 100) + 1 < threshold) {
		return true;
	}

	return false;
}