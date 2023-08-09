/* 
 * File:   FileProcessor.cpp
 * Author: heshan
 * Modify: Jack_Xul --> 2022/03/30
 * Created on April 14, 2018, 7:45 PM
 */

#include <vector>
#include <exception>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include "FileProcessor.h"
#include "mysql/mysql.h"
FileProcessor::FileProcessor() { }

FileProcessor::FileProcessor(const FileProcessor& orig) { }

FileProcessor::~FileProcessor() { }


int lineNo = 0;
int tokenNo;
std::string *headerline = new std::string;

//mysql function implement
void FileProcessor::connect_db(MYSQL*& conn){
	conn = mysql_init(NULL);
	if(conn == NULL){
		std::cout<<"Error:"<<mysql_error(conn);
		exit(1);
	}
	if(mysql_real_connect(conn,"localhost","root","Sql^JX45","gNb",0,NULL,0) == NULL){
		std::cout<<"Error:"<<mysql_error(conn);
		exit(1);
	}
	std::cout<<"Connect to database successfully!"<<std::endl;
}

void FileProcessor::close_db(MYSQL*& conn){
	mysql_close(conn);
	std::cout<<"Close database successfully!"<<std::endl;
}

int FileProcessor::create_table(MYSQL*& conn, int tableNo , bool df){
	if(!df)
		return 0;
	std::string tableName = "test_data";
	tableName = tableName + std::to_string(tableNo);
	std::string sql = "CREATE TABLE " + tableName + " ( count INT , id INT , date VARCHAR(255) , handover FLOAT, DRB_RlcDelayUL  FLOAT , DRB_AirlfDelayUL FLOAT , DRB_RlcSduDelayDL FLOAT , DRB_AirlfDelayDL FLOAT , total_delay FLOAT , PRIMARY KEY(id))";
	if(mysql_query(conn,sql.c_str())){
		std::cout<<"Error test_data creation"<<mysql_error(conn);
		exit(1);
	}

	tableName = "train_data";
	tableName = tableName + std::to_string(tableNo);
	sql = "CREATE TABLE " + tableName + " ( count INT , id INT , date VARCHAR(255) , handover FLOAT, DRB_RlcDelayUL  FLOAT , DRB_AirlfDelayUL FLOAT , DRB_RlcSduDelayDL FLOAT , DRB_AirlfDelayDL FLOAT , total_delay FLOAT , PRIMARY KEY(id))";
	if(mysql_query(conn,sql.c_str())){
		std::cout<<"Error train_data creation"<<mysql_error(conn);
		exit(1);
	}
	tableName = "val_data";
	tableName = tableName + std::to_string(tableNo);
	sql = "CREATE TABLE " + tableName + " ( count INT , id INT , date VARCHAR(255) , handover FLOAT, DRB_RlcDelayUL  FLOAT , DRB_AirlfDelayUL FLOAT , DRB_RlcSduDelayDL FLOAT , DRB_AirlfDelayDL FLOAT , total_delay FLOAT , PRIMARY KEY(id))";
	if(mysql_query(conn,sql.c_str())){
		std::cout<<"Error val_data creation"<<mysql_error(conn);
		exit(1);
	}






	std::cout<<"Create table successfully!"<<std::endl;
	return 0;
}


int FileProcessor::clean_table(MYSQL*& conn, int startNo , int endNo , bool df){
	if(df)
		return 0;
	while(startNo <= endNo){
		std::string test = "test_data";
		std::string train = "train_data";
		std::string val = "val_data";
		test = test + std::to_string(startNo);
		train = train + std::to_string(startNo);
		val = val + std::to_string(startNo);

		std::string sql = "DROP TABLE IF EXISTS " + test + "," + train + "," + val;
		startNo++;
		if(mysql_query(conn,sql.c_str())){
			std::cout<<"Error:"<<mysql_error(conn);
			exit(1);
		}

	}
	std::cout<<"Clean table successfully!"<<std::endl;
	return 0;
}


// NDY //
void FileProcessor::add_data(MYSQL*& conn , std::string tableName ,int checkcount , int line_id , char *date , float handover , float DRB_RlcDelayUL , float DRB_AirlfDelayUL , float DRB_RlcSduDelayDL , float DRB_AirlfDelayDL , float total_delay){
	char query[200];
	sprintf(query,"INSERT INTO %s VALUES(%d,%d,'%s',%f,%f,%f,%f,%f,%f)",tableName.c_str(),checkcount,line_id,date,handover,DRB_RlcDelayUL,DRB_AirlfDelayUL,DRB_RlcSduDelayDL,DRB_AirlfDelayDL,total_delay);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Error executing MySQL query: %s\n", mysql_error(conn));
    } else {
        printf("Data added successfully!\n");
    }
}











