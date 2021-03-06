cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE "RELEASE") 

# FIND_PACKAGE(eigen3)
#include_directories(${EIGEN3_INCLUDE_DIRS})

include_directories(/usr/include/eigen3)
FIND_PACKAGE(orocos_kdl)

# QT4 usage
FIND_PACKAGE(Qt4)
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
INCLUDE(${QT_USE_FILE})

# MOC_HDRS set to all files with Q_OBJECT in it
SET(MOC_HDRS  GlWidget.h  QtWindow.h)

# Regular cpp sources
SET(CPP_SRCS  QtWindow.cpp GlWidget.cpp Graphics.cpp helpers.cpp gfxl/Base.cpp gfxl/Model.cpp  
              model/KinematicsModel.cpp model/Movement.cpp model/Poly3Profile.cpp)

# Run moc on header files with Q_OBJECT
QT4_WRAP_CPP( MOC_SRCS ${MOC_HDRS} )

# Builds up all
ADD_EXECUTABLE(sim_RU_link main.cpp ${CPP_SRCS} ${MOC_SRCS})

# Link libraries
TARGET_LINK_LIBRARIES(sim_RU_link ${QT_LIBRARIES} GLU orocos-kdl)
