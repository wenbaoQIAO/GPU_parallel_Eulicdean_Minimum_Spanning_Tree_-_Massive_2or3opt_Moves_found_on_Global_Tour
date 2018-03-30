#ifndef CTRLWIDGET_H
#define CTRLWIDGET_H
//***************************************************************************
//
// Jean-Charles CREPUT, Wenbao Qiao, Abdelkhalek MANSOURI
// Created in 2013, Modified in 2015/2018
//
//***************************************************************************

#include <QWidget>
#include "ConfigParams.h"
#include "Converter.h"
#include "interfaceUI.h"
#include <iostream>
#include <ctime>

using namespace std;

#define CPU 0
#define GPU 1
#define GPU_NAY 0

//#if GPU
extern "C" void cuda_main(ConfigParams* cp);
//#endif

//#if GPU_NAY
extern "C" void cuda_main_nay( int, int , float * );
//#endif

// DIRECTIVES DE COMPILATION
#define ANCIEN				0

#define SPIRAL_SEARCH       1
#define SPIRAL_SEARCH_REFRESH_RATE  4000
// to choose whether lissage or not
#define LISSAGE             1

#define BREAK_ITE			10000

#define DEMO    			0

//extern TMatPix* MatRess;

static long cptIte; // nombre d'iterations
static long maxIte; // nombre total d'iterations
static int breakIte; // nombre d'iteration avant affichage

namespace Ui {
class MainWindow;
}

class CtrlWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit CtrlWidget(QWidget *parent,char* fileData, char* fileSolution, char* fileStats, char* cfgFile) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        ui->paramFrame->setWidgetsLink(ui->paintingMesh);
        param = ConfigParams(cfgFile);


        NN gtNN, adaptiveNN;
        ImageRW<Point2D, GLfloat> input_read;

        int fromPFM = 0;
        param.readConfigParameters();
        param.readConfigParameter("calib_stereo","pfm", fromPFM);

        //! wenbao Qiao, different functionModeChoice represents different applications
        if (param.functionModeChoice == 11 || param.functionModeChoice == 13 ||  param.functionModeChoice == 14
                ||  param.functionModeChoice == 15 ||  param.functionModeChoice == 16){

            NetLinkPointCoord inNnLinks;
            MatNetLinks oNnLink2D;
            NN inPut;
            inPut.read(fileSolution);

            int _w = inPut.adaptiveMap.width;
            int _h = inPut.adaptiveMap.height;

            cout << "netWorkLinks w , h " << _w << ", " << _h << endl;

            inNnLinks.resize(_w, _h); // this step is necessary
            inNnLinks.readNetLinks(fileSolution); // Q: there is a bug here using nnLinks to read adaptiveMap directly
            inNnLinks.adaptiveMap = inPut.adaptiveMap;
            inNnLinks.colorMap = inPut.colorMap;
            inNnLinks.densityMap = inPut.densityMap;


            //! wenbao Qiao: 040816 convert inNnLinks (2D adaptiveMap) into oNnLink2D (3D adaptiveMap with the same Z value)
            //! they have the same cmLinks and size, the only difference is the Dimension of adaptiveMap
            oNnLink2D.resize(_w, _h);
            oNnLink2D.networkLinks = inNnLinks.networkLinks;

            convert.convertNnLinkPointsFromItself(inNnLinks, oNnLink2D, param, 1, 1);

            oNnLink2D.writeLinks("outputNnLinksItself2D");
            oNnLink2D.write("outputNnLinksItself2D");

        }//end functionChoice != 11
        else{
            if (fromPFM)
            {
                //! wenbao Qiao 141015 add for new irw.h
                cout << "Using Middlebury database version 2014" << endl;
                input_read.read(fileData, gtNN, 1);// wenbao Qiao 220416 windows can read disp0GT.pfm, linux can not read this file.
            }
            else
            {
                //! wenbao Qiao: for xxxL.png xxxR.png, check the "pfm = 0" in config.cfg
                input_read.read(fileData, gtNN);
            }



            adaptiveNN.read(fileSolution); //! wenbao Qiao 04/2015 NN read from output.xxx

            adaptiveNN.densityMap =  gtNN.densityMap;
            adaptiveNN.colorMap.resize(gtNN.colorMap.width, gtNN.colorMap.height);
            adaptiveNN.colorMap = gtNN.colorMap;


            int _w= adaptiveNN.adaptiveMap.width;
            int _h = adaptiveNN.adaptiveMap.height;
            cout << "adaptiveNN.adaptiveMap w = " << _w << " , h = " << _h << endl;
            cout << "adaptiveNN.densityMap  w = " << adaptiveNN.densityMap.width << " , h = " << adaptiveNN.densityMap.height << endl;


            NNP3D oNN2D, oNN3D;
            //!wb.Q: NN-nn button
            //!wb.Q: Convert by itself: can read adaptiveNN' own colormap and densitymap to produce 3D points
            //!wb.Q: if adaptiveNN has no densityMap , then it is displayed in white and 2D, but we can always use the gt values.
            convert.convertGridItself(adaptiveNN, oNN2D, param, 1); // 2D
            oNN2D.write("outputNnNoImage2D");
            convert.convertGridItself(adaptiveNN, oNN3D, param, 0); //3D
            oNN3D.write("outputNnNoImage3D");

            NNP3D ioNN2D,ioNN3D, oNN2, oNN3;

            convert.convertFromImage(gtNN, adaptiveNN, oNN2, param, 1); //2D
            oNN2.write("outputNnFromImage2D");
            convert.convertFromImage(gtNN, adaptiveNN,  oNN3, param, 0); //3D
            oNN3.write("outputNnFromImage3D");

            //! wenbao Qiao add image button
            convert.convertImageItself(gtNN, ioNN2D,  param, 1);
            ioNN2D.write("outputImageItself2D");
            convert.convertImageItself(gtNN, ioNN3D,  param, 0);
            ioNN3D.write("outputImageItself3D");
        }

        ui->paintingMesh->initialize(&param);
    }

    ~CtrlWidget()
    {
        delete ui;
    }

private:
    Ui::MainWindow *ui;
    Converter< Point2D, GLfloat > convert;
    ConfigParams param;
};
#endif // CTRLWIDGET_H