//Fin
void FileProcessor::Set_col(int colval){
	mtx.lock();
	p_col = colval;
	std::cout<<"Set p_col => "<<p_col<<std::endl;
	mtx.unlock();
}
void FileProcessor::Set_row(int rowval){
	mtx.lock();
	p_row = rowval;
	std::cout<<"Set p_row => "<<p_row<<std::endl;
	mtx.unlock();
}
int FileProcessor::Show_p_row(){
	return p_row;
}
int FileProcessor::Show_p_col(){
	return p_col;
}
// NDY  //
int FileProcessor::Delete_column(std::string fileName, std::string outFileName, int targetCol) {
    	
	return 0;   
}
//
//
//
//
// NFM //
// 1:4:5 = datatest : datatest2 : datatraining = valFile : testFile : trainFile
int FileProcessor::Split_txt(MYSQL*& conn , int gNbNo , std::string fileName , float testv , float valv , float trainv , bool df){
	if(!df)
		return 0;
	if((trainv + testv + valv)!= 1.0){
		std::cout<<"Split Proportion Error"<<std::endl;
		return 0;
	}
	int checkcount = 0;
	int _index;
	char _date[20];
	float _handover;
	float _DRB_RlcDelayUL;
	float _DRB_AirlfDelayUL;
	float _DRB_RlcSduDelayDL;
	float _DRB_AirlfDelayDL;
	float _total_delay;
	int tokenNo = 0;
	std::string test = "test_data";
	std::string train = "train_data";
	std::string val = "val_data";
    	std::string token;
	/*
	 * testNo
	 * valNo
	 * trainNo
	 * */


	test = test + std::to_string(gNbNo);
	train = train + std::to_string(gNbNo);
	val = val + std::to_string(gNbNo);

	std::string line;

	const char *comma = ",";
	
	char* field = strtok(const_cast<char*>(line.c_str()), comma);	
	int count = 0;
	int testNo = (Show_p_row() * testv);
	int valNo = (Show_p_row() * (testv + valv));
	int trainNo = Show_p_row();

    	std::ifstream file (fileName);

	
	std::cout<<"This is test code of enter Split function"<<std::endl;
    	if (file.is_open()) {
		//Test
		
		std::cout<<"This is test code of enter Split function"<<std::endl;
		
		while(lineNo < testNo && getline(file,line)){
			lineNo++;
			std::cout<<"This is test code of enter Split while function"<<std::endl;
			
			//line[strcspn(line, "\n")] = '\0';
			//field = strtok(line,comma);
    			const char* comma = ",";
    			size_t pos = line.find('\0');
    			line[pos] = '\0';

    			char* field = strtok(const_cast<char*>(line.c_str()), comma);

    			while (field != nullptr) {
				
    			 	//std::cout<<count++<<" : "<< field<<std::endl;
				if(count == 0){
					_index = atoi(field);
				}
				else if(count == 1){
					strncpy(_date,field,20);
				}
				else if(count == 2){
					_handover = atof(field);
				}
				else if(count == 3){
					_DRB_RlcDelayUL = atof(field);
				}
				else if(count == 4){
					_DRB_AirlfDelayUL = atof(field);
				}
				else if(count == 5){
					_DRB_RlcSduDelayDL = atof(field);
				}
				else if(count == 6){
					_DRB_AirlfDelayDL = atof(field);
				}
				else if(count == 7){
					_total_delay = atof(field);
				}
				count += 1;

    			    	field = strtok(nullptr, comma);
    			}
			count = 0;
			//std::cout<<line<<std::endl;
			//std::cout<<"testNo = "<<lineNo<<std::endl;
			 
			std::cout<<_index<<",";
			std::cout<<_date<<",";
			std::cout<<_handover<<",";
			std::cout<<_DRB_RlcDelayUL<<",";
			std::cout<<_DRB_AirlfDelayUL<<",";
			std::cout<<_DRB_RlcSduDelayDL<<",";
			std::cout<<_DRB_AirlfDelayDL<<",";
			std::cout<<_total_delay;
			std::cout<<std::endl;
			add_data(conn,test,checkcount,_index,_date,_handover,_DRB_RlcDelayUL,_DRB_AirlfDelayUL,_DRB_RlcSduDelayDL,_DRB_AirlfDelayDL,_total_delay);
			checkcount++;
		}
		checkcount = 0;
		//std::cout<<"testEnd"<<std::endl;

		//Val
		
		while(lineNo < valNo && getline(file,line)){
			lineNo++;
			//std::cout<<line<<std::endl;
			//std::cout<<"valNo = "<<lineNo<<std::endl;
			
    			const char* comma = ",";
    			size_t pos = line.find('\0');
    			line[pos] = '\0';

    			char* field = strtok(const_cast<char*>(line.c_str()), comma);

    			while (field != nullptr) {
    			    //std::cout<<count++<<" : "<< field<<std::endl;
				if(count == 0){
					_index = atoi(field);
				}
				else if(count == 1){
					strncpy(_date,field,20);
				}
				else if(count == 2){
					_handover = atof(field);
				}
				else if(count == 3){
					_DRB_RlcDelayUL = atof(field);
				}
				else if(count == 4){
					_DRB_AirlfDelayUL = atof(field);
				}
				else if(count == 5){
					_DRB_RlcSduDelayDL = atof(field);
				}
				else if(count == 6){
					_DRB_AirlfDelayDL = atof(field);
				}
				else if(count == 7){
					_total_delay = atof(field);
				}
    			    	count += 1;
				field = strtok(nullptr, comma);
    			}
			count = 0;
			std::cout<<_index<<",";
			std::cout<<_date<<",";
			std::cout<<_handover<<",";
			std::cout<<_DRB_RlcDelayUL<<",";
			std::cout<<_DRB_AirlfDelayUL<<",";
			std::cout<<_DRB_RlcSduDelayDL<<",";
			std::cout<<_DRB_AirlfDelayDL<<",";
			std::cout<<_total_delay;
			std::cout<<std::endl;
			add_data(conn,val,checkcount,_index,_date,_handover,_DRB_RlcDelayUL,_DRB_AirlfDelayUL,_DRB_RlcSduDelayDL,_DRB_AirlfDelayDL,_total_delay);
			checkcount++;
		}
		//std::cout<<"valEnd"<<std::endl;

		//Train
		checkcount = 0;
		while(lineNo < trainNo && getline(file,line)){
			lineNo++;
			//std::cout<<line<<std::endl;
			//std::cout<<"trainNo = "<<lineNo<<std::endl;
			
    			const char* comma = ",";
    			size_t pos = line.find('\0');
    			line[pos] = '\0';

    			char* field = strtok(const_cast<char*>(line.c_str()), comma);

    			while (field != nullptr) {
				
    				//std::cout<<count++<<" : "<< field<<std::endl;
				if(count == 0){
					_index = atoi(field);
				}
				else if(count == 1){
					strncpy(_date,field,20);
				}
				else if(count == 2){
					_handover = atof(field);
				}
				else if(count == 3){
					_DRB_RlcDelayUL = atof(field);
				}
				else if(count == 4){
					_DRB_AirlfDelayUL = atof(field);
				}
				else if(count == 5){
					_DRB_RlcSduDelayDL = atof(field);
				}
				else if(count == 6){
					_DRB_AirlfDelayDL = atof(field);
				}
				else if(count == 7){
					_total_delay = atof(field);
				}
    			    	count += 1;
				field = strtok(nullptr, comma);
    			}
			count = 0;

			std::cout<<_index<<",";
			std::cout<<_date<<",";
			std::cout<<_handover<<",";
			std::cout<<_DRB_RlcDelayUL<<",";
			std::cout<<_DRB_AirlfDelayUL<<",";
			std::cout<<_DRB_RlcSduDelayDL<<",";
			std::cout<<_DRB_AirlfDelayDL<<",";
			std::cout<<_total_delay;
			std::cout<<std::endl;
			add_data(conn,train,checkcount,_index,_date,_handover,_DRB_RlcDelayUL,_DRB_AirlfDelayUL,_DRB_RlcSduDelayDL,_DRB_AirlfDelayDL,_total_delay);
			checkcount++;
		}
		checkcount = 0;
		//std::cout<<"trainEnd"<<std::endl;

        	file.close();
    	}
    	else std::cout << "Unable to open file '"<<fileName<<"'"; 
	lineNo = 0;
	return 0;
}

