//modify version
/* 
 * File:   main.cpp
 * Author: heshan
 *
 * Created on April 8, 2018, 12:46 PM
 */

#include <vector>
#include <unistd.h>

#include "mysql/mysql.h"

#include "LSTMNet.h"
#include "DataProcessor.h"
#include "FileProcessor.h"
/*
 *index  			---> int 
 *Date  	 		---> char[20]
 *handover success rate  	---> float
 *DRB dealy1			---> float
 *DRB dealy2			---> float
 *DRB dealy3			---> float
 *DRB dealy4			---> float
 *Total delay			---> float
 * */



void add_LTable(MYSQL *conn, int index, const char *date, float handover, float delay1, float delay2, float delay3, float delay4, float total_delay) {
    char query[200];
    sprintf(query, "INSERT INTO lstm2 (`index`, date, handover, delay1, delay2, delay3, delay4, total_delay) VALUES (%d, '%s', %f, %f , %f , %f , %f , %f)", index, date, handover, delay1, delay2, delay3, delay4, total_delay);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Error executing MySQL query: %s\n", mysql_error(conn));
    } else {
        printf("Data added successfully!\n");
    }
}
void add_person_data(MYSQL *conn, const char *name, const char *sex, const char *height, const char *face) {
    	char query[200];
    	sprintf(query, "INSERT INTO person (name, sex, height, face) VALUES ('%s', '%s', '%s', '%s')", name, sex, height, face);
    
    	if (mysql_query(conn, query) != 0) {
        	fprintf(stderr, "Error executing MySQL query: %s\n", mysql_error(conn));
    	} else {
        	printf("Data added successfully!\n");
    	}
}
int msq(){
	MYSQL *conn = mysql_init(NULL);
	const char *host = "localhost";
	const char *user = "root";
	const char *password = "Sql^JX45";
	const char *database = "Jacktest";
	MYSQL_RES *result;
	MYSQL_ROW row;
	if (conn == NULL) {
   	     fprintf(stderr, "Error initializing MySQL connection: %s\n", mysql_error(conn));
   	     return 1;
   	 }
   	if (mysql_real_connect(conn, host, user, password, database, 0, NULL, 0) == NULL) {
   	     fprintf(stderr, "Error connecting to MySQL database: %s\n", mysql_error(conn));
   	     mysql_close(conn);
   	     return 1;
   	}
   	if (mysql_query(conn, "SELECT * FROM `lstm`") != 0) {
   	     fprintf(stderr, "Error executing MySQL query: %s\n", mysql_error(conn));
   	     mysql_close(conn);
   	     return 1;
   	}
	result = mysql_store_result(conn);
	add_LTable(conn, 1, "2018-04-08", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6);
	add_LTable(conn, 2, "2018-04-08", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6);
	add_LTable(conn, 3, "2018-04-08", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6);
	while ((row = mysql_fetch_row(result)) != NULL) {
    		printf("%s %s %s %s\n", row[0], row[1], row[2], row[3]);
	}
	mysql_free_result(result);
    	mysql_close(conn);
}


