FROM gitpod/workspace-full-vnc

# More information: https://www.gitpod.io/docs/config-docker/

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

#install qt
ARG QT_VERSION=5.15.2
RUN sudo cp /etc/apt/sources.list /etc/apt/sources.list~ \
 && sudo sed -Ei 's/^# deb-src /deb-src /' /etc/apt/sources.list \
 && sudo apt-get update
RUN sudo apt-get -qq build-dep -y qt5-default
RUN sudo apt-get -qq install -y libxcb-xinerama0-dev \
 && sudo apt-get -qq install -y '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev \
 && sudo apt-get -qq install -y perl 

USER root
RUN git clone https://code.qt.io/qt/qt5.git
WORKDIR qt5
RUN git checkout v${QT_VERSION}
RUN perl init-repository --module-subset=default,-qtwebengine
WORKDIR ./qt5-build
RUN ../configure -developer-build -opensource -nomake examples -nomake tests -confirm-license

RUN make -s -j$(nproc)
ENV Qt5_DIR=/home/gitpod/qt5/qt5-build/qtbase/lib/cmake/Qt5

USER gitpod

WORKDIR /home/gitpod/

# lcov and doxygen
RUN sudo apt-get -qq install -y lcov doxygen graphviz \
 && rm -rf /var/lib/apt/lists/*

# conan
RUN pip3 install conan
RUN conan profile new default --detect

