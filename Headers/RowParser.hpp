#include <iostream>
#include <sstream>
#include <vector>




class RowParser
{
    public:

    RowParser();
    ~RowParser();
    void ConvertRow(std::string, char);
    std::vector<double> getParsedRow();
    

    private:
    std::vector<double> parsedRow;
};