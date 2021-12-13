//Author: 315097113 Noam Pdut

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
    float x,y;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
public:
    vector<correlatedFeatures> cf;
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

protected:
    void maxOffset(const TimeSeries& ts);
    virtual void createCorreletedFeature(const TimeSeries& ts, int sizeOfVector, float m, int c, string feature1, string feature2,
                                                        Point** pointArr);
    virtual bool checkIfAnomalous(float x, float y, correlatedFeatures tempCF);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
