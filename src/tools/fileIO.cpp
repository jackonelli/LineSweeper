#include "json.hpp"
#include <string>
#include <fstream>
//#include <sstream>
//#include <iostream>
#include <iomanip>
using json = nlohmann::json;

namespace fileIO {

json ReadJsonFile(const std::string filePath){
  // TODO: Verify file read
  std::ifstream jsonFile(filePath.c_str());
  json json_data;
  jsonFile >> json_data;
  jsonFile.close();
  return json_data;
}

void WriteJsonToFile(const json *graphData, const std::string filePath){
  // TODO: Verify file read
  std::ofstream jsonFile(filePath.c_str());
  jsonFile << std::setw(4) << *graphData << std::endl;
  jsonFile.close();
}

}
