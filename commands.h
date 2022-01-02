

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"


using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
    void readToFile(string fileName){
        ofstream out(fileName);
        string s="";
        while((s=read())!="done"){
            out<<s<<endl;
        }
        out.close();
    }
};

// you may add here helper classes
struct sharedReports{
    int startTimeStep;
    int endTimeStep;
    string description;
    bool tp;
};

struct CurrentData{
    float threshold;
    vector<AnomalyReport> reports;
    vector<sharedReports> sReports;
    CurrentData(){
        threshold=0.9;
    }
};

// you may edit this class
class Command {
protected:
	DefaultIO* dio;
public:
    string description;
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute(CurrentData* currentData)=0;
	virtual ~Command(){}
};

// implement here your command classes

class UploadCSV:public Command{
    UploadCSV(DefaultIO* dio): Command(dio){
        this->description = "upload a time series csv file";
    };
    void execute(CurrentData* currentData) const {
        this->dio->write("Please upload your local train CSV file.\n");
        this->dio->readToFile("anomalyTrain.csv");
        this->dio->write("Upload complete\n");
        this->dio->write("Please upload your local test CSV file.\n");
        this->dio->readToFile("anomalyTest.csv");
        this->dio->write("Upload complete\n");
    }
};

class algorithmSettings: public Command{
    algorithmSettings(DefaultIO* dio): Command(dio){
        this->description = "algorithm settings\n";
    }
    void execute(CurrentData* currentData) {
        this->dio->write("The current correlation threshold is " + to_string(currentData.threshold) +"\n");
        float choose;
        this->dio->read(&choose);
        if (choose > 0 || choose < 1){
            currentData->threshold = choose;
            return;
        }
        this->dio->write("please choose a value between o and 1\n");
        execute(currentData);
    }
};

class detectAnomalies: public Command{
    detectAnomalies(DefaultIO* dio): Command(dio){
        this->description = "detect anomalies\n";
    }
    void execute(CurrentData* currentData) {
        TimeSeries timeSeries1 = TimeSeries("anomalyTrain.csv");
        TimeSeries timeSeries2 = TimeSeries("anomalyTest.csv");
        HybridAnomalyDetector hybridAnomalyDetector;
        hybridAnomalyDetector.setDefaultThreshold(currentData->threshold);
        hybridAnomalyDetector.learnNormal(timeSeries1);
        currentData->reports = hybridAnomalyDetector.detect(timeSeries2);
        this->dio->write("anomaly detection complete\n");
    }
};


class DisplayResults: public Command {
    DisplayResults(DefaultIO* dio): Command(dio){
        this->description = "display results\n";
    }
    void execute(CurrentData* currentData) {
        for(int i=0; i< currentData->reports.size(); i++) {
            this->dio->write(to_string(currentData.reports[i].timeStep) + "\t" + currentData.reports[i].description + "\n");
        }
        this->dio->write("Done.\n");
    }
};

class analyzeResult: public Command {
    analyzeResult(DefaultIO* dio): Command(dio){
        this->description = "Please upload your local anomalies file.\n";
    }

    void createSharedReports(CurrentData* currentData) {
        sharedReports sr;
        sr.description = "";
        sr.startTimeStep = 0;
        sr.endTimeStep = 0;
        sr.tp = false;
        for(vector<AnomalyReport>::iterator it = currentData->reports.begin(); it != currentData->reports.end(); ++it) {
            if (sr.description == it->description && it->timeStep+1 == sr.endTimeStep) {
                sr.endTimeStep++;
            }
            else {
                currentData->sReports.push_back(sr);
                sr.description = it->description;
                sr.startTimeStep = it->timeStep;
                sr.endTimeStep = sr.startTimeStep;
                sr.tp = false;
            }
        }
        currentData->sReports.push_back(sr);
        currentData->sReports.erase(currentData->sReports.begin());
    }

    bool OverlapTP(int start, int end, vector<sharedReports> sr) {
        for(vector<sharedReports>::iterator it = sr.begin(); it != sr.end(); ++it) {
            if (start >= it->startTimeStep )
        }
    }

    /*
     * 110-120
     *
     * 100-105
     * 110-125
     * 105-110
     */

    void execute(CurrentData* currentData) {
        createSharedReports(currentData);

        float p = 0;
        int tp = 0;
        int sumOfInputLines = 0;
        string line = dio->read();
        while (line != "done\n") {
            int index = line.find(":");
            int start = stoi(line.substr(0, index));
            int end = stoi (line.substr(index+1,line.length()));
                if(OverlapTP(start,end,currentData->sReports))
                    tp++;
                sumOfInputLines+=end+1-start;
                p++;
            }


            line = dio->read();
        }
};

class StandardIO:public DefaultIO {
    StandardIO(): DefaultIO(){};

    void write(string text) {
        cout << "" << text;
    }
    void write(float f) {
        cout << "" << f;
    }
};

#endif /* COMMANDS_H_ */
