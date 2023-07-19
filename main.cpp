//modify version
/* 
 * File:   main.cpp
 * Author: Jackxul
 *
 * Created on April 8, 2018, 12:46 PM
 */

#define CROW_JSON_USE_MAP
#include <vector>
#include <unistd.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json.hpp>
#include "mysql/mysql.h"
#include "crow.h"
//#include "crow/tower.h"
#include "LSTMNet.h"
#include "DataProcessor.h"
#include "FileProcessor.h"
//#define Mode false // true: create table, false: clean table
bool Mode = false;
bool API_Mode = false;
#define datarec_size 20
using json = nlohmann::json;

/*
 *index  			---> int 
 *Date  	 		---> char[20]
 *handover success rate  	---> float
 *DRB DRB_RlcDelayUL			---> float
 *DRB DRB_AirlfDelayUL			---> float
 *DRB DRB_RlcSduDelayDL			---> float
 *DRB DRB_AirlfDelayDL			---> float

 *Total delay			---> float
 * */
int _gNb_No = 0;
/*Data Rec Structure*/
struct DataRec{
	int index;
	char date[20];
	float handover;
	float DRB_RlcDelayUL;
	float DRB_AirlfDelayUL;
	float DRB_RlcSduDelayDL;
	float DRB_AirlfDelayDL;
	float total_delay;
};

std::vector<DataRec> DataVector;
FileProcessor * fileProc;

/*
Test Code

void add_person_data(MYSQL *conn, const char *name, const char *sex, const char *height, const char *face) {
    	char query[200];
    	sprintf(query, "INSERT INTO person (name, sex, height, face) VALUES ('%s', '%s', '%s', '%s')", name, sex, height, face);
    
    	if (mysql_query(conn, query) != 0) {a
        	fprintf(stderr, "Error executing MySQL query: %s\n", mysql_error(conn));
    	} else {
        	printf("Data added successfully!\n");
    	}
}
*/



int msq(){	




	return printf("Data added ly!\n");
	
	/*	Test Code	*/
	//MYSQL *conn = mysql_init(NULL);
	//const char *host = "localhost";
	//const char *user = "root";
	//const char *password = "Sql^JX45";
	//const char *database = "Jacktest";
	//MYSQL_RES *result;
	//MYSQL_ROW row;
	//if (conn == NULL) {
   	//     fprintf(stderr, "Error initializing MySQL connection: %s\n", mysql_error(conn));
   	//     return 1;
   	// }
   	//if (mysql_real_connect(conn, host, user, password, database, 0, NULL, 0) == NULL) {
   	//     fprintf(stderr, "Error connecting to MySQL database: %s\n", mysql_error(conn));
   	//     mysql_close(conn);
   	//     return 1;
   	//}

   	//if (mysql_query(conn, "SELECT * FROM `lstm2`") != 0) {
   	//     fprintf(stderr, "Error executing MySQL query: %s\n", mysql_error(conn));
   	//     mysql_close(conn);
   	//     return 1;
   	//}
	//result = mysql_store_result(conn);
	//add_LTable(conn, 1, "2018-04-08", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6);
	//add_LTable(conn, 2, "2018-04-08", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6);
	//add_LTable(conn, 3, "2018-04-08", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6);



	//add_person_data(conn, "Bob", "GAY", "0.01", "pathetic");

	//while ((row = mysql_fetch_row(result)) != NULL) {
    	//	printf("%s %s %s %s\n", row[0], row[1], row[2], row[3]);
	//}
	//mysql_free_result(result);
    	//mysql_close(conn);
}


void dataconvert(){    
	fileProc = new FileProcessor();
    ////////// Converting the CVS ////////////////////////    
    

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
	
	MYSQL* conn;

	fileProc->connect_db(conn);
	std::cout<<"connect successfully!"<<std::endl;
	
	fileProc->create_table(conn,1 , Mode);
	fileProc->create_table(conn,2 , Mode);
	fileProc->create_table(conn,3 , Mode);
	fileProc->create_table(conn,4 , Mode);
	fileProc->create_table(conn,5 , Mode);

	//(val : test : train)
	fileProc->Split_txt(conn , 1 , "datasets/output.txt",0.1,0.4,0.5 , Mode);
	fileProc->Split_txt(conn , 2 , "datasets/output2.txt",0.2,0.6,0.2 , Mode);
	fileProc->Split_txt(conn , 3 , "datasets/output3.txt",0.3,0.6,0.1 , Mode);
	fileProc->Split_txt(conn , 4 , "datasets/output4.txt",0.3,0.4,0.3 , Mode);
	fileProc->Split_txt(conn , 5 , "datasets/output5.txt",0.2,0.5,0.3 , Mode);

	fileProc->clean_table(conn , 1 , 5 , Mode);
		


		/*MYSQL*/



	//add_LTable(conn, 1, "2018-04-08", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6);
	fileProc->close_db(conn);


		/*MYSQL*/
}

