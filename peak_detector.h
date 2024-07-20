#pragma once

#include <QVector>
#include <utility>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QCoreApplication>

QVector<float> medianFilter(const QVector<float>& input, int windowSize);
QVector<std::pair<int, int>> findPeaks(const QVector<float>& data, float floorLevel, int minPeakWidth, int maxPeakWidth);
int search(QString inputFileName, QString outputFileName, int minPeakWidth, int maxPeakWidth);

