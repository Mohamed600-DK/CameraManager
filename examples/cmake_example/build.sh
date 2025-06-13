#! /bin/bash
BUILD_TYPE=Debug
LIBRARY_DIR=/home/mohamed/Data/My_Files/GitHub_Repos/CameraManager
BUILD_DIR=./build/$BUILD_TYPE
LIB_BUILD_DIR=$BUILD_DIR/lib_build
pkg_instal_dir_path="$BUILD_DIR/dep"
OpenCV_DIR="/home/mohamed/Data/My_Files/GitHub_Repos/CameraManager/cv_build"

if [ -e "$BUILD_DIR" ]; then 
    rm -r $BUILD_DIR/*
fi

camera_manager_DIR="$pkg_instal_dir_path/lib/cmake"


cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$pkg_instal_dir_path -DOpenCV_DIR=$OpenCV_DIR -B $LIB_BUILD_DIR  $LIBRARY_DIR
make -C $LIB_BUILD_DIR
cmake --install $LIB_BUILD_DIR

cmake -Dcamera_manager_DIR=$camera_manager_DIR -DOpenCV_DIR=$OpenCV_DIR -DCMAKE_BUILD_TYPE=$BUILD_TYPE  -B $BUILD_DIR .

cmake --build $BUILD_DIR
./$BUILD_DIR/example