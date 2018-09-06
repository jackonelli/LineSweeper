#include "json.hpp"
#include <string>
#include <fstream>
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

}
