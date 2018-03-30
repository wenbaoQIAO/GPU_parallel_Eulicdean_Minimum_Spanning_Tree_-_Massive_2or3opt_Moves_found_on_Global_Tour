#ifndef PARAMFRAME_H
#define PARAMFRAME_H
//***************************************************************************
//
// Jean-Charles CREPUT, wenbao Qiao, Abdelkhalek MANSOURI
// Created in 2013, Modified in 2015, Modified 2015/2016/2018
//
//***************************************************************************

#include <QWidget>
#include "paintingmesh.h"
#include <QGroupBox>
#include <QHeaderView>
#include <QRadioButton>
#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
class QGroupBox;
class QRadioButton;
QT_END_NAMESPACE

class ParamFrame : public QWidget
{
    Q_OBJECT

private:
    QGroupBox *twoSidedGroupBox;
    QGroupBox *nnImageGroupBox;
    QGroupBox *colorsGroupBox;
    QHBoxLayout *horizontalLayout;

    QRadioButton *view2DEnabledRadio;
    QRadioButton *view2DDisabledRadio;
    QRadioButton *nnEnabledRadio;
    QRadioButton *nnDisabledRadio;

    //! wenbao Qiao 140415/030516 add these buttons
    QRadioButton *nnImageItself;
    QRadioButton *nnLinks;
    QRadioButton *colorsEnabledRadio;
    QRadioButton *colorsDisabledRadio;


    QRadioButton *SPEnableRadio;
    QRadioButton *VREnableRadio;

    QGroupBox *displayGroupBox;
    QRadioButton *displayMRadio;
    QRadioButton *displayTRadio;
    QRadioButton *displayPRadio;
    QRadioButton *displayLRadio;
    QRadioButton *displayClosed;
    QRadioButton *displayOpen;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_4;


    PaintingMesh *pme;
public:
    explicit ParamFrame(QFrame *parent = 0)
    {

        horizontalLayout = new QHBoxLayout(parent);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString("horizontalLayout"));

        label = new QLabel(parent);
        label->setObjectName(QString("label"));
        label->setAlignment(Qt::AlignCenter);
        label->setMaximumSize(QSize(250, 79));

        QFont font;
        font.setPointSize(13);
        font.setItalic(true);
        label->setFont(font);
        label->raise();



        horizontalLayout->addWidget(label);

        twoSidedGroupBox = new QGroupBox(parent);
        twoSidedGroupBox->setObjectName(QString("groupBox"));
        twoSidedGroupBox->setMaximumSize(QSize(120, 82));
        horizontalLayout->addWidget(twoSidedGroupBox);

        verticalLayout = new QVBoxLayout(twoSidedGroupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString("verticalLayout"));


        view2DEnabledRadio = new QRadioButton(twoSidedGroupBox);
        view2DEnabledRadio->setObjectName(QString("radioButton"));

        view2DDisabledRadio = new QRadioButton(twoSidedGroupBox);
        view2DDisabledRadio->setObjectName(QString("radioButton_2"));


        view2DEnabledRadio->setChecked(true);
        verticalLayout->addWidget(view2DEnabledRadio);
        verticalLayout->addWidget(view2DDisabledRadio);


        nnImageGroupBox = new QGroupBox(parent);
        nnImageGroupBox->setObjectName(QString("nnImageBox"));
       // nnImageGroupBox->setMaximumSize(QSize(310, 80));
        nnImageGroupBox->setMinimumSize(QSize(310, 80));

        horizontalLayout->addWidget(nnImageGroupBox);

        gridLayout_3 = new QGridLayout(nnImageGroupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString("gridLayout_3"));

        nnEnabledRadio = new QRadioButton(nnImageGroupBox);
        nnEnabledRadio->setObjectName(QString("nnImageNn"));
        nnEnabledRadio->setGeometry(QRect(30, 30, 117, 22));

        nnDisabledRadio = new QRadioButton(nnImageGroupBox);
        nnDisabledRadio->setObjectName(QString("nnImageImage"));
        nnDisabledRadio->setGeometry(QRect(30, 60, 117, 22));

        nnImageItself = new QRadioButton(nnImageGroupBox);
        nnImageItself->setObjectName(QString("nnImageItself"));
        nnImageItself->setGeometry(QRect(150, 30, 117, 22));

        nnLinks = new QRadioButton(nnImageGroupBox);
        nnLinks->setObjectName(QString("nnLinks"));
        nnLinks->setGeometry(QRect(150, 30, 117, 22));

