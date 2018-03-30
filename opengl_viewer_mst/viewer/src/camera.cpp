#include "camera.h"
#include "math.h"
#include <iostream>
//# include <Windows.h>

#define SQR(x) (x*x)

#define NULL_VECTOR F3dVector(0.0f,0.0f,0.0f)

SF3dVector F3dVector ( GLfloat x, GLfloat y, GLfloat z )
{
    SF3dVector tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.z = z;
    return tmp;
}

GLfloat GetF3dVectorLength( SF3dVector * v)
{
    return (GLfloat)(sqrt(SQR(v->x)+SQR(v->y)+SQR(v->z)));
}

SF3dVector Normalize3dVector( SF3dVector v)
{
    SF3dVector res;
    float l = GetF3dVectorLength(&v);
    if (l == 0.0f) return NULL_VECTOR;
    res.x = v.x / l;
    res.y = v.y / l;
    res.z = v.z / l;
    return res;
}

SF3dVector operator+ (SF3dVector v, SF3dVector u)
{
    SF3dVector res;
    res.x = v.x+u.x;
    res.y = v.y+u.y;
    res.z = v.z+u.z;
    return res;
}
SF3dVector operator- (SF3dVector v, SF3dVector u)
{
    SF3dVector res;
    res.x = v.x-u.x;
    res.y = v.y-u.y;
    res.z = v.z-u.z;
    return res;
}


SF3dVector operator* (SF3dVector v, float r)
{
    SF3dVector res;
    res.x = v.x*r;
    res.y = v.y*r;
    res.z = v.z*r;
    return res;
}

SF3dVector CrossProduct (SF3dVector * u, SF3dVector * v)
{
    SF3dVector resVector;
    resVector.x = u->y*v->z - u->z*v->y;
    resVector.y = u->z*v->x - u->x*v->z;
    resVector.z = u->x*v->y - u->y*v->x;

    return resVector;
}
float operator* (SF3dVector v, SF3dVector u)	//dot product
{
    return v.x*u.x+v.y*u.y+v.z*u.z;
}

CCamera::CCamera()
{
    //Init with standard OGL values:

    Position = F3dVector (0.0, 0.0,	0.0);
    ViewDir = F3dVector( 0.0, 0.0, -1.0);
    RightVector = F3dVector (1.0, 0.0, 0.0);
    UpVector = F3dVector (0.0, 1.0, 0.0);

    target = F3dVector (0.0, 0.0, -10.0);

    //Only to be sure:
    RotatedX = RotatedY = RotatedZ = 0.0;
}

//WB.Q add to change the initial position of original coordiante
void CCamera::initializePos(GLfloat _iniX, GLfloat _iniY, GLfloat _iniZ)
{
    //Init with standard OGL values:

    Position = F3dVector (_iniX, _iniY,	_iniZ);
    ViewDir = F3dVector(_iniX, _iniY,	_iniZ - 1);
    RightVector = F3dVector (_iniX, _iniY,	_iniZ);
    UpVector = F3dVector (_iniX, _iniY,	_iniZ);
    target = F3dVector (_iniX, _iniY,	_iniZ);

    //Only to be sure:
    RotatedX = RotatedY = RotatedZ = 0.0;
}

void CCamera::Move (SF3dVector Direction)
{
    Position = Position + Direction;
}

void CCamera::RotateX (GLfloat Angle)
{
    Angle /= 10.0;
    RotatedX += Angle;

    ViewDir = ViewDir*cos(Angle*PIdiv180) + UpVector*sin(Angle*PIdiv180);

    ViewDir = Normalize3dVector(ViewDir);

    //now compute the new UpVector (by cross product)
    UpVector = CrossProduct(&ViewDir, &RightVector)*-1;
}

void CCamera::RotateY (GLfloat Angle)
{
    Angle /= 10.0;
    RotatedY += Angle;

    ViewDir = ViewDir*cos(Angle*PIdiv180) + RightVector*sin(Angle*PIdiv180);

    ViewDir = Normalize3dVector(ViewDir);

    //now compute the new RightVector (by cross product)
    RightVector = CrossProduct(&ViewDir, &UpVector);
}

void CCamera::RotateZ (GLfloat Angle)
{
    RotatedZ += Angle;

    //Rotate viewdir around the right vector:
    RightVector = Normalize3dVector(RightVector*cos(Angle*PIdiv180)
                                    + UpVector*sin(Angle*PIdiv180));

    //now compute the new UpVector (by cross product)
    UpVector = CrossProduct(&ViewDir, &RightVector)*-1;
}

void CCamera::RotateObjectX (GLfloat Angle)
{
    Angle /= 10.0;
    RotatedX += Angle;

    SF3dVector tmp;
    tmp = target - Position;

    SF3dVector tangeante;
    tangeante = CrossProduct(&tmp, &RightVector);
    tangeante = Normalize3dVector(tangeante);

    SF3dVector center_dir;
    center_dir = CrossProduct(&RightVector, &tangeante);
    center_dir = Normalize3dVector(center_dir);

    float dist = center_dir * tmp;

    Position = Position + center_dir*(1-cos(Angle*PIdiv180))*dist
            + tangeante*sin(Angle*PIdiv180)*dist;

    ViewDir = ViewDir*cos(Angle*PIdiv180) + UpVector*sin(Angle*PIdiv180);
    ViewDir = Normalize3dVector(ViewDir);

    //now compute the new UpVector (by cross product)
    UpVector = CrossProduct(&ViewDir, &RightVector)*-1;
}

void CCamera::RotateObjectY (GLfloat Angle)
{
    Angle /= 10.0;
    RotatedY += Angle;

    SF3dVector tmp;
    tmp = target - Position;

    SF3dVector tangeante;
    tangeante = CrossProduct(&tmp, &UpVector);
    tangeante = Normalize3dVector(tangeante);

    SF3dVector center_dir;
    center_dir = CrossProduct(&UpVector, &tangeante);
    center_dir = Normalize3dVector(center_dir);

    float dist = center_dir * tmp;

    Position = Position + center_dir*(1-cos(Angle*PIdiv180))*dist
            + tangeante*sin(Angle*PIdiv180)*dist;

    ViewDir = ViewDir*cos(Angle*PIdiv180) - RightVector*sin(Angle*PIdiv180);
    ViewDir = Normalize3dVector(ViewDir);

    //now compute the new RightVector (by cross product)
    RightVector = CrossProduct(&ViewDir, &UpVector);
}

void CCamera::Render( void )
{
    //The point at which the camera looks:
    SF3dVector ViewPoint = Position+ViewDir;

    //as we know the up vector, we can easily use gluLookAt:
    gluLookAt(	Position.x,Position.y,Position.z,
                ViewPoint.x,ViewPoint.y,ViewPoint.z,
                UpVector.x,UpVector.y,UpVector.z);
}

void CCamera::MoveForward( GLfloat Distance )
{
    Position = Position + (ViewDir*-Distance);
}

void CCamera::StrafeRight ( GLfloat Distance )
{
    Position = Position + (RightVector*Distance);
}

void CCamera::MoveUpward( GLfloat Distance )
{
    Position = Position + (UpVector*Distance);
}


