#ifndef PAINTINGMESH_H
#define PAINTINGMESH_H

//***************************************************************************
//
// Jean-Charles CREPUT, Wenbao Qiao, Abdelkhalek MANSOURI
// Created in 2013, Modified in 2015, Modified in 2015/2016/2018
//
//***************************************************************************

#include <QtGui>
#include <QWidget>
#include <QGLWidget>
#include "ConfigParams.h"
#include "camera.h"
#include <GridOfNodes.h>
#include <Node.h>
#include "NeuralNet.h"

//! wenbao Qiao add scale factor to zoom out the area
#define scaleFactor 1

using namespace components;
class PaintingMesh : public QGLWidget
{
    Q_OBJECT
public:
    bool modeColors;
    bool modDimension;
    bool modeSP;
    bool modeVR;
    int  modeDisplay;
    //! wenbao Qiao added 04/15
    int  modNnOrImage;

    explicit  PaintingMesh(QWidget *parent) :
        QGLWidget(parent)
    {
        modeColors = true;
        modDimension = false;
        modeSP = false;
        modeVR = false;
        modNnOrImage = 2;
        modeDisplay = 0;
        clearColor = Qt::black;//::white;
        lastWheel = 0;
    }
    ~PaintingMesh()
    {
        mat_points.freeMem();
        mat_colors.freeMem();
        mat_points_colonnes.freeMem();
        mat_colors_colonnes.freeMem();
        mat_points_colonnes_decalees.freeMem();
        mat_colors_colonnes_decalees.freeMem();
    }
    void initialize(ConfigParams* cp) {
        this->param = cp;
    }
    void makeObject()
    {
        components::Point3D color_default(1.0 ,1.0 ,1.0);
        //! wenbao Qiao modified 14/04/2015
        cout << "Display dimension mode: "  << modDimension << ", content mode:   " << modNnOrImage << endl;
        ifstream fi;
        string file_cfg;
        string file_cfg_color;
        //! akm modif
        string file_SP_color;
        string file_VR_color;


        //! wenbao Qiao modified 14/04/2015
        if (modDimension == false && modNnOrImage == 0)
        {
            cout << "Dimension: "  << modDimension << "   " << modNnOrImage << endl;
            param->readConfigParameter("param_1","fileOutImageItself2DPoint", file_cfg);
            param->readConfigParameter("param_1","fileOutImageItself2DColors", file_cfg_color);
        }
        if (modDimension == true && modNnOrImage == 0)
        {
            cout << "Dimension: "  << modDimension << "   " << modNnOrImage << endl;
            param->readConfigParameter("param_1","fileOutImageItself3DPoint", file_cfg);
            param->readConfigParameter("param_1","fileOutImageItself3DColors", file_cfg_color);
        }
        if (modDimension == false && modNnOrImage == 2)
        {
            cout << "Dimension: "  << modDimension << "   " << modNnOrImage << endl;
            param->readConfigParameter("param_1","fileNnOutNoImage2DPoints", file_cfg);
            param->readConfigParameter("param_1","fileNnOutNoImage2DColors", file_cfg_color);
        }
        if (modDimension == true && modNnOrImage == 2)
        {
            cout << "Dimension: "  << modDimension << "   " << modNnOrImage << endl;
            param->readConfigParameter("param_1","fileNnOutNoImage3DPoints", file_cfg);
            param->readConfigParameter("param_1","fileNnOutNoImage3DColors", file_cfg_color);
        }
        if (modDimension == true && modNnOrImage == 1)
        {
            cout << "Dimension: "  << modDimension << "   " << modNnOrImage << endl;
            param->readConfigParameter("param_1","fileNnOutFromImage3DPoints", file_cfg);
            param->readConfigParameter("param_1","fileNnOutFromImage3DColors", file_cfg_color);

        }
        if (modDimension == false && modNnOrImage == 1)
        {
            cout << "Dimension: "  << modDimension << "   " << modNnOrImage << endl;
            param->readConfigParameter("param_1","fileNnOutFromImage2DPoints", file_cfg);
            param->readConfigParameter("param_1","fileNnOutFromImage2DColors", file_cfg_color);

        }
         //! wenbao Qiao: show 2D links
        if (modDimension == false && modNnOrImage == 3)
        {
            cout << "Dimension: "  << modDimension << "   " << modNnOrImage << endl;
            param->readConfigParameter("param_1","fileNnOutLinksItself2DPoints", file_cfg);
            param->readConfigParameter("param_1","fileNnOutLinksItself2DColors", file_cfg_color);
        }

        fi.open(file_cfg);
        if (!fi) {
            std::cout << "erreur ouverture mesh: " << file_cfg << endl;
        }
        else
            fi >> mat_points;
        fi.close();

        if (mat_points.getHeight() == 0)
        {
            cout << "erreur paintingmesh: not exist points" << endl;
            return;
        }

        if (modeColors ){
            fi.open(file_cfg_color);
            if (!fi) {
                std::cout << "erreur ouverture mesh colors: " << file_cfg_color << endl;
            }
            else
                fi >> mat_colors;
            fi.close();
        }

        for (int _y = 0; _y < mat_points.getHeight(); _y++ ) {
            for (int _x = 0; _x < mat_points.getWidth(); _x++ ) {

                components::Point3D c;
                GLint abc;

                //! 090516 wenbao Qiao delete these modeSP/modeVR, these two modes should not be the same as modeColors
                if (!modeColors)
                {
                    c = color_default;
                }
                else {
                    c= mat_colors.get(_x,_y);
                    //                    c.set(0,(c[0] / 255.0));
                    //                    c.set(1,(c[1] / 255.0));
                    //                    c.set(2,(c[2] / 255.0));
                    c.set(0, c[0]);
                    c.set(1, c[1]);
                    c.set(2, c[2]);
                }
                mat_colors.set(_x, _y, c);
            }
        }

        //! wenbao Qiao : read links
        mat_points_links.readNetLinks(file_cfg);

        pointsLinks.clear();
        colorsLinks.clear();

        points.clear();
        colors.clear();

        //! wenbao Qiao traversing all the node by using links
        vector<PointCoord> nodeAlreadyTraversed;

        PointCoord ps(0, 0); // wenbao Qiao begin node in the netLinks, can be random, but should exist
        mat_points_links.writeLinks("testPainting");

        //! wenbao Qiao iteratively add points to OpenGL
        if(mat_points_links.networkLinks.width && mat_points_links.networkLinks[ps[1]][ps[0]].numLinks != 0){

            addPointsToGLIterative(mat_points_links, ps, nodeAlreadyTraversed, pointsLinks, colorsLinks, mat_points, mat_colors);

        }


        for (int _y = 0; _y < mat_points.getHeight(); _y++ ) {
            for (int _x = 0; _x < mat_points.getWidth(); _x++ ) {

                components::Point3D p = mat_points.get(_x, _y);

                points.append(QVector3D(p[0],
                              p[1],
                        p[2]));
                components::Point3D c = mat_colors.get(_x, _y);
                colors.append(QVector3D(c[0],
                              c[1],
                        c[2]));
                //                }
            }
        }
        cout << "Finish loading 1 : " << pointsLinks.size() << ", points " << points.size() << endl;



        vertices.clear();
        color_vertices.clear();
        vertices.reserve(mat_points.getHeight()*mat_points.getWidth()*6);           //////
        color_vertices.reserve(mat_points.getHeight()*mat_points.getWidth()*6);     //////

        components::Point3D p1, p2, p3, p4, p5, p6;

        nTriangles = 0;
        for (int _y = 0; _y < mat_points.getHeight()-1; _y++ ) {
            for (int _x = 0; _x < mat_points.getWidth()-1; _x++ ) {

                if (_x + 1 < mat_points.getWidth() && _y + 1 < mat_points.getHeight()) {
                    nTriangles += 2;

                    // Points
                    if (_y % 2 == 0) {

                        p1 = mat_points.get(_x, _y);
                        p2 = mat_points.get(_x + 1, _y);
                        p3 = mat_points.get(_x + 1, _y + 1);
                        p4 = mat_points.get(_x + 1, _y + 1);
                        p5 = mat_points.get(_x, _y + 1);
                        p6 = mat_points.get(_x, _y);
                    }
                    else {

                        p1 = mat_points.get(_x, _y);
                        p2 = mat_points.get(_x, _y + 1);
                        p3 = mat_points.get(_x + 1, _y);
                        p4 = mat_points.get(_x + 1, _y);
                        p5 = mat_points.get(_x, _y + 1);
                        p6 = mat_points.get(_x + 1, _y + 1);
                    }

                    vertices.append(QVector3D(p1[0], p1[1], p1[2]));
                    vertices.append(QVector3D(p2[0], p2[1], p2[2]));
                    vertices.append(QVector3D(p3[0], p3[1], p3[2]));
                    vertices.append(QVector3D(p4[0], p4[1], p4[2]));
                    vertices.append(QVector3D(p5[0], p5[1], p5[2]));
                    vertices.append(QVector3D(p6[0], p6[1], p6[2]));

                    // Couleurs
                    if (_y % 2 == 0) {

                        p1 = mat_colors.get(_x, _y);
                        p2 = mat_colors.get(_x + 1, _y);
                        p3 = mat_colors.get(_x + 1, _y + 1);
                        p4 = mat_colors.get(_x + 1, _y + 1);
                        p5 = mat_colors.get(_x, _y + 1);
                        p6 = mat_colors.get(_x, _y);
                    }
                    else {

                        p1 = mat_colors.get(_x, _y);
                        p2 = mat_colors.get(_x, _y + 1);
                        p3 = mat_colors.get(_x + 1, _y);
                        p4 = mat_colors.get(_x + 1, _y);
                        p5 = mat_colors.get(_x, _y + 1);
                        p6 = mat_colors.get(_x + 1, _y + 1);
                    }

                    color_vertices.append(QVector3D(p1[0], p1[1], p1[2]));
                    color_vertices.append(QVector3D(p2[0], p2[1], p2[2]));
                    color_vertices.append(QVector3D(p3[0], p3[1], p3[2]));
                    color_vertices.append(QVector3D(p4[0], p4[1], p4[2]));
                    color_vertices.append(QVector3D(p5[0], p5[1], p5[2]));
                    color_vertices.append(QVector3D(p6[0], p6[1], p6[2]));

                }//if
            }//for
        }//for




        mat_points_colonnes.resize(mat_points.getHeight(), mat_points.getWidth());
        mat_colors_colonnes.resize(mat_points.getHeight(), mat_points.getWidth());
        mat_points_colonnes_decalees.resize(mat_points.getHeight(), mat_points.getWidth()-1);
        mat_colors_colonnes_decalees.resize(mat_points.getHeight(), mat_points.getWidth()-1);

        for (int _y = 0; _y < mat_points.getHeight(); _y++ ) {
            for (int _x = 0; _x < mat_points.getWidth(); _x++ ) {


                components::Point3D p = mat_points.get(_x, _y);
                components::Point3D c = mat_colors.get(_x, _y);

                mat_points_colonnes.set(_y, _x, p);
                mat_colors_colonnes.set(_y, _x, c);

                if (_y % 2 == 0) {
                    if (_x < mat_points.getWidth()-1){
                        mat_points_colonnes_decalees.set(_y, _x, p);
                        mat_colors_colonnes_decalees.set(_y, _x, c);
                    }
                }
                else {
                    if (_x > 0) {
                        mat_points_colonnes_decalees.set(_y, _x-1, p);
                        mat_colors_colonnes_decalees.set(_y, _x-1, c);

                    }
                }
            }
        }
        cout << "Finish loading 2 " << vertices.capacity() << " " << nTriangles * 3 << endl;

    }//else mode 3



private:
    //! wenbao Qiao add iteratively breadth-first search to add points to OpenGL
    void addPointsToGLIterative( MatNetLinks& mat_points_links,
                                 PointCoord ps,
                                 vector<PointCoord>& nodeAlreadyTraversed,
                                 QVector<QVector3D>& pointsLinks,
                                 QVector<QVector3D>& colorsLinks,
                                 Mat3DPoints& mat_points,
                                 Mat3DPoints& mat_colors){

        vector<PointCoord> tabO;
        vector<PointCoord> tabD;

        tabO.push_back(ps);

        PointCoord pInitial(-1, -1);

        while((tabO).size() > 0){

            for (size_t i = 0; i < (tabO).size(); i++){

                PointCoord pCoord = (tabO)[i];

                if(pCoord == pInitial)
                    continue;

                else{

                    bool traversed = 0;

                    PointCoord pLinkFather(-1, -1);
                    for (size_t k = 0; k < nodeAlreadyTraversed.size(); k ++){
                        pLinkFather = nodeAlreadyTraversed[k];
                        if (pCoord == pLinkFather)
                            traversed = 1;
                    }
                    if (traversed){
                        continue;
                    }
                    else{
                        nodeAlreadyTraversed.push_back(pCoord);
                    }

                    Point2D pInitial2D(-1, -1);

                    for (int i = 0; i < mat_points_links.networkLinks[pCoord[1]][pCoord[0]].numLinks; i ++){

                        Point2D pLinkOfNode;
                        mat_points_links.networkLinks[pCoord[1]][pCoord[0]].get(i, pLinkOfNode);

                        if(pLinkOfNode == pInitial2D)
                            continue;

                        else{

                            PointCoord pCoordLink(-1, -1);// can not be (0,0)
                            pCoordLink[0] = (int)pLinkOfNode[0];
                            pCoordLink[1] = (int)pLinkOfNode[1];


                            // compare if the current pCoord is already be teached
                            bool traversed = 0;
                            for (size_t k = 0; k < nodeAlreadyTraversed.size(); k ++){
                                PointCoord pLinkTemp(-1, -1);
                                pLinkTemp = nodeAlreadyTraversed[k];
                                if (pCoordLink == pLinkTemp)
                                    traversed = 1;
                            }
                            if (traversed){
                                continue;
                            }
                            else{

                                // operations
                                components::Point3D pL = mat_points[pCoord[1]][pCoord[0]];
                                components::Point3D cL = mat_colors[pCoord[1]][pCoord[0]];

                                pointsLinks.append(QVector3D(pL[0] * scaleFactor,
                                                   pL[1]* scaleFactor,
                                        pL[2]* scaleFactor));
                                colorsLinks.append(QVector3D(cL[0],
                                                   cL[1],
                                        cL[2]));

                                pL = mat_points[pCoordLink[1]][pCoordLink[0]];
                                cL = mat_colors[pCoordLink[1]][pCoordLink[0]];
                                pointsLinks.append(QVector3D(pL[0] * scaleFactor,
                                                   pL[1]* scaleFactor,
                                        pL[2]* scaleFactor));
                                colorsLinks.append(QVector3D(cL[0],
                                                   cL[1],
                                        cL[2]));

                                cout << "ps pco " << pCoord[0] << ", " << pCoordLink[0] << endl;

                                // end operations
                                (tabD).push_back(pCoord);
                                (tabD).push_back(pCoordLink);
                            }
                        }
                    }

                }
            }

            (tabO).clear();

            tabO.swap(tabD);

        }
    }// end add to gl iterative

signals:
public slots:
signals:
    void clicked();
protected:
    void initializeGL()
    {
        makeObject();
        // set up the depth value for the 3D coordinates
        glClearDepth( 1.0 );
        // enable the depth test work
        glEnable( GL_DEPTH_TEST );
        // the type of the depth test
        glDepthFunc( GL_LEQUAL );
        // make the system to revise the perspective
        glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    }


