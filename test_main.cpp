#include <gtest/gtest.h>
#include <QFile>
#include <QTextStream>
#include "peak_detector.h"

QString inputFileName;
QString outputFileName;
int minw;
int maxw;

TEST(PeakDetectorTest, PeaksTest) {
    std::string res_str = search(inputFileName, outputFileName, minw, maxw);

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

    if(res_str != "Found 20 peaks."){
        FAIL() << "incorrect number of peaks";
    }
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);

    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <minw> <maxw>" << std::endl;
        return 1;
    }

    inputFileName = argv[1];
    outputFileName = argv[2];
    minw = QString(argv[3]).toInt();
    maxw = QString(argv[4]).toInt();

    return RUN_ALL_TESTS();
}
