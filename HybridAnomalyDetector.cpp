#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector():SimpleAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() = default;

void HybridAnomalyDetector::createCorreletedFeature(const TimeSeries& ts, int sizeOfVector, float m, int c,
                                                    string feature1, string feature2,Point** pointArr) {
    int sizeOfArr = sizeof(pointArr)/sizeof(pointArr[0]);
    SimpleAnomalyDetector::createCorreletedFeature(ts, sizeOfVector, m, c, feature1, feature2, pointArr);
    if (m > 0.5 && m < 0.9) {
        Circle minimalCircle = findMinCircle(pointArr, sizeOfVector);
        correlatedFeatures cfTemp;
        cfTemp.feature1 = feature1;
        cfTemp.feature2 = feature2;
        cfTemp.corrlation = m;
        cfTemp.lin_reg = linear_reg(pointArr, sizeOfVector);
        float radius = minimalCircle.radius*(float)1.1;
        cfTemp.threshold = radius;
        cfTemp.x = minimalCircle.center.x;
        cfTemp.y = minimalCircle.center.y;
        cf.push_back(cfTemp);
    }
}

bool HybridAnomalyDetector::checkIfAnomalous(float x, float y, correlatedFeatures tempCF) {
    Circle tempCircle = Circle(Point(tempCF.x, tempCF.y), tempCF.threshold);
    if (tempCF.corrlation >= 0.9 && SimpleAnomalyDetector::checkIfAnomalous(x,y,tempCF)) {
        return true;
    }
    Point* p = new Point(x,y);
    if ((tempCF.corrlation > 0.5 && tempCF.corrlation < 0.9) && (!ifPointIsInCircle(tempCircle, p))) {
        delete p;
        return true;
    }
    return false;
}