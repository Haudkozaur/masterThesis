//
// Created by Julian Haudek on 10/02/2024.
//

#ifndef MASTERTHESIS_DATABASECROSSSECTIONSMANAGER_H
#define MASTERTHESIS_DATABASECROSSSECTIONSMANAGER_H


#include "DataBaseModelObjectsManager.h"
namespace DataBaseManagers {
class DataBaseCrossSectionsManager : public DataBaseModelObjectsManager{

public:
    explicit DataBaseCrossSectionsManager(const string& dateBaseName);

    void addObjectToDataBase(const string &name, int materialID, double A, double I);
    void iterateOverTable();

    std::map<int, std::string> crossSectionsMap;
    std::map<int, std::string> getCrossSectionsMap() const;
};
}

#endif //MASTERTHESIS_DATABASECROSSSECTIONSMANAGER_H
