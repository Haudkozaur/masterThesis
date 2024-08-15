#include "DataBaseResultsManager.h"
#include "TableTypes.h"
#include "toStringWithPrecision.h"

namespace DataBaseManagers {

DataBaseResultsManager::DataBaseResultsManager(std::string dateBaseName)
    : DataBaseModelObjectsManager(std::move(dateBaseName)) {

}


void DataBaseManagers::DataBaseResultsManager::addObjectToDataBase(int nodeId, double XCord, double ZCord, double Nx, double Vz, double My, double deformation)
{
    // if (checkIfDuplicate(TableType::POINTS, std::make_tuple(XCoordinate, ZCoordinate, 0))) {
    //     std::cout << "Error: Point already exists in DB" << std::endl;
    //     return;
    // }

    // std::string QueryInsertPoint = "INSERT INTO points (id, x_cord, z_cord) VALUES (NULL, "
    //                                + std::to_string(XCoordinate) + ", " + std::to_string(ZCoordinate) + ")";

    // executeAndCheckIfSQLOk(QueryInsertPoint, TableType::POINTS);

    // if (checkIfDuplicate(TableType::RESULTS, std::make_tuple(nodeId, XCord, ZCord))) {
    //     std::cout << "Error: Result already exists in DB" << std::endl;
    //     return;
    // }

    std::string queryAddNodeResult = "INSERT INTO " + tableTypesMap.at(TableType::RESULTS)
                         + " (node_id, x_cord, z_cord, Nx, Vz, My, deformation) VALUES (" + std::to_string(nodeId) + ", "
                         + toStringWithPrecision(XCord) + ", " + toStringWithPrecision(ZCord) + ", "
                         + toStringWithPrecision(Nx) + ", " + toStringWithPrecision(Vz) + ", "
                         + toStringWithPrecision(My) + ", " + toStringWithPrecision(deformation) + ")";
    executeAndCheckIfSQLOk(queryAddNodeResult, TableType::RESULTS);
}


void DataBaseManagers::DataBaseResultsManager::iterateOverTable()
{
    std::string querySelectAllResults = "SELECT * FROM " + tableTypesMap.at(TableType::RESULTS);
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllResults);

    std::cout << "we are here" << std::endl;

    resultsMap.clear();

    if (results.empty()) {
        std::cerr << "Error: No results returned from query." << std::endl;
        return;
    }

    for (const auto& row : results) {
        std::cout << "Row size: " << row.size() << std::endl;

        if (row.size() == 7) {
            try {
                int id = std::stoi(row[0]);
                double xCord = std::stod(row[2]);
                double zCord = std::stod(row[3]);
                double Nx = std::stod(row[4]);
                double Vz = std::stod(row[5]);
                double My = std::stod(row[6]);

                // Instead of parsing deformation, assign a default value of 0.0
                double deformation = 0.0;

                std::cout << "Parsed ID: " << id << " xCord: " << xCord << " zCord: " << zCord
                          << " Nx: " << Nx << " Vz: " << Vz << " My: " << My
                          << " Deformation: " << deformation << std::endl;

                // Store in resultsMap with all fields, using default deformation value
                resultsMap[id] = std::make_tuple(xCord, zCord, Nx, Vz, My, deformation);

            } catch (const std::exception& e) {
                std::cerr << "Error during parsing: " << e.what() << std::endl;
                continue; // Skip this row if there was an error
            }
        } else {
            std::cerr << "Error: Unexpected row size. Expected 7, got " << row.size() << std::endl;
        }
    }
}


std::map<int, std::tuple<double, double, double, double, double, double> > DataBaseManagers::DataBaseResultsManager::getResultsMap() const
{
    return resultsMap;
}

}
