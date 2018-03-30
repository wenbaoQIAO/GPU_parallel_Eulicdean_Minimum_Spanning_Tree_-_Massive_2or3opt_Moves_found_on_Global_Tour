#ifndef INTERFACUI_H
#define INTERFACUI_H
//***************************************************************************
//
// Jean-Charles CREPUT, Wenbao Qiao, Abdelkhalek MANSOURI
//
//***************************************************************************

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QRadioButton>
#include <QGroupBox>

#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTabWidget>
#include <QWidget>

#include "paintingmesh.h"
#include "paramframe.h"
#include <QtWidgets/QMainWindow>
#include "paramframe.h"


class Ui_MainWindow
{

public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QFrame *frame;
    QFrame *frame_2;
    QWidget *Mesh;
    QWidget *Mesh2;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_6;
    PaintingMesh *paintingMesh;
    PaintingMesh *paintingMesh2;
    ParamFrame *paramFrame;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString("MainWindow"));
        MainWindow->resize(1000, 750);
        MainWindow->setAnimated(false);

        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString("centralWidget"));
        centralWidget->setEnabled(true);

        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString("gridLayout"));

        frame = new QFrame(centralWidget);
        frame->setObjectName(QString("frame"));
        frame->setMaximumSize(QSize(16777215, 101));
        frame->setSizeIncrement(QSize(0, 0));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout->addWidget(frame, 0, 0, 1, 2);
        paramFrame = new ParamFrame(frame);

        Mesh = new QWidget(centralWidget);
        Mesh->setObjectName(QString("Mesh"));
        Mesh->setEnabled(true);

        gridLayout_5 = new QGridLayout(Mesh);
        gridLayout_5->setSpacing(2);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString("gridLayout_5"));

        paintingMesh = new PaintingMesh(Mesh);
        paintingMesh->setObjectName(QString("paintingMesh"));

        gridLayout_5->addWidget(paintingMesh);


        gridLayout->addWidget(Mesh, 1, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    }
    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "POPIP Viewer ", 0));
    }
};

namespace Ui {
class MainWindow: public Ui_MainWindow {};
}

#endif // INTERFACUI_H
