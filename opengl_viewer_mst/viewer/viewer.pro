#-------------------------------------------------
#
# Project created by QtCreator 2013-01-09T01:18:19
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT           += core gui opengl widgets
TARGET        = bin/viewer
TEMPLATE      = app

#CONFIG += static
CONFIG += console

SOURCES      += \
    src/main.cpp \
    src/camera.cpp

HEADERS      += \
    include/paramframe.h \
    include/paintingmesh.h \
    include/ctrlwidget.h \
    include/camera.h \
    include/interfaceUI.h

FORMS        +=

#CUDA FILE
#CUDA_SOURCES += cuda_code.cu
#cuda_code_nay.cu
OTHER_FILES  +=

######################################################
#
# For ubuntu, add environment variable into the project.
# Projects->Build Environment
# LD_LIBRARY_PATH = /usr/local/cuda/lib
#
######################################################


CUDA_FLOAT    = float
CUDA_ARCH     = -gencode arch=compute_50,code=sm_50

#win32:{
#  #Do'nt use the full path.
#  #Because it is include the space character,
#  #use the short name of path, it may be NVIDIA~1 or NVIDIA~2 (C:/Progra~1/NVIDIA~1/CUDA/v5.0),
#  #or use the mklink to create link in Windows 7 (mklink /d c:\cuda "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.0").
#  #CUDA_DIR      = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v5.0"
#  CUDA_DIR      = "C:/Progra~1/NVIDIA~2/CUDA/v5.0"
#  QMAKE_LIBDIR += $$CUDA_DIR/lib/Win32
#  INCLUDEPATH  += $$CUDA_DIR/include C:\QT\qt-everywhere-opensource-src-4.8.4\include C:\QT\qt-everywhere-opensource-src-4.8.4\include\QtOpenGL
#  INCLUDEPATH  += include ../../basic_components/components/include
#  LIBS         += -lcuda -lcudart
#  QMAKE_LFLAGS_DEBUG    = /DEBUG /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib
#}

#for qiao windows
  #Do'nt use the full path.
  #Because it is include the space character,
  #use the short name of path, it may be NVIDIA~1 or NVIDIA~2 (C:/Progra~1/NVIDIA~1/CUDA/v5.0),
  #or use the mklink to create link in Windows 7 (mklink /d c:\cuda "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.0").
  CUDA_DIR      = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.0"
#  CUDA_DIR      = "C:/Progra~1/NVIDIA~2/CUDA/v7.0"
#  QMAKE_LIBDIR += $$CUDA_DIR/lib/x64
  QMAKE_LIBDIR += $$CUDA_DIR/lib/x64 $$LIBS_OPERATORS_DIR $$LIBS_COMPONENTS_DIR
  INCLUDEPATH  += $$CUDA_DIR/include C:\Qt\Qt5.7.0\5.7\msvc2013_64\include C:\Qt\Qt5.7.0\5.7\msvc2013_64\include\QtCore C:\Qt\Qt5.7.0\5.7\msvc2013_64\include\QtXml C:\Qt\Qt5.7.0\5.7\msvc2013_64\include\QtOpenGL  C:/boost_1_59_0
  INCLUDEPATH  += ../include
  INCLUDEPATH  += include ../../basic_components\components\include
  INCLUDEPATH  += include ../../optimization_operators\operators\include
  INCLUDEPATH  += include ../../adaptive_meshing\meshing\include



#$$QTDIR/include/QtOpenGL
#  LIBS         += -L$$CUDA_DIR/lib/x64 -lcuda -lcudart
  LIBS         += -lcuda -lcudart
  LIBS += opengl32.lib glu32.lib

#-llibOperators
# -L$$CUDA_DIR/lib/Win32

# Add the necessary libraries
#  CUDA_LIBS = cuda cudart
#  NVCC_LIBS = $$join(CUDA_LIBS,' -l','-l', '')
  QMAKE_LFLAGS_DEBUG    = /DEBUG /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib
#  QMAKE_LFLAGS_RELEASE  =         /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib



unix:{

  ##############################################################################
  # Here to add the specific QT and BOOST paths according to your Linux system.
  # For H.W's system
  INCLUDEPATH  += ../include /usr/local/Trolltech/Qt-4.8.4/include C:/boost_1_55_0  /home/abdo/boost_1_57_0 /home/ben/Downloads/boost_1_57_0
  # For A.M's system
  INCLUDEPATH  += /home/mansouri/Qt/5.4/gcc_64/include

  CUDA_DIR      = /usr/local/cuda-6.5
  QMAKE_LIBDIR += $$CUDA_DIR/lib64
  INCLUDEPATH  += $$CUDA_DIR/include
  LIBS += -lcudart -lcuda -lglut -lGLU
  QMAKE_CXXFLAGS += -std=c++0x

  INCLUDEPATH  += ../../basic_components/components/include
  INCLUDEPATH  += ../../optimization_operators/operators/include
  INCLUDEPATH  += ../../adaptive_meshing/meshing/include
  INCLUDEPATH  += include
  INCLUDEPATH  += $$CUDA_DIR/samples/common/inc

}

DEFINES += "CUDA_FLOAT=$${CUDA_FLOAT}"

NVCC_OPTIONS = --use_fast_math -DCUDA_FLOAT=$${CUDA_FLOAT}
#NVCCFLAGS     = --compiler-options -fno-strict-aliasing -use_fast_math --ptxas-options=-v
QMAKE_EXTRA_COMPILERS += cuda

CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')

CONFIG(release, debug|release) {
  OBJECTS_DIR = ./release
  cuda.commands = $$CUDA_DIR/bin/nvcc $$NVCC_OPTIONS $$CUDA_INC $$LIBS --machine 64 $$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
}
CONFIG(debug, debug|release) {
  OBJECTS_DIR = ./debug
  cuda.commands = $$CUDA_DIR/bin/nvcc $$NVCC_OPTIONS $$CUDA_INC $$LIBS --machine 64 $$CUDA_ARCH -c -D_DEBUG -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
}

cuda.input = CUDA_SOURCES
cuda.output = $${OBJECTS_DIR}/${QMAKE_FILE_BASE}_cuda.o
