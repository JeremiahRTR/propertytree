#include "DataBall.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

DataBall::DataBall()
{
  dataStored = "localData";
  // Initialize variables
  
  // File Inputs
  robotURDFPath = "/home/robo/robo1.urdf";
  nodesFilePath = "/home/robo/robo1.nodes";
  edgesFilePath = "/home/robo/robo1.edges";
  
  // Viewing
  viewMode << "Corner" << "Front" << "Top" << "Bottom";
  linkMeshMode << "Mode A" << "Mode B" << "Mode C";
  voxelVisible = true;
  index = 0;
  entryReadOnly = "Read Only Entry";
  
  // Voxel Region
  loadRegion = "/home/robot/robo1_region1.reg";
  regionVisible = true;
  voxelFrame << "Blue" << "Green" << "Black";
  positionVec3.x = 1.0;
  positionVec3.y = 2.0;
  positionVec3.z = 3.0;
  dimensionsVec3.x = 4.0;
  dimensionsVec3.y = 5.0;
  dimensionsVec3.z = 6.0;
  voxelDimensionsReadOnly = "6 Dimensions of freedom";
  
  baseLinkFrame << "Frame 1" << "Frame 2" << "Frame 3";
  endEffectorFrame << "Open" << "Closed";
  
  threads = 8;
}

DataBall::~DataBall()
{
  
}

// This is a dumb function in that it only does one thing to one place
// It's an example function.
void DataBall::LoadData()
{
  QFileInfo check_file(dataStored);

  // Make sure file exists
  if (check_file.exists() && check_file.isFile())
  {

    QFile file(dataStored);
    QTextStream in(&file);

    while(!in.atEnd()) {
      
        if (in.readLine() == "'FileInput")
        {
          robotURDFPath = in.readLine();
          nodesFilePath = in.readLine();
          edgesFilePath = in.readLine();
        }
        if (in.readLine() == "'Viewing")
        {
          viewMode      = in.readLine().trimmed().split(" ");
          linkMeshMode  = in.readLine().trimmed().split(" ");
          voxelVisible  = (in.readLine() == "true");
          index         = in.readLine().toInt();
          entryReadOnly = in.readLine();
        }
        if (in.readLine() == "'VoxelRegion")
        {
          loadRegion    = in.readLine();
          regionVisible = (in.readLine() == "true");
          voxelFrame    = in.readLine().trimmed().split(" ");
          QStringList temp = in.readLine().trimmed().split(" ");
          positionVec3.x = temp[0].toFloat();
          positionVec3.y = temp[1].toFloat();
          positionVec3.z = temp[2].toFloat();
          
          temp = in.readLine().trimmed().split(" ");
          dimensionsVec3.x = temp[0].toFloat();
          dimensionsVec3.y = temp[1].toFloat();
          dimensionsVec3.z = temp[2].toFloat();
          
          voxelDimensionsReadOnly = in.readLine();
        }
        if (in.readLine() == "'LinkPoses")
        {
          baseLinkFrame   = in.readLine().trimmed().split(" ");
          endEffectorFrame= in.readLine().trimmed().split(" ");
        }
        threads = in.readLine().trimmed().toInt();
    }

    file.close();
  }
  else
  {
    // If no file exists, then create one and populate it with file data
    //~ qDebug() << "NO FILE HERE" << endl;
    SaveData();
  }
  
}


// Another dumb function - saves the data that it's got
void DataBall::SaveData()
{
  
  QFile file(dataStored);
  if (file.open(QIODevice::ReadWrite))
  {
    QTextStream stream (&file);
    
    // Store the File Inputs
    stream <<"'FileInput" << endl;
    stream << robotURDFPath <<endl;
    stream << nodesFilePath <<endl;
    stream << edgesFilePath <<endl;
    
    stream << "'Viewing" << endl;
    for (QString r : viewMode)
      stream << r << " ";
    stream << endl;
    for (QString r : linkMeshMode)
      stream << r << " ";
    stream << endl;
    stream << voxelVisible <<endl;
    stream << index <<endl;
    stream << entryReadOnly <<endl;
    
    stream << "'VoxelRegion" << endl;
    stream << loadRegion << endl;
    stream << regionVisible << endl;
    for (QString r : voxelFrame)
      stream << r << " ";
    stream << endl;
    stream << positionVec3.x << " " << positionVec3.y << " " << positionVec3.z << endl;
    stream << dimensionsVec3.x << " " << dimensionsVec3.y << " " << dimensionsVec3.z << endl;
    stream << voxelDimensionsReadOnly<< endl;
    
    stream << "'LinkPoses" << endl;
    for (QString r : baseLinkFrame)
      stream << r << " ";
    stream << endl;
    for (QString r : endEffectorFrame)
      stream << r << " ";
    stream << endl;
    
    stream << threads << endl;
  }
  
}