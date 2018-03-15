#ifndef DATABALL_H
#define DATABALL_H

#include "Vec3.h"
#include <QString>
#include <QStringList>

// This class will store all of the data of the project.
// Anything which is relevant which should be shown to the user should be here first, or at least stored here
class DataBall {
  
  
  public: 
    
    DataBall();
    ~DataBall();
    
    void LoadData();
    void SaveData();
    
    QString dataStored;
  
    // FileInputs
    QString robotURDFPath;
    QString nodesFilePath;
    QString edgesFilePath;
    
    // Viewing
    QStringList viewMode;
    QStringList linkMeshMode;
    bool voxelVisible;
    int index;
    QString entryReadOnly;
    
    //Voxel Region
    QString loadRegion;
    bool regionVisible;
    QStringList voxelFrame;
    Dash::Vec3 centerVec3;
    Dash::Vec3 dimensionsVec3;
    QString voxelDimensionsReadOnly;
    
    //Link Poses
    QStringList baseLinkFrame;
    QStringList endEffectorFrame;
    
    int threads;
  
};


#endif // DATABALL_H