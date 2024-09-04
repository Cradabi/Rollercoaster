#pragma once
#include <QVector>
#include <QFile>
#include <utility>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QCoreApplication>
#include <iostream>
#include <QVector>
#include <QString>
#include <utility>
#include <string>

QVector<float> movingAverage(const QVector<float>& input, int windowSize);
QVector<std::pair<int, int>> findPeaks(const QVector<float>& data, float floorLevel, int minPeakWidth, int maxPeakWidth);
std::string search(QString inputFileName, QString outputFileName, int minPeakWidth, int maxPeakWidth);
