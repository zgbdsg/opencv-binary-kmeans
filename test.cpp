#include <atltime.h>
#include <highgui.h>
#include "common.h"

using namespace std;
int old_main()
{
	int c;
	CTime time;
	IplImage *img;
	CvCapture* capture = cvCaptureFromCAM(0);
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("mainWin", 50, 50);

	while (1)
	{
		img = cvQueryFrame(capture);
		cvShowImage("mainWin", img);
		c = cvWaitKey(10);

		//按s 保存 jpg
		if (c == 's'){
			time = CTime::GetCurrentTime();
			CStringA filename(time.Format(CString("%Y%m%d%H%M%S")) + ".jpg");
			cvSaveImage(filename, img);
		}

		//按 ESC 退出
		if (c == 27)
			break;
	}

	cvReleaseImage(&img);
	cvDestroyWindow("mainWin");

	return 0;
}

int testKmeans()
{
	const int MAX_CLUSTERS = 5;
	Scalar colorTab[] =
	{
		Scalar(0, 0, 255),
		Scalar(0, 255, 0),
		Scalar(255, 100, 100),
		Scalar(255, 0, 255),
		Scalar(0, 255, 255)
	};

	Mat img(500, 500, CV_8UC3);
	RNG rng(12345);

	for (;;)
	{
		int k, clusterCount = rng.uniform(2, MAX_CLUSTERS + 1);
		int i, sampleCount = rng.uniform(1, 1001);
		Mat points(sampleCount, 1, CV_32FC2), labels;

		clusterCount = MIN(clusterCount, sampleCount);
		Mat centers(clusterCount, 1, points.type());

		/* generate random sample from multigaussian distribution */
		for (k = 0; k < clusterCount; k++)
		{
			Point center;
			center.x = rng.uniform(0, img.cols);
			center.y = rng.uniform(0, img.rows);
			Mat pointChunk = points.rowRange(k*sampleCount / clusterCount,
				k == clusterCount - 1 ? sampleCount :
				(k + 1)*sampleCount / clusterCount);
			rng.fill(pointChunk, CV_RAND_NORMAL, Scalar(center.x, center.y), Scalar(img.cols*0.05, img.rows*0.05));
		}

		randShuffle(points, 1, &rng);

		kmeans(points, clusterCount, labels,
			TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),
			3, KMEANS_PP_CENTERS, centers);

		img = Scalar::all(0);

		for (i = 0; i < sampleCount; i++)
		{
			int clusterIdx = labels.at<int>(i);
			Point ipt = points.at<Point2f>(i);
			circle(img, ipt, 2, colorTab[clusterIdx], CV_FILLED, CV_AA);
		}

		imshow("clusters", img);

		char key = (char)waitKey();
		if (key == 27 || key == 'q' || key == 'Q') // 'ESC'
			break;
	}

	return 0;
}


int h_main()
{
	//float la[3][3] = { { 250, 400, 350 }, { 400, 600, 350 }, { 200, 400, 250 } };
	float la[3][1] = { { 1 }, { 2 }, { 3 } };
	Mat labels = Mat(3, 1, CV_32FC1);
	labels.at<float>(0, 0) = 1;
	labels.at<float>(1, 0) = 2;
	labels.at<float>(2, 0) = 3;
	Mat readgnd = Mat(5, 1, CV_32FC1);
	readgnd.at<float>(0, 0) = 2;
	readgnd.at<float>(1, 0) = 1;
	readgnd.at<float>(2, 0) = 1;
	readgnd.at<float>(3, 0) = 3;
	readgnd.at<float>(4, 0) = 3;
	reindex(readgnd);
	for (int i = 0; i < readgnd.rows; i++)
	{
		cout << readgnd.at<float>(i, 0) << endl;
	}
	float AC = Evaluate(labels, readgnd);
	cout << "AC is  " << AC << endl;

	int a = 0;
	cin >> a;
	return 0;
}

