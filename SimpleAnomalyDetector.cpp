//Author: 315097113 Noam Pdut
#include "SimpleAnomalyDetector.h"
#include <map>

SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

/*
 * convert the vectors of the two correlated fetters into Points array.
 */
Point** vectorsToPoints(vector<float> feature1, vector<float> feature2) {
    Point** p = new Point*[feature1.size()];
    for (int i = 0; i < feature1.size(); i++){
        p[i] = new Point(feature1[i],feature2[i]);
    }
    return p;
}

/*
 * delete the points array of every correlated features.
 */
void deletePoint(Point** p, int sizeArray) {
    for (int i = 0; i < sizeArray; i++) {
        delete p[i];
    }
    delete [] p;
}

/*
 * the function calculate the correletion between every feature by getting a normal data of TimeSeries.
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    map<string,vector<float>> data= ts.getMap();
    vector<string> vectorOfFeatures = ts.getFeatures();
    int numberOfFeatures = vectorOfFeatures.size();
    int sizeOfVector = data[vectorOfFeatures[1]].size(); //size of the rows.
    Point** pointArr;

    /*
     * implement the pseudocode of finding the most correlative pairs of features.
     */
    for (int i = 0; i < numberOfFeatures; i ++) {
        float m = 0;
        int c = -1;
        //run over all the others features.
        for (int j = i+1; j< numberOfFeatures; j++) {
            //get the vectors as array.
            float* arrI = &data[vectorOfFeatures[i]][0];
            float* arrJ = &data[vectorOfFeatures[j]][0];
            //get the correlation between the current features.
            float p = std::abs(pearson(arrI, arrJ, sizeOfVector));
            //check if the current features is the most correlated to features i.
            if(p > m) {
               m = p;
               c = j;
           }
        }
        //check if there is correlation with feature i, and if so, check if it is higher than 0.9.
        if(c != -1 && m > 0.9) {
            //create correlated feature struct and push it to cf vector member.
            correlatedFeatures cfTemp;
            cfTemp.feature1 = vectorOfFeatures[i];
            cfTemp.feature2 = vectorOfFeatures[c];
            cfTemp.corrlation = m;
            //create array of points, to get the linear reg.
            pointArr = vectorsToPoints(data[vectorOfFeatures[i]], data[vectorOfFeatures[c]]);
            cfTemp.lin_reg = linear_reg(pointArr, sizeOfVector);
            cfTemp.threshold = 0.9;
            SimpleAnomalyDetector::cf.push_back(cfTemp);
        }
    }
    //update the thresholds of every correlated features.
    SimpleAnomalyDetector::maxOffset(ts);
    //delete the points array.
    deletePoint(pointArr, sizeOfVector);
}
/*
 * detect all the deviations of the Time Series, by checking for every row if there is deviation between two correlated
 * features.
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    map<string,vector<float>> data= ts.getMap();
    vector<string> vectorOfFeatures = ts.getFeatures();
    int sizeOfVector = data[vectorOfFeatures[1]].size(); //number of rows
    vector<AnomalyReport> reportsVector;

    //run over all the lines.
    for( int i = 0; i < sizeOfVector; i++) {
        //run over all the correlated features in the row
        for( int j = 0; j < cf.size(); j++) {
            //get the x,y values of the two features in the current line.
            float x = data[cf[j].feature1][i];
            float y = data[cf[j].feature2][i];
            //get the dev between the excepted y and the current y.
            float temp = abs(y-cf[j].lin_reg.f(x));
            //check if there is deviation.
            if (temp >= cf[j].threshold) {
                //create an anomaly report and add it into a reports vector.
                AnomalyReport report = AnomalyReport(cf[j].feature1+ "-" +cf[j].feature2, i+1);
                reportsVector.push_back(report);
            }
        }
    }
    return reportsVector;
}

/*
 * for every two correlated features, get the max offset.
 */
void SimpleAnomalyDetector::maxOffset(const TimeSeries& ts) {
    map<string,vector<float>> data= ts.getMap();
    vector<string> vectorOfFeatures = ts.getFeatures();
    // the number of the correlated features in cf.
    int numOfCorreletedFeatures = cf.size();
    // the number of the rows.
    int sizeOfVector = data[vectorOfFeatures[1]].size();
    float maxNormalOffset = 0;
    // go over the correlated features and calculate the max normal offset.
    for (int i = 0; i < numOfCorreletedFeatures; i++) {
        for(int j = 0; j < sizeOfVector; j++) {
            float x = data[cf[i].feature1][j];
            float y = data[cf[i].feature2][j];
            // get the current offset
            float temp = abs(y-cf[i].lin_reg.f(x));
            // check if the current offset is higher than the max.
            if (temp > maxNormalOffset) {
                maxNormalOffset = temp;
            }
            // update the threshold.
            cf[i].threshold = maxNormalOffset * 1.1;
        }
    }
}