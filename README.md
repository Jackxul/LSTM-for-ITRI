LSTM model and backend written in C++(for ITRI)
---
# Main.cpp
##### Variable
    * Mode
        - the boolean variable that will decide what datarec() be doing (table creation/table clean up)
    * API_Mode
        - decide how to handle API request(to void direct access)
    * datarec_size
        - the number that datarec() can handle per time
    * filerow
        - total row of the data (can replace by the function in FileProcessor.cpp (Show_p_row()))
    * init_page
        - initial page setting
    * page_size
        - setting the data number that can be displayed per time in JSON format
    * pgno
        - total page number
    + arrayforsplit[][]
        - array for saving and access partition rate for the data(100%)
          {{validation data , test data , train data},
          {validation data , test data , train data}}
          etc....
    * _gNb_No
        - Base station number
#### Functions
    + msq()
        - Not Done Yet
    + dataconvert()
        - data convert(data split ,  delete the title row , insert data to table(s))
    + datarec()
        - data recive(recive data from database, according to the api request then handle and recive certain data from database)
    + multivarPredicts()
        - LSTM prediction
---
# FileProcessor.cpp
#### Variable
    * lineNo
    * tokenNo
    * headerline
#### Functions
    + connect_db() start connect to database
    + close_db() end the connection to database
    + create_table(conn , tableNo , df) create table(s) in database
        * tableNo
            - the number behind tableName
        * df
            - boolean variable for table create
    + clean_table(conn , startNo , endNo , df) clean table(s) in database
        * startNo
            - start number for which table to be cleaned
        * endNo
            - end number for which table is the last to be clean
        * df
            - boolean variable for table clean
    + add_data(conn , tableName , checkcount , line_id , etc...) insert data in table
        * tableName
            - the name of table that is gonna add data in it
        * checkcount
            -  the variable for checking how many data is in one single table (For provide API INFO)
        * line_id
            - row number to store in database 
        * etc....
            - the data that need to be stored in database

    + Set_col(colval) setting column number (using mutex lock)
        * colval
            - column number
    + Set_row(rowval) setting row number (using mutex lock)
        * rowval
            - row number
    + Show_p_row() Display or set the file row number to main.cpp
    + Show_p_col() Display or set the file column number to main.cpp
    + Delete_column() Not Done Yet
    + Split_txt(conn , gNbNo , fileName , testv , valv , trainv , df) For file split into three data(train , test and validation) and store in database
        * gNbNo
            - the gNb numbeur for adding data into certain database table
        * fileName
            - the file name that need to be split
        * testv 
            - the partition value of test data
        * valv
            - the partition value of validation data
        * tarinv
            - the partition value of tarin data
        * df
    + Delete_row(fileName , outFileName , targetRow) delete certain row in certain file 
        * fileName
            - targetfile
        * outFileName
            - outputfile
        * targetRow
            - target row need to be delete
    + read(fileName , valuesPerLine) read file's data
        * fileName
            - the target file name
        * valuesPerLine
            - If there is values in the current Line
    + readMultivariate() LSTM prediction
    + Csv_to_Txt(fileName , outFileName) Convert csv file to text file
        * fileName 
            - target file name
        * outFileName
            - output file name