int e1_main()
{
	Mat labels = Mat(5, 1, CV_32FC1);
	labels.at<float>(0, 0) = 1;
	labels.at<float>(1, 0) = 2;
	labels.at<float>(2, 0) = 3;
	labels.at<float>(3, 0) = 3;
	labels.at<float>(4, 0) = 3;
	Mat readgnd = Mat(5, 1, CV_32FC1);
	readgnd.at<float>(0, 0) = 2;
	readgnd.at<float>(1, 0) = 1;
	readgnd.at<float>(2, 0) = 1;
	readgnd.at<float>(3, 0) = 3;
	readgnd.at<float>(4, 0) = 3;
	reindex(readgnd);
	float AC = Evaluate(labels, readgnd);
	cout << "AC is  " << AC << endl;

	int a = 0;
	cin >> a;
	return 0;
}

int small_test_main()
{
	string filedir = "..\\data";//mat文件的文件目录，此处设置的当前目录
	string filename = "smalltest";//mat文件的文件名

	cout << "Start reading mat file!" << endl;
	Mat readfea = DataRead(filedir, filename, "fea");
	cout << readfea.cols << " " << readfea.rows << endl;

	Mat readgnd = DataRead(filedir, filename, "gnd");
	cout << readgnd.cols << " " << readgnd.rows << endl;

	/*计算 nclass*/
	int nclass = FindNClass(readgnd);
	cout << "nclass is " << nclass << endl;

	Mat fea = NormalizeFea(readfea).t();

	Mat labels;
	//for (int i = 0; i < fea.rows; i++)
	//{
	//	for (int j = 0; j < fea.cols;j++)
	//		cout << fea.at<float>(i,j)<<" , ";
	//	cout << endl;
	//}


	kmeans(fea, nclass, labels,
		TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 1000, pow(10, -20)),
		3, KMEANS_PP_CENTERS);

	/*kmeans(fea, nclass, labels,
	TermCriteria(CV_TERMCRIT_EPS, 50, pow(10,-20)),
	3, KMEANS_PP_CENTERS);*/
	//cout << labels.at<int>(1, 0) << endl;
	//cout << readgnd.at<float>(0, 2) << endl;
	labels.convertTo(labels, CV_32FC1);
	reindex(labels);
	for (int i = 0; i < 10; i++)
	{
		cout << labels.at<float>(i, 0) << "  " << readgnd.at<float>(0, i) << endl;
	}

	Mat gndTranse = readgnd.t();
	float AC = Evaluate(labels, gndTranse);
	cout << "AC is  " << AC << endl;

	int a = 0;
	cin >> a;

	return 0;
}
int oold_main()
{
	clock_t start, stop;
	string filedir = "..\\data";//mat文件的文件目录，此处设置的当前目录
	string filename = "ORL_32x32";//mat文件的文件名
	
	cout << "Start reading mat file!" << endl;
	Mat readfea = DataRead(filedir, filename, "fea");
	cout << readfea.cols << " " << readfea.rows << endl;

	Mat readgnd = DataRead(filedir, filename, "gnd");
	cout << readgnd.cols << " " << readgnd.rows << endl;

	/*计算 nclass*/
	int nclass = FindNClass(readgnd);
	cout << "nclass is " << nclass << endl;
	Mat fea = NormalizeFea(readfea).t();


	/*使用 FLANN 提取属性*/
	Mat newfea;
	int knnsize = fea.rows / 20;
	Mat merge(fea.rows, fea.cols + fea.rows, CV_32FC1);

	flann::Index flann_index(fea, cv::flann::LinearIndexParams(), cvflann::FLANN_DIST_MANHATTAN);

	Mat indices(fea.rows, knnsize, CV_32FC1);
	Mat dists(fea.rows, knnsize, CV_32FC1);
	//Mat searchrow = fea.row(i);
	start = clock();
	flann_index.knnSearch(fea, indices, dists, knnsize, cv::flann::SearchParams(64));
	stop = clock();
	cout << "FLANN :" << 1.0*(stop - start) / CLOCKS_PER_SEC << "  seconds" << endl;

	indices.convertTo(indices, CV_32FC1);
	dists.convertTo(dists, CV_32FC1);
	newfea = indices.clone();

	//for (int a = 0; a < newfea.rows; a++)
	//{
	//	for (int b = 0; b < merge.cols; b++)
	//	{
	//		if (b<newfea.cols)
	//			merge.at<float>(a, b) = fea.at<float>(a, b);
	//		else
	//		{
	//			assert(b - newfea.cols >= 0);
	//			merge.at<float>(a, b) = newfea.at<float>(a, b - newfea.cols);
	//		}

	//	}
	//}

	Mat linefea(fea.rows, fea.cols, CV_32FC1);
	linefea = Scalar::all(0);

	for (int i = 0; i < newfea.rows; i++)
	{
		for (int j = 0; j < newfea.cols; j++)
		{
			int t = newfea.at<float>(i, j);
			//assert(t >= 0 && t< 11554);
			//if (t >= 0 && t< newfea.cols)
			//linefea.at<float>(i, t) = exp(-pow(dists.at<float>(i, j), 2));
			linefea.at<float>(i, t) = 1;
		}
	}

	//for (int i = 0; i < linefea.rows; i++)
	//{
	//	for (int j = 0; j < knnsize; j++)
	//	{
	//		int t = newfea.at<float>(i, j);

	//		for (int k = 0; k < fea.cols; k++)
	//		{
	//			linefea.at<float>(i, k) = linefea.at<float>(i, k) + fea.at<float>(t, k);
	//		}

	//	}

	//	for (int k= 0; k < fea.cols; k++)
	//	{
	//		linefea.at<float>(i, k) = linefea.at<float>(i, k) / knnsize;
	//	}
	//}

	DataSave(linefea,"..\\data\\new",filename,"knnfea");
	//for (int a = 0; a < merge.rows; a++)
	//{
	//	for (int b = 0; b < merge.cols; b++)
	//	{
	//		if (b<fea.cols)
	//			merge.at<float>(a, b) = fea.at<float>(a, b);
	//		else
	//		{
	//			assert(b - newfea.cols >= 0);
	//			merge.at<float>(a, b) = linefea.at<float>(a, b - fea.cols)/sqrt(1.0*knnsize);
	//		}

	//	}
	//}

	/*使用 kmeans*/
	int round = 10;
	Mat labels;


	for (int k = 0; k < round; k++){
		

		start = clock();
		kmeans(fea, nclass, labels,
			TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 1000, pow(10, -20)),
			3, KMEANS_RANDOM_CENTERS);
		stop = clock();
		//cout << "K-means :" << 1.0*(stop - start) / CLOCKS_PER_SEC << "  seconds" << endl;
		cout<< 1.0*(stop - start) / CLOCKS_PER_SEC <<",";
		labels.convertTo(labels, CV_32FC1);
		reindex(labels);

		Mat gndTranse = readgnd.t();
		float AC = Evaluate(labels, gndTranse);
		//cout << "kmeans AC is  " << AC << endl;
		cout <<AC << ",";

		//merge = NormalizeFea(merge);
		//cout << merge.at<float>(100, 1024) << endl;
		//newfea = NormalizeFea(newfea);

		Mat newlabels;
		start = clock();
		kmeans(linefea, nclass, newlabels,
			TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 1000, pow(10, -20)),
			3, KMEANS_RANDOM_CENTERS);
		stop = clock();
		//cout << "FLANN-kneams :" << 1.0*(stop - start) / CLOCKS_PER_SEC << "  seconds" << endl;
		cout << 1.0*(stop - start) / CLOCKS_PER_SEC <<",";
		newlabels.convertTo(newlabels, CV_32FC1);
		reindex(newlabels);

		gndTranse = readgnd.t();
		AC = Evaluate(labels, gndTranse);

		//cout << "FLANN  AC is  " << AC << endl;
		cout << AC << endl;
		//cout << endl;
	}
	int a = 0;
	cin >> a;
	return 0;
}