    void paintGL()
    {

        if (modeDisplay == 0)
            displayGrid();
        else if (modeDisplay == 1)
            displayTriangles();
        else if (modeDisplay == 2)
            displayPoints();
        else if (modeDisplay == 3)
            displayLines();
    }
    void resizeGL(int width, int height)
    {

        //Set a new projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0,(GLdouble)width/(GLdouble)height,0.1,100.0);
        //glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);

        glMatrixMode(GL_MODELVIEW);
        glViewport(0,0,width,height);  //Use the whole window for rendering
    }
    void mousePressEvent(QMouseEvent *event)
    {
        lastPos = event->pos();
    }
    void mouseMoveEvent(QMouseEvent *event)
    {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();

        if (event->buttons() & Qt::LeftButton) {
            rotateObjectBy(1 * -dy, 1 * -dx, 0);
            updateGL();
        } else if (event->buttons() & Qt::RightButton) {
            this->rotateBy(1 * -dy, 1 * dx, 0);

            updateGL();
        }
        lastPos = event->pos();
    }
    void wheelEvent(QWheelEvent *event)
    {
        int dx = event->delta();

        if (event->buttons() & Qt::RightButton) {

            rotateBy(0, 0, 1 * dx/100); // original
            //            rotateBy(0, 0, 0);// WB.Q modify
            updateGL();
        } else {

            camera.MoveForward(dx >= 0 ? -0.5 : 0.5);
            updateGL();
        }
        lastWheel = event->Wheel;
    }
    void mouseReleaseEvent(QMouseEvent * /* event */)
    {
        emit clicked();
    }


private:

