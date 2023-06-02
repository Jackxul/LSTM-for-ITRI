/* 
 * File:   FileProcessor.h
 * Author: heshan
 *
 * Created on April 14, 2018, 7:45 PM
 */

#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include "mysql/mysql.h"
class FileProcessor {
	public:
	    FileProcessor();
	    FileProcessor(const FileProcessor& orig);
	    virtual ~FileProcessor();
	    
	    /**
	     * 
	     * @param fileName name of the input file
	     * @param valuesPerLine number of values in line
	     * @return vector of values
	     */
	    std::vector<double> read(std::string fileName, int valuesPerLine);
	    /**
	     * Read a text file containing comma separated values.
	     * 
	     * @param fileName file name of the multivariate data series
	     * @param lines no of lines that should be read
	     * @param variables variable in the file
	     * @param inputCols index of the variable in a line that should used to create the vector
	     * @param targetValCol index of the target value column
	     * @return an array of vectors
	     * 
	     * each vector contain variables in one row
	     * last array vector is the target vector 
	     */
	    std::vector<double> * readMultivariate(std::string fileName, int lines, int variables, int * inputCols, int targetValCol);
	    /**
	     * 
	     * @param fileName
	     * @return 
	     */
	    int write(std::string fileName);
	    /**
	     * 
	     * @param line
	     * @return 
	     */
	    int append(std::string line);
	    /**
	     * 
	     * @param fileName: input filename
	     * @param outFileName: output file name
	     * @param valuesPerLine: values in a line in the input file
	     * @param columnIndx: variable written to the output file
	     * @return 0
	     */
	    void connect_db(MYSQL*& conn);
	    
	
	    void close_db(MYSQL*& conn);
	
	    void create_table(MYSQL*& conn, std::string tableName);

	    
	    void add_data(MYSQL*& conn , std::string tableName , int line_id , char *date , float handover , float DRB_RlcDelayUL , float DRB_AirlfDelayUL , float DRB_RlcSduDelayDL , float DRB_AirlfDelayDL , float total_delay);
	    
	    int Split_txt(MYSQL*& conn , int gNbNo , std::string fileName, std::string trainFileName , std::string testFileName , std::string valFileName , float trainv , float testv , float valv );
	    /*
	     *trainFileName = Training File
	     *testFileName = Datatest File
	     *valFileName = Datatest Fil
	     *Split the text file after convert into three part(trainv , testv , valv )
	     *trainv + testv + valv = 1
	     *Jack_Xul
	     * */
	    int Csv_to_Txt(std::string fileName, std::string outFileName);
	   	/*
		 *input csv file
		 *output text file
		 *use getline to read and write
		 *Jack_Xul
		 * */ 
	    int Delete_column(std::string fileName, std::string outFileName , int targetCol);
		/*
		 *input text file
		 *output text file
		 *targetCol = The number of Column that want to delete.
		 *Jack_Xul
		 * */
	    int Delete_row(std::string fileName, std::string outFileName , int targetRow);
		/*
		 *input text file
		 *output text file
		 *targetRow = The number of Row that want to delete.
		 *Jack_Xul
		 * */
	    void Set_row(int rowval);
		/*
		 *Set private row value
		 *Jack_Xul
		 * */
	    void Set_col(int colval);
		/*
		 *Set private column value
		 *Jack_Xul
		 * */
	    int Show_p_row();
	    	/*
		 *Return p_row
		 *Jack_Xul
		 * */
	    int Show_p_col();
	    	/*
		 *Return p_col
		 *Jack_Xul
		 * */
	private:
	    std::mutex mtx;
	
	    std::ofstream out_file;
	    unsigned int p_row;
	    unsigned int p_col;
};

#endif /* FILEPROCESSOR_H */