void datarec(std::string table_name , int gNb_No){
	MYSQL* conn;
	API_Mode = true;	
	_gNb_No = gNb_No;
	table_name = table_name + std::to_string(gNb_No);
	std::string query = "SELECT * FROM " + table_name +" WHERE count < 500";	
	fileProc->connect_db(conn);
	std::cout<<"connect successfully!"<<std::endl;
	



	if (mysql_query(conn, query.c_str())){
		std::cerr << "Failed to execute query: " << mysql_error(conn) << std::endl;
	}


	MYSQL_RES* result = mysql_store_result(conn);
		if (!result) {
	        	std::cerr << "Failed to retrieve result set: " << mysql_error(conn) << std::endl;
	    	}

	MYSQL_ROW row;
	while((row = mysql_fetch_row(result))) {
		DataRec datarec;

		datarec.index = std::stoi(row[1]);
		
		strncpy(datarec.date, row[2], 16);

		datarec.handover = std::stof(row[3]);
		datarec.DRB_RlcDelayUL = std::stof(row[4]);
		datarec.DRB_AirlfDelayUL = std::stof(row[5]);
		datarec.DRB_RlcSduDelayDL = std::stof(row[6]);
		datarec.DRB_AirlfDelayDL = std::stof(row[7]);
		datarec.total_delay = std::stof(row[8]);

		DataVector.push_back(datarec);
	}
	
	for(const auto& datarec : DataVector) {
		std::cout << datarec.index << " " << datarec.date << " " << datarec.handover << " " << datarec.DRB_RlcDelayUL << " " << datarec.DRB_AirlfDelayUL << " " << datarec.DRB_RlcSduDelayDL << " " << datarec.DRB_AirlfDelayDL << " " << datarec.total_delay << std::endl;
	}

	mysql_free_result(result);
	fileProc->close_db(conn);
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


//curl/curl.h functions
size_t curlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
   	size_t totalSize = size * nmemb;
    	response->append((char*)contents, totalSize);
    	return totalSize;
}

std::string makeApiCall(const std::string& url) {
    	CURL* curl = curl_easy_init();
    	std::string response;

    	if (curl) {
    		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    	    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    	    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    	    	CURLcode res = curl_easy_perform(curl);
    	    	if (res != CURLE_OK) {
    	    	    std::cerr << "Failed to perform API call: " << curl_easy_strerror(res) << std::endl;
    	    	}
    	    	curl_easy_cleanup(curl);
    	}

    	return response;
}

