#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QDebug>
#include <algorithm>
#include <gtest/gtest.h>


QString inputFileName;
QString outputFileName;
int minPeakWidth;
int maxPeakWidth;

QVector<float> medianFilter(const QVector<float>& input, int windowSize) {
    QVector<float> result = input;
    QVector<float> window(windowSize);

    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < windowSize; ++j) {
            int idx = i - windowSize / 2 + j;
            if (idx < 0) {
                idx = 0;
            }
            if (idx >= input.size()) {
                idx = input.size() - 1;
            }
            window[j] = input[idx];
        }
        std::nth_element(window.begin(), window.begin() + windowSize / 2, window.end());
        result[i] = window[windowSize / 2];
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

TEST(PeakDetectorTest, OutputFileHasTwoIntegersPerLine) {
    QFile outputFile(outputFileName);
    if (!outputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        FAIL() << "Failed to open file: " << outputFileName.toStdString();
        return;
    }

    QTextStream in(&outputFile);
    QString line;
    QStringList parts;
    int lineNumber = 1;

    while (!in.atEnd()) {
        line = in.readLine();
        parts = line.split(" ");

        if (parts.size() != 2) {
            FAIL() << "Line " << lineNumber << " does not contain two integers separated by a space: "
                   << line.toStdString();
            outputFile.close();
            return;
        }

        bool ok1, ok2;
        parts[0].toInt(&ok1);
        parts[1].toInt(&ok2);

        if (!ok1) {
            FAIL() << "First value is not an integer on line " << lineNumber << ": "
                   << parts[0].toStdString();
        }

        if (!ok2) {
            FAIL() << "Second value is not an integer on line " << lineNumber << ": "
                   << parts[1].toStdString();
        }

        ++lineNumber;
    }

    outputFile.close();
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        qDebug() << "Usage: " << argv[0] << " <input_file> <output_file> <min_peak_width> <max_peak_width>";
        return 1;
    }

    inputFileName = argv[1];
    outputFileName = argv[2];
    minPeakWidth = QString(argv[3]).toInt();
    maxPeakWidth = QString(argv[4]).toInt();
    int windowSize = 50;

    QFile file(inputFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for reading";
        return 1;
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

    QVector<float> filteredData = medianFilter(data, windowSize);

    QVector<float> sortedData = filteredData;
    std::nth_element(sortedData.begin(), sortedData.begin() + sortedData.size() / 2, sortedData.end());
    float floorLevel = sortedData[sortedData.size() / 2];

    QVector<std::pair<int, int>> peaks = findPeaks(data, floorLevel, minPeakWidth, maxPeakWidth);

    QFile outputFile(outputFileName);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << outputFileName;
        return 1;
    }
    QTextStream out(&outputFile);
    for (int i = 0; i < peaks.size(); ++i) {
        out << peaks[i].first << " " << peaks[i].second << "\n";
    }
    outputFile.close();

    qDebug() << "Found" << peaks.size() << "peaks. Results saved to" << outputFileName;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
