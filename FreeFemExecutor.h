#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "DataBaseManagers/DataBaseFreeFEMPreparer.h"

class FreeFemExecutor {
public:
    FreeFemExecutor(const DataBaseManagers::DataBaseFreeFEMPreparer &dataPreparer);

    void generateFreeFemScript(const std::string &scriptPath);
    void executeFreeFemScript(const std::string &scriptPath);
    void parseResults(const std::string &resultPath);

    void displayMeshedGeometry();
    void runAnalysisAndDisplayResults();
private:
    const DataBaseManagers::DataBaseFreeFEMPreparer &dataPreparer;

    void writeGeometry(std::ofstream &script);
    void writeMaterialProperties(std::ofstream &script);
    void writeBoundaryConditions(std::ofstream &script);
    void writeLoads(std::ofstream &script);
    void writeMesh(std::ofstream &script);
    void writeSolveSection(std::ofstream &script);
    void writeFiniteElementSpace(std::ofstream &script);
};
