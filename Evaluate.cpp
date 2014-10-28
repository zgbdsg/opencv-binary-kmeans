#include "common.h"

float Evaluate(Mat& labels, Mat& gnd)
{
	Mat newlabel = BestMap(gnd, labels);

	//for (int i = 0; i < 20; i++)
	//	cout << newlabel.at<float>(i, 0) << endl;
	//Grouppring(newlabel, gnd);

	int length = gnd.rows;
	int sum = 0;
	for (int i = 0; i < length; i++)
	{
		if (newlabel.at<double>(i, 0) == gnd.at<double>(i, 0))
			sum++;
	}

	cout << "sum  :" << sum << endl;
	float AC = (1.0*sum) / length;
	return AC;
}


void Grouppring(Mat& L1, Mat& L2)
{
	int map[20][20];
	for (int i = 1; i <= 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			map[i][j] = 0;
		}
	}

	for (int i = 0; i < L1.rows; i++)
	{

	}
}