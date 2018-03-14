
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <QMessageBox>

QString loadData();
double loadData(QString filepath);

void saveData(QString data);
void saveData(QString fileName, double data);

#endif