#include "fileHandler.h"

// Quick and dirty. Just declare this and move on with our lives.
QString loadData()
{
  QString filepath = "myMemory";
  QFileInfo check_file(filepath);
  QString payload = "0 0 0";

  // Make sure file exists
  if (check_file.exists() && check_file.isFile())
  {
    qDebug() << "FILE EXISTS";

    QFile file(filepath);
    // if the file won't open - can prolly get rid of thid
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    // Read everything in, split it by spaces and then print it line by line
    // @todo - push values into the spinnerboxes
    while(!in.atEnd()) {
      
        payload = in.readLine();    
        QStringList fields = payload.split(" ");    
        
        foreach (QString v, fields)
        {
          qDebug() << v;
        }
    }

    file.close();
  }
  else
  {
    // If no file exists, then create one and populate it with file data
    qDebug() << "NO FILE HERE";
    // Default Case
    QFile file(filepath);
    if (file.open(QIODevice::ReadWrite))
    {
      QTextStream stream (&file);
      stream << payload <<endl;
    }
  }
  
  return payload;
}

double loadData(QString filepath)
{
  QFileInfo check_file(filepath);
  QString payload = "0";

  // Make sure file exists
  if (check_file.exists() && check_file.isFile())
  {
    qDebug() << "FILE EXISTS";

    QFile file(filepath);
    // if the file won't open - can prolly get rid of thid
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    // Read everything in, split it by spaces and then print it line by line
    // @todo - push values into the spinnerboxes
    while(!in.atEnd()) {
      
        payload = in.readLine();   
        qDebug() << filepath << " : " << payload;
    }

    file.close();
  }
  else
  {
    // If no file exists, then create one and populate it with file data
    qDebug() << "NO FILE HERE";
    // Default Case
    QFile file(filepath);
    if (file.open(QIODevice::ReadWrite))
    {
      QTextStream stream (&file);
      stream << payload <<endl;
    }
  }
  
  bool ok = false;
  return payload.toDouble(&ok);
}

// Again, quick and dirty.
void saveData(QString data)
{
    QString filepath = "myMemory";
    QFile file(filepath);
    if (file.open(QIODevice::ReadWrite))
    {
      QTextStream stream (&file);
      stream << data <<endl;
    }
}

void saveData(QString fileName, double data)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite))
    {
      QTextStream stream (&file);
      stream << data <<endl;
    }
}