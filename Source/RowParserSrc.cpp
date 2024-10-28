#include "../Headers/FileOperations/RowParser.hpp"


RowParser::RowParser(){}
RowParser::~RowParser(){}

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

std::vector<double> RowParser::getParsedRow()
{
    return parsedRow;
}