void dataconvert(){    
    ////////// Converting the CVS ////////////////////////    
    
        
//    fileProc->writeUniVariate("datasets/internetTrafficData.csv","datasets/InternetTraff.txt",2,1);
//    fileProc->writeUniVariate("datasets/monthlyReturnsOfValueweighted.csv","datasets/monthlyReturnsOfValueweighted.txt",2,1);
//    fileProc->writeUniVariate("datasets/treeAlmagreMountainPiarLocat.csv","datasets/treeAlmagreMountainPiarLocat.txt",2,1);
//    fileProc->writeUniVariate("datasets/dailyCyclistsAlongSudurlandsb.csv","datasets/dailyCyclistsAlongSudurlandsb.txt",2,1);
//    fileProc->writeUniVariate("datasets/totalPopulation.csv","datasets/totalPopulation.txt",2,1);
//    fileProc->writeUniVariate("datasets/numberOfUnemployed.csv","datasets/numberOfUnemployed.txt",2,1);
//    fileProc->writeUniVariate("datasets/data.csv","datasets/data.txt",2,1);
//    fileProc->writeUniVariate("datasets/monthlySunspotNumbers.csv","datasets/monthlySunspotNumbers.txt",2,1);
//    fileProc->writeUniVariate("datasets/dailyMinimumTemperatures.csv","datasets/dailyMinimumTemperatures.txt",2,1);    
//fileProc->writeUniVariate("datasets/Test.csv","datasets/Test.txt",4,2);    

	FileProcessor * fileProc;
	fileProc = new FileProcessor();
//Multi Dataset File from Csv to Txt
	fileProc->Csv_to_Txt("datasets/output.csv","datasets/output.txt");    
	fileProc->Csv_to_Txt("datasets/output2.csv","datasets/output2.txt");    
	fileProc->Csv_to_Txt("datasets/output3.csv","datasets/output3.txt");    
	fileProc->Csv_to_Txt("datasets/output4.csv","datasets/output4.txt");    
	fileProc->Csv_to_Txt("datasets/output5.csv","datasets/output5.txt");    
	
	fileProc->Delete_row("datasets/output.txt","datasets/ADF.txt",0);//Default off
	fileProc->Delete_row("datasets/output2.txt","datasets/ADF2.txt",0);//Default off
	fileProc->Delete_row("datasets/output3.txt","datasets/ADF3.txt",0);//Default off
	fileProc->Delete_row("datasets/output4.txt","datasets/ADF4.txt",0);//Default off
	fileProc->Delete_row("datasets/output5.txt","datasets/ADF5.txt",0);//Default off
	/* Test function
	 * fileProc->Set_row(1233);
	 * fileProc->Set_col(4566);
	 * fileProc->Delete_column("datasets/output2.csv","datasets/modify.txt",9);
	 *
	 *
	 *
	 *
	
	std::cout<<"ROW = "<<fileProc->Show_p_row()<<std::endl;
	std::cout<<"COL = "<<fileProc->Show_p_col()<<std::endl;
	*/
	

	fileProc->Split_txt("datasets/output.txt","datasets/train.txt","datasets/test.txt","datasets/val.txt",0.3,0.6,0.1);
	fileProc->Split_txt("datasets/output2.txt","datasets/train2.txt","datasets/test2.txt","datasets/val2.txt",0.3,0.6,0.1);
	fileProc->Split_txt("datasets/output3.txt","datasets/train3.txt","datasets/test3.txt","datasets/val3.txt",0.3,0.6,0.1);
	fileProc->Split_txt("datasets/output4.txt","datasets/train4.txt","datasets/test4.txt","datasets/val4.txt",0.3,0.6,0.1);
	fileProc->Split_txt("datasets/output5.txt","datasets/train5.txt","datasets/test5.txt","datasets/val5.txt",0.3,0.6,0.1);

}
int multivarPredicts() {

    ///////////////////////// Multivariate time series data prediction ////////////////////////////////////
    
    int memCells = 10; // number of memory cells
    int inputVecSize = 5; // input vector size
    int trainDataSize = 5000; // train data size
    int timeSteps = 1; // data points used for one forward step
    float learningRate = 0.0001;
    int iterations = 10; // training iterations with training data
    int lines = 5000;
    
    DataProcessor * dataproc;
    dataproc = new DataProcessor();
    FileProcessor * fileProc;
    fileProc = new FileProcessor();
    
    int colIndxs[] = {0,0,1,1,1,1,1};
    int targetValCol = 7;
    
    std::vector<double> * timeSeries;
    //timeSeries = fileProc->readMultivariate("datasets/multivariate/input/occupancyData/datatraining.txt",lines,inputVecSize,colIndxs,targetValCol);
    timeSeries = fileProc->readMultivariate("datasets/train.txt",lines,inputVecSize,colIndxs,targetValCol);
    
    // Creating the input vector Array
    std::vector<double> * input;
    input = new std::vector<double>[trainDataSize];    
    for (int i = 0; i < trainDataSize; i++) {
        input[i] = dataproc->process(timeSeries[i],0);
    }
    
    // Creating the target vector using the time series 
    std::vector<double>::const_iterator first = timeSeries[lines].begin();
    std::vector<double>::const_iterator last = timeSeries[lines].begin() + trainDataSize;
    std::vector<double> targetVector(first, last);
    for (std::vector<double>::iterator it = targetVector.begin(); it != targetVector.end(); ++it) {
        if (*it == 0) *it = -1;
    }    
    
    // Training the LSTM net
    LSTMNet * lstm = new LSTMNet(memCells,inputVecSize);    
    lstm->train(input, targetVector, trainDataSize, timeSteps, learningRate, iterations);
  
    // Predictions
    int predictions = 2000; // prediction points
    lines = 2000; // lines read from the files
    
    //timeSeries = fileProc->readMultivariate("datasets/multivariate/input/occupancyData/datatest.txt",lines,inputVecSize,colIndxs,targetValCol);
    timeSeries = fileProc->readMultivariate("datasets/test.txt",lines,inputVecSize,colIndxs,targetValCol);
    input = new std::vector<double>[1];
    double result;
    double min = 0, max = 0;
    std::vector<double> resultVec;
    for (int i = 0; i < predictions; i++) {   
        input[0] = dataproc->process(timeSeries[i],0);
	std::cout<<"Input["<<i<<"] = ";




	for(std::vector<double>::iterator pv = input->begin();pv != input->end();pv++)
		std::cout<<*pv<<",";
	std::cout<<"\b";
	std::cout<<" ";
	std::cout<<std::endl;



	std::cout<<std::endl;
        result = lstm->predict(input);
        resultVec.push_back(result);
        std::cout<<std::endl<<"result: "<<result<<"  ==>  expected: "<<timeSeries[lines].at(i)<<std::endl;
        
        if (i == 0){
            min = result;
            max = result;
        } else {
        
            if (min > result) min = result;
            if (max < result) max = result;
        }
    }
    std::cout<<"min: "<<min<<std::endl;
    std::cout<<"max: "<<max<<std::endl;
    
    double line = 0; //(min + max)/2;
    std::cout<<"margin: "<<line<<std::endl<<std::endl;
    
    
    int occu = 0, notoccu = 0;
    
    int corr = 0;
    int incorr = 0;
    
    int truePos = 0;
    int falsePos = 0;
    int trueNeg = 0;
    int falseNeg = 0;
    
    int corrNwMgn = 0;
    int incorrNwMgn = 0;
    
    // Open the file to write the time series predictions
    std::ofstream out_file;
    std::ofstream out_file2;
    out_file.open("datasets/multivariate/predictions/occupancyData/multiResults.txt",std::ofstream::out | std::ofstream::trunc);
    out_file2.open("datasets/multivariate/predictions/occupancyData/multiTargets.txt",std::ofstream::out | std::ofstream::trunc);
    
    for (int i = 0; i < predictions; i++) { 
        out_file<<timeSeries[lines].at(i)<<","<<resultVec.at(i)<<"\n";
        out_file2<<timeSeries[lines].at(i)<<",";
        if (timeSeries[lines].at(i) == 1) {
            out_file2<<1<<"\n";
        } else out_file2<<-1<<"\n";
        
        if ( (resultVec.at(i) > line) && (timeSeries[lines].at(i) == 1)) { 
            corr++;
            truePos++;
            occu++;
        } else if ( (resultVec.at(i) <= line) && (timeSeries[lines].at(i) == 0)) {
            corr++;
            trueNeg++;
            notoccu++;
        } else if ( (resultVec.at(i) <= line) && (timeSeries[lines].at(i) == 1)) { 
            incorr++; 
            falseNeg++;
            occu++;
        } else if ( (resultVec.at(i) > line) && (timeSeries[lines].at(i) == 0)) { 
            incorr++; 
            falsePos++;
            notoccu++;
        }
        //std::cout<<resultVec.at(i)<<" ------ "<<timeSeries[lines].at(i)<<"\n";
        
    }
    
    std::cout<<std::endl;
    
    std::cout<<"----------------------"<<std::endl;
    std::cout<<"Data "<<std::endl;
    std::cout<<"----------------------"<<std::endl;
    std::cout<<"Occupied: "<<occu<<std::endl;
    std::cout<<"NotOccupied: "<<notoccu<<std::endl<<std::endl;
    
    std::cout<<"----------------------"<<std::endl;
    std::cout<<"margin: "<<line<<std::endl;
    std::cout<<"----------------------"<<std::endl;
    std::cout<<"Correct predictions: "<<corr<<std::endl;
    std::cout<<"Incorrect predictions: "<<incorr<<std::endl<<std::endl;
    
    std::cout<<"True Positive: "<<truePos<<std::endl;
    std::cout<<"True Negative: "<<trueNeg<<std::endl;
    std::cout<<"False Positive: "<<falsePos<<std::endl;
    std::cout<<"False Negative: "<<falseNeg<<std::endl;
    
    std::cout<<std::endl<<"Accuracy: "<<(corr/(double)predictions)*100<<"%"<<std::endl<<std::endl;
    
    
    line = (min + max)/2;
    occu = 0;
    notoccu = 0;
    corr = 0;
    incorr = 0;
    truePos = 0;
    falsePos = 0;
    trueNeg = 0;
    falseNeg = 0;
    
    for (int i = 0; i < predictions; i++) {    
        if ( (resultVec.at(i) > line) && (timeSeries[lines].at(i) == 1)) { 
            corr++;
            truePos++;
            occu++;
        } else if ( (resultVec.at(i) <= line) && (timeSeries[lines].at(i) == 0)) {
            corr++;
            trueNeg++;
            notoccu++;
        } else if ( (resultVec.at(i) <= line) && (timeSeries[lines].at(i) == 1)) { 
            incorr++; 
            falseNeg++;
            occu++;
        } else if ( (resultVec.at(i) > line) && (timeSeries[lines].at(i) == 0)) { 
            incorr++; 
            falsePos++;
            notoccu++;
        }
        
        
        
        if (line > 0) {
            if ( (resultVec.at(i) <= line) && (resultVec.at(i) > 0)) {
                if (timeSeries[lines].at(i) == 0) {
                    corrNwMgn++;
                } else incorrNwMgn++;
            }
        } else {
            if ( (resultVec.at(i) > line) && (resultVec.at(i) < 0)) {
                if (timeSeries[lines].at(i) == 1) {
                    corrNwMgn++;
                } else incorrNwMgn++;
            }
        }
        
    }
    
    std::cout<<"----------------------"<<std::endl;
    std::cout<<"margin: "<<line<<std::endl;
    std::cout<<"----------------------"<<std::endl;
    std::cout<<"Correct predictions: "<<corr<<std::endl;
    std::cout<<"Incorrect predictions: "<<incorr<<std::endl<<std::endl;
    
    std::cout<<"True Positive: "<<truePos<<std::endl;
    std::cout<<"True Negative: "<<trueNeg<<std::endl;
    std::cout<<"False Positive: "<<falsePos<<std::endl;
    std::cout<<"False Negative: "<<falseNeg<<std::endl;
    
    std::cout<<std::endl<<"Accuracy: "<<(corr/(double)predictions)*100<<"%"<<std::endl<<std::endl;
    
    std::cout<<"----------------------"<<std::endl;
    std::cout<<"Within the new margin and 0"<<std::endl;
    std::cout<<"----------------------"<<std::endl;
    std::cout<<"Correct: "<<corrNwMgn<<std::endl;
    std::cout<<"Incorrect: "<<incorrNwMgn<<std::endl<<std::endl<<std::endl;
    
    return 0;
}


int main() {
	
	msq();



    dataconvert();	
    std::cout<<"-----multivariate starts now-----"<<std::endl;
    // predicting multivariate series
//    multivarPredicts();

}
