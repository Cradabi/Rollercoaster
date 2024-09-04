#include "peak_detector.h"
#include <algorithm>
#include <numeric>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QTextStream>

QVector<float> movingAverage(const QVector<float>& input, int windowSize) {
    QVector<float> result(input.size(), 0.0);
    float sum = 0.0f;

    for (int i = 0; i < input.size(); ++i) {
        sum += input[i];
        if (i >= windowSize) {
            sum -= input[i - windowSize];
        }
        if (i >= windowSize - 1) {
            result[i] = sum / windowSize;
        }
    }

    return result;
}

QVector<std::pair<int, int>> findPeaks(const QVector<float>& data, float floorLevel, int minPeakWidth, int maxPeakWidth) {
    QVector<std::pair<int, int>> peaks;
    bool inPeak = false;
    int peakStart = 0;

    for (int i = 0; i < data.size(); ++i) {
        if (!inPeak && data[i] > floorLevel) {
            inPeak = true;
            peakStart = i;
        } else if (inPeak && data[i] <= floorLevel) {
            inPeak = false;
            int peakWidth = i - peakStart;
            if (peakWidth >= minPeakWidth && peakWidth <= maxPeakWidth) {
                peaks.push_back(std::make_pair(peakStart, i - 1));
            }
        }
    }
    if (inPeak) {
        int peakWidth = data.size() - peakStart;
        if (peakWidth >= minPeakWidth && peakWidth <= maxPeakWidth) {
            peaks.push_back(std::make_pair(peakStart, data.size() - 1));
        }
    }

    return peaks;
}

std::string search(QString inputFileName, QString outputFileName, int minPeakWidth, int maxPeakWidth) {
    int windowSize = 5000;
    QFile file(inputFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for reading";
        return "fail";
    }

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);

    QVector<float> data;
    float value;
    while (!in.atEnd()) {
        in >> value;
        data.push_back(value);
    }

    file.close();

    QVector<float> filteredData = movingAverage(data, windowSize);

    QVector<float> sortedData = filteredData;
    std::nth_element(sortedData.begin(), sortedData.begin() + sortedData.size() / 2, sortedData.end());
    float floorLevel = sortedData[sortedData.size() / 2];

    QVector<std::pair<int, int>> peaks = findPeaks(filteredData, floorLevel, minPeakWidth, maxPeakWidth);

    QFile outputFile(outputFileName);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << outputFileName;
        return "fail";
    }

    QTextStream out(&outputFile);
    for (int i = 0; i < peaks.size(); ++i) {
        out << peaks[i].first << " " << peaks[i].second << "\n";
    }
    outputFile.close();

    return "Found " + std::to_string(peaks.size()) + " peaks.";
}
