FROM ubuntu:18.04
RUN apt-get update
RUN apt-get install -y git

# https://linuxize.com/post/how-to-install-opencv-on-ubuntu-18-04/
RUN apt-get install -y build-essential cmake git pkg-config libgtk-3-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    gfortran openexr libatlas-base-dev python3-dev python3-numpy \
    libtbb2 libtbb-dev libdc1394-22-dev
RUN apt-get install -y lsb-release freeglut3-dev
WORKDIR /root
RUN pwd
RUN git clone https://github.com/opencv/opencv.git
RUN git clone https://github.com/opencv/opencv_contrib.git
RUN cd ./opencv; git checkout 3.4.9
RUN cd ./opencv_contrib; git checkout 3.4.9
RUN mkdir ./opencv/build 
RUN cd ./opencv/build; cmake \
    -D CMAKE_BUILD_TYPE=RELEASE  \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D INSTALL_C_EXAMPLES=ON  \
    -D INSTALL_PYTHON_EXAMPLES=ON  \
    -D OPENCV_GENERATE_PKGCONFIG=ON \
    -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules  \
    -D BUILD_EXAMPLES=ON -D BUILD_opencv_world=ON  \
    ..
RUN cd ./opencv/build; make -j8
RUN cd ./opencv/build; make install

RUN apt-get -y install unzip
COPY Terabee-3Dcam-SDK-1.6.0.0-Linux.zip .
RUN unzip Terabee-3Dcam-SDK-1.6.0.0-Linux.zip
RUN cd ./Terabee_3Dcam_SDK_1.6.0.0_Linux/TERABEE-Linux-x64-OpenNI2.2; bash install.sh

