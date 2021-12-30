

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
struct CurrentData{
    float threshold;
    vector<AnomalyReport> reports;
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
	virtual void execute(CurrentData currentData)=0;
	virtual ~Command(){}
};

// implement here your command classes

class UploadCSV:public Command{
    UploadCSV(DefaultIO* dio): Command(dio){
        this->description = "upload a time series csv file";
    };
    void execute(CurrentData currentData) const {
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
    void execute(CurrentData currentData) {
        this->dio->write("The current correlation threshold is " + to_string(currentData.threshold) +"\n");
        float choose;
        this->dio->read(&choose);
        if (choose > 0 || choose < 1){
            currentData.threshold = choose;
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
    void execute(CurrentData currentData) {
        TimeSeries timeSeries1 = TimeSeries("anomalyTrain.csv");
        TimeSeries timeSeries2 = TimeSeries("anomalyTest.csv");
        HybridAnomalyDetector hybridAnomalyDetector;
        hybridAnomalyDetector.setDefaultThreshold(currentData.threshold);
        hybridAnomalyDetector.learnNormal(timeSeries1);
        currentData.reports = hybridAnomalyDetector.detect(timeSeries2);
        this->dio->write("anomaly detection complete\n");
    }
};


class DisplayResults: public Command {
    DisplayResults(DefaultIO* dio): Command(dio){
        this->description = "display results\n";
    }
    void execute(CurrentData currentData) {
        for(int i=0; i< currentData.reports.size(); i++) {
            this->dio->write(to_string(currentData.reports[i].timeStep) + "\t" + currentData.reports[i].description + "\n");
        }
        this->dio->write("Done.\n");
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
