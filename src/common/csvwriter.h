#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "libs/csv/single_include/csv.hpp"

class CSVWriter
{
public:
    CSVWriter(const std::vector<std::string>& columns, const std::string& file_path);

    void write_row(const std::vector<std::string>& row);

private:
    std::vector<std::string> m_columns;
    std::stringstream m_sstream;
    std::ofstream m_fstream;
    csv::CSVWriter<std::ofstream> m_csv_writer;
};
