#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "util.h"

using namespace cv;
using namespace std;

void saveall(int featureNum) {
    saveIntoCsv(featureNum);  
}

vector<int> rankList(Mat3f targetImage, int featureNum, int numRank) {
    // loading the CSV into array
    vector<vector<float>> featureArray;

    // defining return array
    vector<int> returnArray;

    // defining the distance array
    vector<vector<float>> distanceArray;
    vector<vector<float>> sortedArray;
    vector<float> targetFeature;
    if (featureNum == 1) {
        loadCSVintoArray(featureNum, featureArray);
        feature9x9(targetImage, targetFeature);
    }
    if (featureNum == 2) {
        loadCSVintoArray(featureNum, featureArray);
        Hist3D(targetImage, targetFeature);
    }
    if (featureNum == 3) {
        loadCSVintoArray(featureNum, featureArray);
        MultiHist(targetImage, targetFeature);

    }
    for (int outerIndex = 0; outerIndex < featureArray.size(); outerIndex++) {
        // removing picture name and copying into another vector
        vector<float> comparisonFeatures(featureArray[outerIndex].begin() + 1, featureArray[outerIndex].end());
        // get Distance using SSD
        float distance = 0;
        if (featureNum == 1) {
            distance = sumSquaredError(targetFeature, comparisonFeatures);
        }
        if (featureNum == 2) {
            distance = histIntersection(targetFeature, comparisonFeatures);
        }
        if (featureNum == 3) {
            distance = histIntersection(targetFeature, comparisonFeatures);
        }
        // add them to distance Array
        vector<float> distanceArrayElement;
        distanceArrayElement.push_back(featureArray[outerIndex][0]);
        distanceArrayElement.push_back(distance);
        distanceArray.push_back(distanceArrayElement);
    }
    sortedArray = sortDistance(distanceArray);
    

    for (int i = 0; i < numRank; i++) {
        //cout << sortedArray[i][0] << "\n";
        returnArray.push_back(sortedArray[i][0]);
    }


    return returnArray;
}

int main(){

    // Select the feature
    // 9x9 pixels in the middle
    //int featureNum = 1;
    // 3D histogram of the whole image
    //int featureNum = 2;
    // Multi histogram
    int featureNum = 3;
    
    // Save all the features of the dataset
    //saveall(featureNum);
    string directory = "olympus/";

    // Target image
    string file_name = directory + "pic.0274.jpg";
    Mat3f targetImage = imread(file_name, IMREAD_COLOR);
    // Get the top n matches
    int n = 3;
    vector<int> rankedPictureNames;
    rankedPictureNames = rankList(targetImage, featureNum, n+1);

    // display the result
    Mat FinalCollage;
    vector<Mat> picHolder;
    for (int i = 0; i < n + 1; i++) {
        int picNum = rankedPictureNames[i];
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(4) << picNum;
        std::string strValue = ss.str();
        string picName = directory + "pic." + strValue + ".jpg";
        Mat image = imread(picName);
        picHolder.push_back(image);
    }
    hconcat(picHolder, FinalCollage);
    imshow("Results", FinalCollage);
    waitKey(0);

    return 0;
}