        nnEnabledRadio->setChecked(true);
        gridLayout_3->addWidget(nnEnabledRadio, 0, 0, 1, 1);
        gridLayout_3->addWidget(nnDisabledRadio, 1, 0, 1, 1);
        gridLayout_3->addWidget(nnImageItself, 0, 1, 1, 1 );
        gridLayout_3->addWidget(nnLinks, 1, 1, 1, 1);

        colorsGroupBox = new QGroupBox(parent);
        colorsGroupBox->setObjectName(QString("groupBox_2"));
        colorsGroupBox->setMinimumSize(QSize(310, 80));
       // colorsGroupBox->setMaximumSize(QSize(310, 80));

        horizontalLayout->addWidget(colorsGroupBox);

        gridLayout_4 = new QGridLayout(colorsGroupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString("gridLayout_4"));

        colorsEnabledRadio = new QRadioButton(colorsGroupBox);
        colorsEnabledRadio->setObjectName(QString("radioButton_3"));
        colorsEnabledRadio->setGeometry(QRect(30, 30, 117, 22));

        colorsDisabledRadio = new QRadioButton(colorsGroupBox);
        colorsDisabledRadio->setObjectName(QString("radioButton_4"));
        colorsDisabledRadio->setGeometry(QRect(30, 60, 117, 22));

        SPEnableRadio = new QRadioButton(colorsGroupBox);
        SPEnableRadio->setObjectName(QString("radioButton_9"));
        SPEnableRadio->setGeometry(QRect(150, 30, 117, 22));

        VREnableRadio = new QRadioButton(colorsGroupBox);
        VREnableRadio->setObjectName(QString("radioButton_9"));
        VREnableRadio->setGeometry(QRect(150, 60, 117, 22));



        colorsEnabledRadio->setChecked(true);
        gridLayout_4->addWidget(colorsEnabledRadio, 0, 0, 1, 1);
        gridLayout_4->addWidget(colorsDisabledRadio, 1, 0, 1, 1);
        gridLayout_4->addWidget(SPEnableRadio, 0, 1, 1, 1);
        gridLayout_4->addWidget(VREnableRadio, 1, 1, 1, 1);



        displayGroupBox = new QGroupBox(parent);
        displayGroupBox->setObjectName(QString("groupBox_3"));
        displayGroupBox->setMinimumSize(QSize(310, 80));
       // displayGroupBox->setMaximumSize(QSize(310, 80));
        horizontalLayout->addWidget(displayGroupBox);

        gridLayout_2 = new QGridLayout(displayGroupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString("gridLayout_2"));

        displayMRadio = new QRadioButton(displayGroupBox);
        displayMRadio->setObjectName(QString("radioButton_5"));
        displayMRadio->setGeometry(QRect(30, 30, 117, 22));

        displayTRadio = new QRadioButton(displayGroupBox);
        displayTRadio->setObjectName(QString("radioButton_6"));
        displayTRadio->setGeometry(QRect(30, 60, 117, 22));

        displayPRadio = new QRadioButton(displayGroupBox);
        displayPRadio->setObjectName(QString("radioButton_7"));
        displayPRadio->setGeometry(QRect(180, 30, 117, 22));

        displayLRadio = new QRadioButton(displayGroupBox);
        displayLRadio->setObjectName(QString("radioButton_8"));
        displayLRadio->setGeometry(QRect(180, 60, 117, 22));

        displayClosed = new QRadioButton(displayGroupBox);
        displayClosed->setObjectName(QString("radioButton_colosed"));
        displayClosed->setGeometry(QRect(280, 30, 117, 22));

        displayOpen = new QRadioButton(displayGroupBox);
        displayOpen->setObjectName(QString("radioButton_open"));
        displayOpen->setGeometry(QRect(280, 60, 117, 22));

        displayMRadio->setChecked(true);
        gridLayout_2->addWidget(displayMRadio, 0, 0, 1, 1);
        gridLayout_2->addWidget(displayTRadio, 0, 1, 1, 1);
        gridLayout_2->addWidget(displayPRadio, 1, 0, 1, 1);
        gridLayout_2->addWidget(displayLRadio, 1, 1, 1, 1);
        gridLayout_2->addWidget(displayClosed, 0, 2, 1, 1);
        gridLayout_2->addWidget(displayOpen, 1, 2, 1, 1);


        twoSidedGroupBox->setTitle(QApplication::translate("MainWindow", "2D-3D", 0));
        view2DEnabledRadio->setText(QApplication::translate("MainWindow", "2D", 0));
        view2DDisabledRadio->setText(QApplication::translate("MainWindow", "3D", 0));