    void rotateBy(int xAngle, int yAngle, int zAngle)
    {
        camera.RotateX(xAngle);
        camera.RotateY(yAngle);
        camera.RotateZ(zAngle);
        updateGL();
    }
    void rotateObjectBy(int xAngle, int yAngle, int zAngle)
    {
        camera.RotateObjectX(xAngle);
        camera.RotateObjectY(yAngle);
        updateGL();
    }
    void setClearColor(const QColor &color)
    {
        clearColor = color;
        updateGL();
    }
    void displayGrid(void)
    {

        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glColor3f(0.5, 0, 1);
        glLineWidth(1);

        glLoadIdentity();
        camera.Render();

        glPointSize(3); // wenbao Qiao

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glEnable(GL_LINE_SMOOTH);

        if (modDimension == false && modNnOrImage == 3){


            glVertexPointer(3, GL_FLOAT, 0, pointsLinks.constData());
            glColorPointer(3, GL_FLOAT, 0, colorsLinks.constData());
            glDrawArrays(GL_LINES, 0, pointsLinks.size());
            glDrawArrays(GL_POINTS, 0, pointsLinks.size());// WB.Q draw points at the same time


        }// if mode 3
        else
        {
            for (int _y = 0; _y < mat_points.getHeight(); _y++ ) {
                glVertexPointer(3, GL_FLOAT, 0, mat_points[_y]);
                glColorPointer(3, GL_FLOAT, 0, mat_colors[_y]);
                glDrawArrays(GL_LINE_STRIP, 0, mat_points.getWidth());

            }

            for (int _y = 0; _y < mat_points_colonnes.getHeight(); _y++ ) {
                glVertexPointer(3, GL_FLOAT, 0, mat_points_colonnes[_y]);
                glColorPointer(3, GL_FLOAT, 0, mat_colors_colonnes[_y]);
                glDrawArrays(GL_LINE_STRIP, 0, mat_points_colonnes.getWidth());
            }

            for (int _y = 0; _y < mat_points_colonnes_decalees.getHeight(); _y++ ) {
                glVertexPointer(3, GL_FLOAT, 0, mat_points_colonnes_decalees[_y]);
                glColorPointer(3, GL_FLOAT, 0, mat_colors_colonnes_decalees[_y]);
                glDrawArrays(GL_LINE_STRIP, 0, mat_points_colonnes_decalees.getWidth());
            }
        }// else mode 3

        glDisableClientState(GL_VERTEX_ARRAY);  // Q: disable vertex arrays
        glDisableClientState(GL_COLOR_ARRAY);

    }
    void displayLines(void)
    {
        //        qglClearColor(clearColor);
        //        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        camera.Render();

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        if (modDimension == false && modNnOrImage == 3){

            glVertexPointer(3, GL_FLOAT, 0, pointsLinks.constData());
            glColorPointer(3, GL_FLOAT, 0, colorsLinks.constData());
            glDrawArrays(GL_LINE_STRIP, 0, pointsLinks.size());

        }// if mode 3
        else
        {
            for (int _y = 0; _y < mat_points.getHeight(); _y++ ) {
                glVertexPointer(3, GL_FLOAT, 0, mat_points[_y]);
                glColorPointer(3, GL_FLOAT, 0, mat_colors[_y]);

                glDrawArrays(GL_LINE_STRIP, 0, mat_points.getWidth());
            }
        }// else mode 3
    }
    void displayTriangles()
    {
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        camera.Render();

        if (modDimension == false && modNnOrImage == 3){

            glVertexPointer(3, GL_FLOAT, 0, verticesLinks.constData());
            glColorPointer(3, GL_FLOAT, 0, color_verticesLinks.constData());


            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);


            glDrawArrays(GL_TRIANGLES, 0, nTriangles*3);

        }// if mode 3
        else
        {

            glVertexPointer(3, GL_FLOAT, 0, vertices.constData());

            glColorPointer(3, GL_FLOAT, 0, color_vertices.constData());

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);

