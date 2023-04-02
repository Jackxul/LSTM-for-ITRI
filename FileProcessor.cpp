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


int FileProcessor::Set_col(int colval){
	mtx.lock();
	p_col = colval;
	std::cout<<"colval = "<<colval<<std::endl;
	std::cout<<"p_col = "<<p_col<<std::endl;
	mtx.unlock();
	return p_col;
}
int FileProcessor::Set_row(int rowval){
	p_row = rowval;
	std::cout<<"Rowval = "<<rowval<<std::endl;
	std::cout<<"p_row = "<<p_row<<std::endl;
	return p_row;
}
int FileProcessor::Delete_column(std::string fileName, std::string outFileName, int targetCol) {
	
    std::string line;
    std::ifstream file (fileName);
    std::string token;
    int tokenNo = 0;
    
    int lineNo = 0;
    
    std::ofstream out_file;
    out_file.open(outFileName,std::ofstream::out | std::ofstream::trunc);
    
    if (file.is_open()) {
            while ( getline (file,line) ) {
               	try{
                    std::stringstream ss(line);
		    for(char count : line){
			if(count == ',')
				tokenNo++;
		    }
                    if(std::getline(ss, token, '\n')){
                       // if (tokenNo == columnIndx) {
			//std::cout<<token;
			//std::cout<<token<<std::endl;


			//while(std::getline(ss,token,',')){
			//	std::cout<<token<<std::endl;
			//	tokenNo++;
			//}
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
			//	out_file<<token<<",";
                        out_file<<token<<"\n";
                       // }
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
int FileProcessor::Delete_row(std::string fileName, std::string outFileName, int targetRow) {
	

	return 0;
}
std::vector<double> FileProcessor::read(std::string fileName, int valuesPerLine) {
    
    std::vector<double> values;
    std::string line;
    std::ifstream file (fileName);
    int lineNo = 0;
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
    int tokenNo = 0;
    int lineNo = 0;
    
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

int FileProcessor::Csv_to_Txt(std::string fileName, std::string outFileName) {
    
    std::string line;
    std::ifstream file (fileName);
    std::string token;
    int tokenNo = 0;
    int lineNo = 0;
    
    std::ofstream out_file;
    out_file.open(outFileName,std::ofstream::out | std::ofstream::trunc);
    
    if (file.is_open()) {
            while ( getline (file,line) ) {
                lineNo++;
		//std::cout<<line<<std::endl;
               	try{
                    std::stringstream ss(line);
		    std::cout<<"line no.= "<<lineNo<<std::endl<<"line = "<<line<<std::endl;
                   if(std::getline(ss, token, '\n')){
                       // if (tokenNo == columnIndx) {
			//std::cout<<token;
			while(std::getline(ss ,token , ',')){
				out_file<<token<<",";
			}
                        out_file<<token<<"\n";
                       // }
			}	
                } catch (std::exception& e) {
                    std::cout<<std::endl<<"33Error in line "<<lineNo<<": "<<e.what()<<std::endl;
                }   
            }
        file.close();
    }
    else std::cout << "Unable to open file '"<<fileName<<"'"; 
    
    std::cout<<"TokenNo = "<<tokenNo<<std::endl;
    std::cout<<"LineNo = "<<lineNo<<std::endl;
    return 0;   
}
