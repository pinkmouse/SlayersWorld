FROM debian:stretch
MAINTAINER contact@slayersworld.com

# Install basic packages
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    cmake \
    make \
    git
    
# Install project packages
RUN apt-get install -y \
    libx11-xcb-dev \
    libglew-dev \
    freeglut3-dev \
    libjpeg-dev \
    libfreetype6-dev \
    libxrandr-dev \
    libglew-dev \
    libsndfile1-dev \
    libopenal-dev \
    libudev-dev \
    libmysqlclient-dev \
    mysql-common


# Install SFML library
RUN git clone -b 2.4.x https://github.com/SFML/SFML.git sfml; \
cd sfml; \
mkdir build; \
cd build; \
cmake -DCMAKE_BUILD_TYPE=Release ../; \
make install .

# Install SlayersWorld Server
RUN git clone -b master https://c740de879739d230a4d377347851f4d141a11613@github.com/pinkmouse/SlayersWorld.git SlayersWorld; \
cd SlayersWorld/Server; \
mkdir build; \
cd build; \
cmake -DCMAKE_BUILD_TYPE=Release ../; \
make

CMD cd /SlayersWorld/Server/build/; ./SWServer