            glDrawArrays(GL_TRIANGLES, 0, nTriangles*3);

        }

    }
    void displayPoints()
    {

        //! wenbao Qiao show white backgournd
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glLoadIdentity();
        camera.Render();
        //      glEnable(GL_POINT_SMOOTH);
        glPointSize(2);

        glVertexPointer(3, GL_FLOAT, 0, points.constData());
        glColorPointer(3, GL_FLOAT, 0, colors.constData());

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glDrawArrays(GL_POINTS, 0, mat_points.getHeight() * mat_points.getWidth());

    }
    //displays the scene.
    void displayCube(void)
    {
        qglClearColor(clearColor);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        camera.Render();

        //Draw the "world" (which consists of six "nets" forming a cuboid
        glTranslatef(0.0, 0.0, -5.0);


        GLfloat size = 2.0;
        GLint LinesX = 10;
        GLint LinesZ = 10;

        GLfloat halfsize = size / 2.0;
        glColor3f(1.0,1.0,1.0);
        glPushMatrix();
        glTranslatef(0.0,-halfsize ,0.0);
        drawNet(size,LinesX,LinesZ);
        glTranslatef(0.0,size,0.0);
        drawNet(size,LinesX,LinesZ);
        glPopMatrix();
        glColor3f(0.0,0.0,1.0);
        glPushMatrix();
        glTranslatef(-halfsize,0.0,0.0);
        glRotatef(90.0,0.0,0.0,halfsize);
        drawNet(size,LinesX,LinesZ);
        glTranslatef(0.0,-size,0.0);
        drawNet(size,LinesX,LinesZ);
        glPopMatrix();
        glColor3f(1.0,0.0,0.0);
        glPushMatrix();
        glTranslatef(0.0,0.0,-halfsize);
        glRotatef(90.0,halfsize,0.0,0.0);
        drawNet(size,LinesX,LinesZ);
        glTranslatef(0.0,size,0.0);
        drawNet(size,LinesX,LinesZ);
        glPopMatrix();

        //finish rendering:
        glFlush();
        //glutSwapBuffers();
    }
    //draws a plane "net"
    void drawNet(GLfloat size, GLint LinesX, GLint LinesZ)
    {
        glBegin(GL_LINES);
        for (int xc = 0; xc < LinesX; xc++)
        {
            glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
                        0.0,
                        size / 2.0);
            glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
                        0.0,
                        size / -2.0);
        }
        for (int zc = 0; zc < LinesZ; zc++)
        {
            glVertex3f(	size / 2.0,
                        0.0,
                        -size / 2.0 + zc / (GLfloat)(LinesZ-1)*size);
            glVertex3f(	size / -2.0,
                        0.0,
                        -size / 2.0 + zc / (GLfloat)(LinesZ-1)*size);
        }
        glEnd();
    }

    ConfigParams* param;

    CCamera camera;

    QColor clearColor;
    QPoint lastPos;
    int lastWheel;

    int nTriangles;

    // data
    Mat3DPoints mat_points;
    Mat3DPoints mat_colors;

    //! wenbao Qiao 120516 add
    MatNetLinks mat_points_links;
    MatNetLinks mat_colors_links;

    //! AKM 270415 : modif
    Grid<GLfloat> mat_density;

    // buffers for opengl arrays
    QVector<QVector3D> points;
    QVector<QVector3D> colors;
    QVector<QVector3D> vertices;
    QVector<QVector3D> color_vertices;

    //! wenbao Qiao 120516 add for view links
    QVector<QVector3D> pointsLinks;
    QVector<QVector3D> colorsLinks;
    QVector<QVector3D> verticesLinks;
    QVector<QVector3D> color_verticesLinks;


    Mat3DPoints mat_points_colonnes;
    Mat3DPoints mat_colors_colonnes;
    Mat3DPoints mat_points_colonnes_decalees;
    Mat3DPoints mat_colors_colonnes_decalees;

public:
    void drawLines(QPainter *qp)
    {
        QPen pen(Qt::black, 2, Qt::SolidLine);

        qp->setPen(pen);
        qp->drawLine(20, 40, 250, 40);

        pen.setStyle(Qt::DashLine);
        qp->setPen(pen);
        qp->drawLine(20, 80, 250, 80);
    }
};

#endif // PAINTINGMESH_H
