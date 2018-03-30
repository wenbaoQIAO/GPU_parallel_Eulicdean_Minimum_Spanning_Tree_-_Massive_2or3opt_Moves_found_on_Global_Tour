#ifndef CAMERA_H
#define CAMERA_H
//***************************************************************************
//
// Advanced CodeColony Camera
// Philipp Crocoll, 2003
//
//***************************************************************************
#include <QGLWidget>
//# include <Windows.h>
#ifndef LINUX
#include <GL/glu.h>		// Need to include it here because the GL* types are required
#endif

//#include <GL\freeglut.h>		// Need to include it here because the GL* types are required
#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)

/////////////////////////////////
//Note: All angles in degrees  //
/////////////////////////////////

struct SF3dVector  //Float 3d-vect, normally used
{
    GLfloat x,y,z;
};

struct SF2dVector
{
    GLfloat x,y;
};

SF3dVector F3dVector ( GLfloat x, GLfloat y, GLfloat z );

class CCamera
{
private:

    SF3dVector ViewDir;
    SF3dVector RightVector;
    SF3dVector UpVector;
    SF3dVector Position;
    SF3dVector target;

    GLfloat RotatedX, RotatedY, RotatedZ;

public:
    CCamera();				//inits the values (Position: (0|0|0) Target: (0|0|-1) )
    void Render ( void );	//executes some glRotates and a glTranslate command
    //Note: You should call glLoadIdentity before using Render

    //! WenBao.Qiao 2015 add
    void initializePos(GLfloat _iniX, GLfloat _iniY, GLfloat _iniZ);

    void Move ( SF3dVector Direction );
    void RotateX ( GLfloat Angle );
    void RotateY ( GLfloat Angle );
    void RotateZ ( GLfloat Angle );

    void RotateObjectX ( GLfloat Angle );
    void RotateObjectY ( GLfloat Angle );

    void MoveForward ( GLfloat Distance );
    void MoveUpward ( GLfloat Distance );
    void StrafeRight ( GLfloat Distance );

};

#endif

