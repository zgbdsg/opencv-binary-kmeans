#include "common.h"

Mat NormalizeFea(Mat& fea, int row)
{
	Mat newfea;
	if (row == 1)
	{
		int nsmp = fea.cols;
		double* powersum = new double[nsmp];


		/*计算每行的平方和*/
		for (int i = 0; i < nsmp; i++)
		{
			powersum[i] = 0;
			for (int j = 0; j < fea.rows; j++)
			{
				powersum[i] = powersum[i] + fea.at<double>(j, i)*fea.at<double>(j, i);
			}

			if (powersum[i] == 0)
				powersum[i] = 0.00000000000001;
		}

		/*单位化*/
		Mat diags = Mat::zeros(nsmp, nsmp, CV_64FC1);

		for (int i = 0; i < nsmp; i++)
		{
			diags.at<double>(i, i) = 1.0 / sqrt(powersum[i]);
		}

		newfea = fea * diags;
		delete[] powersum;
	}
	else
	{
		int nsmp = fea.rows;
		double* powersum = new double[nsmp];

		/*计算每行的平方和*/
		for (int i = 0; i < nsmp; i++)
		{
			powersum[i] = 0;
			for (int j = 0; j < fea.cols; j++)
			{
				powersum[i] = powersum[i] + fea.at<double>(i, j)*fea.at<double>(i, j);
			}

			if (powersum[i] == 0)
				powersum[i] = 0.00000000000001;
		}

		/*单位化*/
		Mat diags = Mat::zeros(nsmp, nsmp, CV_64FC1);

		for (int i = 0; i < nsmp; i++)
		{
			diags.at<double>(i, i) = 1.0 / sqrt(powersum[i]);
		}
		newfea = diags * fea;

		delete[] powersum;
	}

	return newfea;
}

Mat L1NormalizeFea(Mat& fea, int row)
{
	Mat newfea;
	if (row == 1)
	{
		int nsmp = fea.cols;
		double* powersum = new double[nsmp];


		/*计算每行的和*/
		for (int i = 0; i < nsmp; i++)
		{
			powersum[i] = 0;
			for (int j = 0; j < fea.rows; j++)
			{
				powersum[i] = powersum[i] + fea.at<double>(j, i);
			}

			if (powersum[i] == 0)
				powersum[i] = 0.00000000000001;
		}

		/*单位化*/
		Mat diags = Mat::zeros(nsmp, nsmp, CV_64FC1);

		for (int i = 0; i < nsmp; i++)
		{
			diags.at<double>(i, i) = 1.0 / powersum[i];
		}

		newfea = fea * diags;
		delete[] powersum;
	}
	else
	{
		int nsmp = fea.rows;
		double* powersum = new double[nsmp];

		/*计算每行的平方和*/
		for (int i = 0; i < nsmp; i++)
		{
			powersum[i] = 0;
			for (int j = 0; j < fea.cols; j++)
			{
				powersum[i] = powersum[i] + fea.at<double>(i, j);
			}

			if (powersum[i] == 0)
				powersum[i] = 0.00000000000001;
		}

		/*单位化*/
		Mat diags = Mat::zeros(nsmp, nsmp, CV_64FC1);

		for (int i = 0; i < nsmp; i++)
		{
			diags.at<double>(i, i) = 1.0 / powersum[i];
		}
		newfea = diags * fea;

		delete[] powersum;
	}

	return newfea;
}


Mat MaxNormalizeFea(Mat& fea, int row)
{
	Mat newfea;
	if (row == 1)
	{
		int nsmp = fea.cols;
		double* powersum = new double[nsmp];


		/*单位化*/
		Mat diags = Mat::zeros(nsmp, nsmp, CV_64FC1);

		for (int i = 0; i < nsmp; i++)
		{
			diags.at<double>(i, i) = 1.0 / 256;
		}

		newfea = fea * diags;
		delete[] powersum;
	}
	else
	{
		int nsmp = fea.rows;
		double* powersum = new double[nsmp];


		/*单位化*/
		Mat diags = Mat::zeros(nsmp, nsmp, CV_64FC1);

		for (int i = 0; i < nsmp; i++)
		{
			diags.at<double>(i, i) = 1.0 / 256;
		}
		newfea = diags * fea;

		delete[] powersum;
	}

	return newfea;
}