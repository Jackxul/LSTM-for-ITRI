/* 
 * File:   FileProcessor.cpp
 * Author: heshan
 * Modify: Jack_Xul --> 2022/03/30
 * Created on April 14, 2018, 7:45 PM
 */

#include <vector>
#include <exception>
#include <sstream>
#include "FileProcessor.h"

FileProcessor::FileProcessor() { }

FileProcessor::FileProcessor(const FileProcessor& orig) { }

FileProcessor::~FileProcessor() { }


int lineNo;
int tokenNo;
std::string *headerline = new std::string;

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
	
	tokenNo = 0;
    	std::string line;
   	std::ifstream file (fileName);
    	std::string token;
    
    	lineNo = 0;
    
    	std::ofstream out_file;
    	out_file.open(outFileName,std::ofstream::out | std::ofstream::trunc);
    
    	if (file.is_open()) {
            	while ( getline (file,line) ) {
               		try{
                    		std::stringstream ss(line);
                    		if(std::getline(ss, token, '\n')){
                       			if((targetCol < 0) || (tokenNo < targetCol)){
						std::cout<<"tokenNo = "<<tokenNo<<std::endl;
						std::cout<<"Error in target column"<<std::endl;
						return 0;
					}
					//std::cout<<"Token reset"<<std::endl;
					tokenNo = 0;
					while(std::getline(ss ,token , ',')){
						if(tokenNo == targetCol){
							tokenNo++;
						}
						else{
							out_file<<token<<",";
							tokenNo++;
						}
				
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
    	return 0;   
}

// NDY //
// 1:4:5 = datatest : datatest2 : datatraining = valFile : testFile : trainFile
int FileProcessor::Split_txt(std::string fileName, std::string trainFileName , std::string testFileName , std::string valFileName , float trainv , float testv , float valv ){
	if((trainv + testv + valv)!= 1.0){
		std::cout<<"Split Proportion Error"<<std::endl;
		return 0;
	}
	/*
	 * testNo
	 * valNo
	 * trainNo
	 * */
	int trainNo = Show_p_row();
	int testNo = (Show_p_row() * testv);
	int valNo = (Show_p_row() * (testv + valv));
	std::string line;

    	std::ifstream file (fileName);


	out_file.open(trainFileName,std::ofstream::out | std::ofstream::trunc);
	out_file.open(testFileName,std::ofstream::out | std::ofstream::trunc);
	out_file.open(valFileName,std::ofstream::out | std::ofstream::trunc);
	
	

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