int main() {
	
	msq();

	crow::SimpleApp app;
	
	CROW_ROUTE(app, "/")([](){
		crow::json::wvalue::object outter;
		crow::json::wvalue::object outter2;
		if(!API_Mode && _gNb_No != -1){
			crow::json::wvalue xul;
			xul["Error403"] = "Direct Access , You Dummy";
			outter2.insert({"Failed", xul});
		}else if(!API_Mode && _gNb_No == -1){
			crow::json::wvalue xul;
			xul["Error404"] = "Wrong Table Name , Page Not Found";
			outter2.insert({"Failed", xul});
		}
		else{
			int count = 0;
			for(const auto& datarec : DataVector) {
				
				if(count > datarec_size ){
					break;
				}else if(count == datarec_size ){
					outter2.insert({std::to_string(_gNb_No), outter});
				}
				

				crow::json::wvalue xul;
				//xul["No"] = Jack_API_vec[0];
				xul["1 : Date"] = datarec.date;
				xul["2 : Handover"] = datarec.handover;
				xul["3 : DRB_RlcUL"] = datarec.DRB_RlcDelayUL;
				xul["4 : DRB_AirUL"] = datarec.DRB_AirlfDelayUL;
				xul["5 : DRB_RlcDL"] = datarec.DRB_RlcSduDelayDL;
				xul["6 : DRB_AirDL"] = datarec.DRB_AirlfDelayDL;
				xul["7 : Total_Delay"] = datarec.total_delay;
		
				outter.insert({std::to_string(count), xul});
				count++;
			}
		}
		API_Mode = false;
		_gNb_No = 0;
		crow::json::wvalue response1{outter2};
		return crow::response(response1);

	});
	CROW_ROUTE(app, "/json")
	([]{
    		crow::json::wvalue x({{"message1", "Test Code!"}});
    		x["message2"] = "Whats up!";
    		return x;
	});
	
//	CROW_ROUTE(app, "/cr")
//	([](const crow::request& req, crow::response& res){
//	 	crow::http::request apiRequest;
//		apiRequest.method = 'GET';
//		apiRequest.url = "/cr2";
//		auto apiResponse = crow::http::send(apiRequest);
//		if(apiResponse){
//			res.body = apiResponse->body;
//			res.code = apiResponse->code;
//			res.set_header("Content-Type", apiResponse->get_header_value("Content-Type"));
//			res.write("Table Created");
//		}else{
//			res.code = 500;
//			res.body = "Table Creation Failed";
//		}
//		res.end();
//
//    		return res;
//	});
//
//
//
	/*api call api*/
	CROW_ROUTE(app, "/<string>/<string>")
	([&app](const crow::request& req, crow::response& res, const std::string& tds, const std::string& ids){
		int id = std::stoi(ids);
		char *td;
		
		td = (char*)tds.c_str();

		//string compare
		
		
		res.add_header("Access-Control-Allow-Origin", "*");
        	res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        	res.add_header("Access-Control-Allow-Headers", "Content-Type");

		

		if(!(strncmp(td,"ttd",3)) && id > 0 && id <= 5){
			datarec("test_data" , id);
		}else if(!(strncmp(td,"trd",3)) && id > 0 && id <= 5){
			datarec("train_data" , id);
		}else if(!(strncmp(td,"vld",3))&&id > 0 && id <= 5){
			datarec("val_data" , id);
		}else{
			_gNb_No = -1;	
		}
		std::string apiResponse = makeApiCall("192.168.127.76:8888/");
		
		// Use the response from the API call in the current response
		
		json jsonResponse = json::parse(apiResponse);
		std::string message = jsonResponse.dump(4);
		res.set_header("Content-Type", "application/json");
		res.write(message);
		res.end();
	});
/*	Old Code
	CROW_ROUTE(app, "/trd/<string>")
	([&app](const crow::request& req, crow::response& res, const std::string& ids){
	 	int id = std::stoi(ids);
		if(id > 0 && id <= 5){
			datarec("train_data" , id);
		}else{
			_gNb_No = -1;
		}
		std::string apiResponse = makeApiCall("192.168.127.76:8888/");
		
		// Use the response from the API call in the current response
		
		json jsonResponse = json::parse(apiResponse);
		std::string message = jsonResponse.dump(4);
		res.set_header("Content-Type", "application/json");
		res.write(message);
		res.end();
	});

	CROW_ROUTE(app, "/vld/<string>")
	([&app](const crow::request& req, crow::response& res, const std::string& ids){
	 	int id = std::stoi(ids);
		if(id > 0 && id <= 5){
			datarec("val_data" , id);
		}else{
			_gNb_No = -1;
		}
		std::string apiResponse = makeApiCall("192.168.127.76:8888/");
		
		// Use the response from the API call in the current response
		
		json jsonResponse = json::parse(apiResponse);
		std::string message = jsonResponse.dump(4);
		res.set_header("Content-Type", "application/json");
		res.write(message);
		res.end();
	});
*/
	CROW_ROUTE(app, "/cr")
	([]{
	 	Mode = true;	
		dataconvert();
    		return "Finish Table Creation";
	});

	CROW_ROUTE(app, "/cl")
	([]{
	 	Mode = false;
		dataconvert();
    		return "Table Clean Up";
	});

	app.bindaddr("192.168.127.76").port(8888).multithreaded().run();

    	//dataconvert();	
    	std::cout<<"-----multivariate starts now-----"<<std::endl;
    	// predicting multivariate series
   	//multivarPredicts();

}
