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
    QVector<QString> data;

    while (!in.atEnd()) {
        line = in.readLine();
        data.push_back(line);
    }
    outputFile.close();
    QVector<QString> exp_data = {"0 11893",
                                 "25998 30836",
                                 "48130 58418",
                                 "58705 62229",
                                 "141529 144693",
                                 "153652 179903",
                                 "234923 238515",
                                 "320291 384627",
                                 "401710 431041",
                                 "465056 471718",
                                 "475937 499696",
                                 "500073 506599",
                                 "580160 632329",
                                 "644901 699665",
                                 "772721 781801",
                                 "783397 814290",
                                 "819293 825156",
                                 "836064 917861",
                                 "932984 939451",
                                 "952479 1023999"};
    if(res_str != "Found 20 peaks."){
        FAIL() << "incorrect number of peaks";
    }
    if(exp_data != data){
        FAIL() << "Incorrect borders of peaks";
    }
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    inputFileName = argv[1];
    outputFileName = argv[2];
    minw = 3000;
    maxw = 100000;

    return RUN_ALL_TESTS();
}
