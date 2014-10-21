#include "common.h"

int _main()
{
	/************************************************************************/
	/*							mat write and read                          */

	string filedir = "..\\data";//mat文件的文件目录，此处设置的当前目录
	string filename = "COIL20";//mat文件的文件名
	string matname = "fea";//mat文件的文件头

	/****************************读文件********************************/

	cout << "Start reading mat file!" << endl;
	Mat readfea = DataRead(filedir, filename, "fea");
	cout << readfea.cols << " " << readfea.rows << endl;

	Mat readgnd = DataRead(filedir, filename, "gnd");
	cout << readgnd.cols << " " << readgnd.rows << endl;

	/************************************************************************/

	/*计算 nclass*/
	int nclass = FindNClass(readgnd);
	cout << "nclass is " << nclass << endl;

	Mat fea = NormalizeFea(readfea).t();
	cout << fea.rows << "  " << fea.cols << endl;
	//cout << fea.at<float>(0, 1) << endl;

	/*************************PCA*************************/
	//int num_component = 64;
	//PCA pca(fea, Mat(), CV_PCA_DATA_AS_ROW, num_component);

	//Mat mean = pca.mean.clone();
	//Mat eigenvalues = pca.eigenvalues.clone();
	//Mat eigenvectors = pca.eigenvectors.clone();

	//for (int i = 0; i < fea.rows; i++)
	//{
	//	fea.row(i) = fea.row(i) - mean;
	//}

	//cout << eigenvectors.rows << "   " << eigenvectors.cols << endl;
	//Mat newfea = fea*eigenvectors.t();
	Mat newfea = fea;
	/*****************************************************/

	/**************************kmeans***************************/

	Mat labels;
	cout << newfea.at<float>(0, 1) << endl;
	kmeans(newfea, nclass, labels,
		TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 100, pow(10, -20)),
		3, KMEANS_RANDOM_CENTERS);

	//cout << labels.at<int>(1, 0) << endl;
	//cout << readgnd.at<float>(0, 2) << endl;
	labels.convertTo(labels, CV_32FC1);
	//cout << labels.at<float>(2, 0) << endl;
	//reindex(labels);
	for (int i = 0; i < 20; i++)
	{
		cout << labels.at<float>(i, 0) << "  " << readgnd.at<float>(0, i) << endl;
	}

	Mat gndTrans = readgnd.t();
	float AC = Evaluate(labels, gndTrans);
	cout << "AC is  " << AC << endl;

	int a = 0;
	cin >> a;
	return 0;
}