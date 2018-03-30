#include <QApplication>
#include <cstdlib>
#include "ctrlwidget.h"

char cfgFile[256];


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    char* fileData;
    char* fileSolution;
    char* fileStats;
    char* fileConfig;

    /*
     * Lecture des fichiers d'entree
     */
    if (argc >= 5)
    {
        fileData = argv[1];
        fileSolution = argv[2];
        fileStats = argv[3];
        fileConfig = argv[4];
    }

    CtrlWidget w(0, fileData, fileSolution, fileStats, fileConfig);

    w.show();

    return a.exec();
}