int hahamain(){
	string filedir = "..\\data";//mat文件的文件目录，此处设置的当前目录
	string filename = "ORL_32x32";//mat文件的文件名

	cout << "Start reading mat file!" << endl;
	Mat readfea = DataRead(filedir, filename, "fea");
	cout << readfea.cols << " " << readfea.rows << endl;

	Mat readgnd = DataRead(filedir, filename, "gnd");
	cout << readgnd.cols << " " << readgnd.rows << endl;
	int nClass = FindNClass(readgnd);
	cout << "Nclass:" << nClass << endl;

	Mat lables = Mat::zeros(readfea.cols, 1, CV_64FC1);

	Mat fea = NormalizeFea(readfea).t();

	float sum = 0;
	for (int i = 0; i < 10; i++){
		hartigan(fea, nClass, lables, 10);
		//cout << lables<<endl;
		reindex(lables);
		//cout << lables << endl;

		Mat gndTranse = readgnd.t();
		float AC = Evaluate(lables, gndTranse);
		//cout << "FLANN  AC is  " << AC << endl;
		cout << AC << endl;
		sum += AC;
	}

	cout << sum / 10 << endl;
	
	//Mat tmp1 = Mat::ones(10, 10, CV_32FC1);

	//Mat tmp2 = tmp1 * 2;
	//Mat tmp3 = Mat::ones(3, 10, CV_32FC1);

	////Mat tmp4 = Mat::zeros(10, 3, CV_32FC1);
	//Mat tmp4 = Matdistance(tmp2, tmp3);
	//cout << tmp4 << endl;

	//Mat index = Mat::zeros(tmp4.rows, 1, CV_32FC1);
	//Mat dist = Mat::zeros(tmp4.rows, 1, CV_32FC1);

	//MatMin(tmp4, index, dist);

	//printMat(index);
	//cout << "--------------" << endl;
	//printMat(dist);

	//hartigan(tmp2, 3, tmp2, 1);
	
	int a = 0;
	cin >> a;

	return 0;
}

