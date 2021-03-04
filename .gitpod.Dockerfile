FROM gitpod/workspace-full-vnc

# More information: https://www.gitpod.io/docs/config-docker/

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

#install qt depndencies
RUN sudo cp /etc/apt/sources.list /etc/apt/sources.list~ \
 && sudo sed -Ei 's/^# deb-src /deb-src /' /etc/apt/sources.list \
 && sudo apt-get update
RUN sudo apt-get -qq build-dep -y qt5-default
RUN sudo apt-get -qq install -y libxcb-xinerama0-dev \
 && sudo apt-get -qq install -y '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev \
 && sudo apt-get -qq install -y perl 

#install qt
COPY --from=bbvch/qt:5.14.2 /usr/local/Qt /usr/local/Qt
ENV Qt5_DIR=/usr/local/Qt

# lcov and doxygen
RUN sudo apt-get -qq install -y lcov doxygen graphviz

# conan
RUN pip3 install conan
RUN conan profile new default --detect

