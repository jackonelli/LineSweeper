#include "AntSystem.h"
AntSystem::AntSystem(const char * nodeFileName, const char * edgeFileName, const int numberOfAnts, const float alpha, const float beta)
  : graph(nodeFileName, edgeFileName), numberOfAnts_(numberOfAnts), alpha_(alpha), beta_(beta) {
};
AntSystem::~AntSystem(){
};
