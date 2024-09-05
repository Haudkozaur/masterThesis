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
    //void writeLoads(std::ofstream &script);
    void writeMesh(std::ofstream &script);
    void writeSolveSection(std::ofstream &script);
    void writeFiniteElementSpace(std::ofstream &script);
    void writeMeshBorders(std::ofstream &script);


    void applyPointLoads(std::ofstream &script);
    void definePointLoads(std::ofstream &script);
    void applySurfaceLoads(std::ofstream &script);
    void defineLineLoads(std::ofstream &script);
    void applyLineLoads(std::ofstream &script);
    void defineSurfaceLoads(std::ofstream &script);
    void applySurfaceSupports(std::ofstream &script);
    void defineSurfaceSupports(std::ofstream &script);

};
