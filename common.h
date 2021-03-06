#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <mat.h>
#include <iostream>
#include <vector>
#include <math.h>
#include "adapters.h"
#include "matrix.h"
#include "munkres.h"
#include "hun.h"
#include <opencv2/flann/flann.hpp>
#include <opencv2/flann/flann_base.hpp>
#include <time.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

//#ifndef TEST1H
//#define TEST1H
//	static int** dataMap = new int*[256];
//#endif

int DataSave(Mat& SrcMat, string datapath, string filename, string matname);  /*保存数据到mat*/
Mat DataRead(string datapath, string filename, string matname);		/*从mat中读取数据*/
int FindNClass(Mat& data);     /*矩阵中不同类别数*/
vector<double> Unique(Mat& data);    /*矩阵中的不同类别的向量*/
Mat NormalizeFea(Mat& fea, int row = 1);    /*单位化矩阵*/
Mat L1NormalizeFea(Mat& fea, int row=1);
Mat MaxNormalizeFea(Mat& fea, int row=1);
Mat BestMap(Mat& L1, Mat& L2);    /*最优分配的二部图匹配*/
float Evaluate(Mat& labels, Mat& gnd);	/*计算准确率，返回AC*/
//int FindCommonLength(vector<float> vec1, float label1, vector<float> vec2, float label2);   /*找到不能赋值的共同部分*/
int testKmeans();
void reindex(Mat& data);
void Grouppring(Mat& L1, Mat& L2);

vector<String> getLinesFromFile(int line);
void saveBitSetToFile(Mat& data);
int distanceUseIndex(int a, int b);

void hartigan(Mat& data, int k, Mat& lables, int round);
Mat Matdistance(Mat& x, Mat& y);
void printMat(Mat& a);
Mat matMinus(Mat& a, Mat& b);

void MatMin(Mat& data, Mat& index, Mat& dist);



//Bithartigan
void BitHartigan(unsigned char**& data, int rows, int cols, int k, int*& lables, int round, double**& dataMap);
//unsigned char getDataAt(unsigned char**& data, int row, int cols);
//unsigned char getVectorDataAt(unsigned char*& data, int row, int cols);
double* VectorBitDistance(unsigned char*& data, int dcol, double**& centers, int crow, int ccol, double**& dataMap);
//double** BitDistance(unsigned char**& data, int drow, int dcol, double**& centers, int crow, int ccol, double**& dataMap);
double** BitDistance(unsigned char**& data, int drow, int dcol, double**& centers, int crow, int ccol, double**& dataMap, double* tmpXPowerSum, double* tmpYPower);

void updateCenters(Mat& data, Mat& index, Mat& centers, int n);
Mat ToBinary(Mat& data);
Mat saveDataAsBinary(Mat& data);
unsigned char** converToBinary(Mat& data);


void Kmeans(Mat& data, int k, Mat& lables, int round);
void BitKmeans(unsigned char**& data, int rows, int cols, int k, int*& lables, int round,double**& dataMap);
double** KmeansBitDistance(unsigned char**& data, int drow, int dcol, double**& centers, int crow, int ccol, double**& dataMap);

double* centersPower(double**& centers, int crow, int ccol);
double* dataPower(unsigned char**& data, int drow, int dcol, double**& dataMap);