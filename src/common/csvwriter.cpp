#include "csvwriter.h"

CSVWriter::CSVWriter(const std::vector<std::string>& columns, const std::string& file_path) :
    m_columns(columns), m_sstream(), m_fstream(file_path), m_csv_writer(m_fstream)
{
    m_csv_writer << m_columns;
}

void CSVWriter::write_row(const std::vector<std::string> &row)
{
    m_csv_writer << row;
    m_fstream << m_sstream.str();
    m_sstream.str(std::string());
}
