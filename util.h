#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

int saveIntoCsv(int featureNum);
void feature9x9(Mat3f image, vector<float>& feature);
void Hist3D(Mat3f image, vector<float>& feature);
void MultiHist(Mat3f image, vector<float>& feature);
void loadCSVintoArray(int featureNum, vector<vector<float>>& features);
float sumSquaredError(vector<float> targetFeatures, vector<float> comparisonFeatures);
float histIntersection(vector<float> targetFeatures, vector<float> comparisonFeatures);
vector<vector<float>> sortDistance(vector<vector<float>> distanceArray);
bool sortcol(const vector<float>& v1, const vector<float>& v2);