// WFM //
int FileProcessor::Delete_row(std::string fileName, std::string outFileName, int targetRow) {
	std::string line;
    	std::ifstream file (fileName);
    	std::string token;
    	lineNo = 0;
   	if(targetRow == 0){
		return 0;
	}
	else if(targetRow < 0 || targetRow > Show_p_row()){
		std::cout<<"Error in row range"<<std::endl;
		return 0;
	}
    	std::ofstream out_file;
    	out_file.open(outFileName,std::ofstream::out | std::ofstream::trunc);
    
    	if (file.is_open()) {
        	while ( getline (file,line) ) {
			lineNo++;
			if(lineNo == targetRow)
				continue;
			else{
               			try{
                    			std::stringstream ss(line);
                    			if(std::getline(ss, token, '\n')){
						while(std::getline(ss ,token , ','))
							out_file<<token<<",";
                        			out_file<<token<<"\n";
		     			}	
                		} catch (std::exception& e) {
                    			std::cout<<std::endl<<"Delete_row_Error in line "<<lineNo<<": "<<e.what()<<std::endl;
                		}  
			}
            	}
        	file.close();
    	}
    	else std::cout << "Unable to open file '"<<fileName<<"'"; 
	Set_row(Show_p_row() -1 );
    	return 0;
}


