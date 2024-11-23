#include "../Headers/FileOperations/RowParser.hpp"

//Constructor
RowParser::RowParser(){}
//Destructor
RowParser::~RowParser(){}


/**
 * @fn void RowParser::ConvertRow(const std::string& input, char delimiter)
 * @brief Convert the row in string form to vector and saves it internally
 * 
 * @param input The row in string form
 * @param delimiter The delimiter of data as character 
 */
void RowParser::ConvertRow(const std::string& input, char delimiter)
{
    std::stringstream ss(input);
    std::string buf;
    std::vector<double> output;


    while(std::getline(ss, buf, delimiter)){
        try{
            output.push_back(std::stod(buf));
        } catch (std::invalid_argument){
            output.push_back(-65535.0);
        }
    }
    parsedRow = output;
}


/**
 * @fn void RowParser::ConvertRow(const std::string& input, char delimiter)
 * @brief Get the parsed row
 * 
 * @return Parsed row as a double datatype vector for usage
 */
const std::vector<double>& RowParser::getParsedRow()
{
    return parsedRow;
}



