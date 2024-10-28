#include "../Headers/FileOperations/RowParser.hpp"

//Constructor
RowParser::RowParser(){}
//Destructor
RowParser::~RowParser(){}


/**
 * Convert the row in string form to vector and saves it internally
 * 
 * @param input The row in string form
 * @param delimiter The delimiter of data as character 
 */
void RowParser::ConvertRow(std::string input, char delimiter)
{
    std::stringstream ss(input);
    std::string buf;
    std::vector<double> output;


    while(std::getline(ss, buf, delimiter)){
        try{
            output.push_back(std::stod(buf));
        } catch (std::invalid_argument){
            output.push_back(-65535);
        }
    }
    parsedRow = output;
}


/**
 * Get the parsed row
 * 
 * @return Parsed row as a double datatype vector for usage
 */
std::vector<double> RowParser::getParsedRow()
{
    return parsedRow;
}