///////////////////////////////////////////////F   i   n////////////////////////////////////////////////////

std::vector<double> FileProcessor::read(std::string fileName, int valuesPerLine) {
    
    	std::vector<double> values;
    	std::string line;
    	std::ifstream file (fileName);
    	lineNo = 0;
    	if (file.is_open()) {
        	if (valuesPerLine == 1)  {
            		while ( getline (file,line) ) {
                		lineNo++;
                		try{
                    			values.push_back(std::stod(line));
                		} catch (std::exception& e) {
		    			std::cout<<std::endl<<fileName<<","<<line<<std::endl;
                    			std::cout<<std::endl<<"1 Error in line "<<lineNo<<": "<<e.what()<<std::endl;
                		}    
            		}
        	}
       		file.close();
    	}
    	else std::cout << "Unable to open file '"<<fileName<<"'"; 
    	return values;
    
}

std::vector<double> * FileProcessor::readMultivariate(std::string fileName, int lines, int variables, int * inputCols, int targetValCol) {
    
    	std::string line;
    	std::ifstream file (fileName);
    	std::string token;
    	tokenNo = 0;
    	lineNo = 0;
    
    	std::vector<double> target;
    
    	std::vector<double> * data;
    	data = new std::vector<double>[lines+1];
    
    	if (file.is_open()) {
        	while ( getline (file,line) ) {
            		std::vector<double> input;
            		lineNo++;
            		try{
                		std::stringstream ss(line);
                		tokenNo = 0;
                		while(std::getline(ss, token, ',')) {
                    			if (tokenNo == targetValCol) {
                        			target.push_back(std::stod(token));
                    			} else if (inputCols[tokenNo] == 1) {
                        			input.push_back(std::stod(token));
                    			}
                    			tokenNo++;
                		}
                		data[lineNo-1] = input;
            		} catch (std::exception& e) {
                		std::vector<double> input (variables-1,0.0);
                		data[lineNo-1] = input;
                		target.push_back(0.0);
                		std::cout<<std::endl<<fileName<<","<<line<<std::endl;
                		std::cout<<std::endl<<"22Error in line "<<lineNo<<": "<<e.what()<<std::endl;
            		}    
            		if (lineNo == lines) break;
  		}
        	file.close();
        	data[lines] = target;
    	}
    	else std::cout << "Unable to open file '"<<fileName<<"'"; 
    	return data;
    
}

int FileProcessor::write(std::string fileName) {
    	out_file.open(fileName,std::ofstream::out | std::ofstream::app);
    	return 0;
}

int FileProcessor::append(std::string line) {
    	out_file<<"";
    	return 0;
}
//Fin
int FileProcessor::Csv_to_Txt(std::string fileName,std::string outFileName) {
   
	/*declaration*/
    	std::string line;
    	std::ifstream file (fileName);
    	std::string token;
    	tokenNo = 1;
    	lineNo = 0;
    	std::ofstream out_file;
    	/*open choice*/
    	out_file.open(outFileName,std::ofstream::out | std::ofstream::trunc);
    
    	if (file.is_open()) {
		getline(file,*headerline);
		for(char count : *headerline)
			if(count ==',')
				tokenNo++;
		
		/*Clear the contents of string*/
		//headerline->clear();
		/*Memory free for string headerline*/
		//delete headerline;
            	while ( getline (file,line) ) {
                	lineNo++;
			//std::cout<<line<<std::endl;
               		try{
                    	std::stringstream ss(line);
		    	//std::cout<<"line no.= "<<lineNo<<std::endl<<"line = "<<line<<std::endl;
                		if(std::getline(ss, token, '\n')){
                       			//if (tokenNo == columnIndx) {
					//std::cout<<token;
					while(std::getline(ss ,token , ',')){
						out_file<<token<<",";
						std::cout<<token;
					}
                        		out_file<<token<<"\n";
		    		}		
                	} catch (std::exception& e) {
                    	std::cout<<std::endl<<"33Error in line "<<lineNo<<": "<<e.what()<<std::endl;
                	}   
            	}
        	file.close();
    	}
    	else std::cout << "Unable to open file '"<<fileName<<"'"; 
    
    	//std::cout<<"TokenNo = "<<tokenNo<<std::endl;
    	//std::cout<<"LineNo = "<<lineNo<<std::endl;
    	Set_row(lineNo);
    	Set_col(tokenNo);

    	return 0;   
}