int main(){
	clock_t start, stop;
	string filedir = "..\\data";//mat文件的文件目录，此处设置的当前目录
	string filename = "ORL_32x32";//mat文件的文件名

	cout << "Start reading mat file!" << endl;
	Mat readfea = DataRead(filedir, filename, "fea");
	cout << readfea.cols << " " << readfea.rows << endl;

	Mat readgnd = DataRead(filedir, filename, "gnd");
	cout << readgnd.cols << " " << readgnd.rows << endl;

	/*计算 nclass*/
	int nclass = FindNClass(readgnd);
	cout << "nclass is " << nclass << endl;
	Mat fea = NormalizeFea(readfea).t();


	/*使用 FLANN 提取属性*/
	Mat newfea;
	int knnsize = fea.rows / 20;

	fea.convertTo(fea, CV_32FC1);
	flann::Index flann_index(fea, cv::flann::LinearIndexParams(), cvflann::FLANN_DIST_MANHATTAN);

	Mat indices(fea.rows, knnsize, CV_32FC1);
	Mat dists(fea.rows, knnsize, CV_32FC1);
	//Mat searchrow = fea.row(i);
	start = clock();
	flann_index.knnSearch(fea, indices, dists, knnsize, cv::flann::SearchParams(64));
	stop = clock();
	cout << "FLANN :" << 1.0*(stop - start) / CLOCKS_PER_SEC << "  seconds" << endl;

	flann_index.save("index");
	indices.convertTo(indices, CV_64FC1);
	//cout << indices << endl;
	dists.convertTo(dists, CV_64FC1);
	newfea = indices.clone();

	Mat linefea(fea.rows, fea.cols, CV_32FC1);
	linefea = Scalar::all(0);

	for (int i = 0; i < newfea.rows; i++)
	{
		for (int j = 0; j < newfea.cols; j++)
		{
			int t = newfea.at<float>(i, j);
			//assert(t >= 0 && t< 11554);
			//if (t >= 0 && t< newfea.cols)
			//linefea.at<float>(i, t) = exp(-pow(dists.at<float>(i, j), 2));
			linefea.at<float>(i, t) = 1;
		}
	}

	Mat result = saveDataAsBinary(linefea);
	cout << result << endl;

	int a = 0;
	cin >> a;

	return 0;
}