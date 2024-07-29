#include "DataBaseModelObjectsManager.h"
#ifndef MASTERTHESIS_DATEBASEMATERIALSMANAGER_H
#define MASTERTHESIS_DATEBASEMATERIALSMANAGER_H


class DataBaseMaterialsManager : public DataBaseModelObjectsManager{


public:
    explicit DataBaseMaterialsManager(const string& dateBaseName);
    void iterateOverTable();
    void addObjectToDataBase(const string &name, double E, double v, double ro);
    std::map<int, std::string> getMaterialsMap() const;

private:
    std::map<int, std::string> materialsMap; // To store materials with their IDs


    //TODO: implement deleteObjectFromDataBase after making decision what to do with cross section with material that is deleted
    //void deleteObjectFromDataBase(int id);

};
#endif //MASTERTHESIS_DATEBASEMATERIALSMANAGER_H