        //! 0415 wenbao Qiao add to these buttons
        nnImageGroupBox->setTitle(QApplication::translate("MainWindow", "NeuralNet", 0));
        nnEnabledRadio->setText(QApplication::translate("MainWindow", "Nn-Nn", 0));
        nnDisabledRadio->setText(QApplication::translate("MainWindow", "Nn-Im", 0));
        nnImageItself->setText(QApplication::translate("MainWindow", "Image", 0));
        nnLinks->setText(QApplication::translate("MainWindow", "Nn-Links", 0));

        colorsGroupBox->setTitle(QApplication::translate("MainWindow", "Colors", 0));
        colorsEnabledRadio->setText(QApplication::translate("MainWindow", "Enabled", 0));
        colorsDisabledRadio->setText(QApplication::translate("MainWindow", "Disabled", 0));

        //! 0615 abdo add these buttons
        SPEnableRadio->setText(QApplication::translate("MainWindow", "SP segementation", 0));
        VREnableRadio->setText(QApplication::translate("MainWindow", "Voronoi image", 0));

        displayGroupBox->setTitle(QApplication::translate("MainWindow", "Display", 0));
        displayMRadio->setText(QApplication::translate("MainWindow", "Mesh", 0));
        displayTRadio->setText(QApplication::translate("MainWindow", "Triangles", 0));
        displayPRadio->setText(QApplication::translate("MainWindow", "Points", 0));
        displayLRadio->setText(QApplication::translate("MainWindow", "Lines", 0));
        displayClosed->setText(QApplication::translate("MainWindow", "TspC", 0));
        displayOpen->setText(QApplication::translate("MainWindow", "TspO", 0));


        label->setText(QApplication::translate("MainWindow", "Display\nparameters", 0));


        connect(view2DEnabledRadio, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(view2DDisabledRadio, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(nnEnabledRadio, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(nnDisabledRadio, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(nnImageItself, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(nnLinks, SIGNAL(clicked()), this, SLOT(updateView()));


        connect(colorsEnabledRadio, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(colorsDisabledRadio, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(SPEnableRadio, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(VREnableRadio, SIGNAL(clicked()), this, SLOT(updateView()));



        connect(displayMRadio, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(displayTRadio, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(displayPRadio, SIGNAL(clicked()), this, SLOT(updateView()));
        connect(displayLRadio, SIGNAL(clicked()), this, SLOT(updateView()));

    }
    ~ParamFrame() {}

    void setWidgetsLink( PaintingMesh *pme) {
        this->pme = pme;
    }

private slots:

    void updateView()
    {
        //! wenbao Qiao modified 14042015
        if (view2DEnabledRadio->isChecked()) {
            pme->modDimension = false;
            pme->makeObject();
        }
        //! wenbao Qiao modified 14042015
        else if (view2DDisabledRadio->isChecked()) {
            pme->modDimension = true;
            pme->makeObject();
        }

        //! wenbao Qiao modified 14042015
        if (nnDisabledRadio->isChecked()) { //from image
            pme->modNnOrImage = 1;
            pme->makeObject();
        }
        //! wenbao Qiao modified 14042015
        else if (nnEnabledRadio->isChecked()) { //from Nn itself
            pme->modNnOrImage = 2;
            pme->makeObject();
        }
        else if (nnImageItself->isChecked()){
            pme->modNnOrImage = 0;
            pme->makeObject();
        }
        else if (nnLinks->isChecked()){
            pme->modNnOrImage = 3;
            pme->makeObject();
        }

        if (colorsEnabledRadio->isChecked()) {
            pme->modeColors = true;
            pme->modeSP = false;
            pme->modeVR = false;
            pme->makeObject();
            std::cout << "Colors" << endl;
        } else if (SPEnableRadio->isChecked()) {
            pme->modeColors = false;
            pme->modeSP = true;
            pme->modeVR = false;
            pme->makeObject();
            std::cout << "Colors disable" << endl;
        } else if (VREnableRadio->isChecked()) {
            pme->modeColors = false;
            pme->modeSP = false;
            pme->modeVR = true;
            pme->makeObject();
            std::cout << "Colors disable" << endl;
        }
        else {
            pme->modeColors = false;
            pme->modeSP = false;
            pme->modeVR = false;
            pme->makeObject();
            std::cout << "Colors disable" << endl;
        }
        if (displayMRadio->isChecked()) {
            pme->modeDisplay = 0;
        } else if (displayTRadio->isChecked()) {
            pme->modeDisplay = 1;
        } else if (displayPRadio->isChecked()) {
            pme->modeDisplay = 2;
        } else if (displayLRadio->isChecked()) {
            pme->modeDisplay = 3;
        }
    }

};

#endif // PARAMFRAME_H
