FROM arm64v8/debian

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install chromium-driver -y

RUN apt-get install g++ -y
RUN apt-get install make -y
RUN apt-get install binutils -y
RUN apt-get install cmake -y
RUN apt-get install libboost-system-dev -y
RUN apt-get install libssl-dev -y
RUN apt-get install zlib1g-dev -y
RUN apt-get install libcurl4-openssl-dev -y
RUN apt-get install cmake git -y

WORKDIR /home
RUN git clone -b v1.7.2 --depth 1 https://github.com/reo7sp/tgbot-cpp.git
WORKDIR /home/tgbot-cpp
RUN cmake .
RUN make -j 8
RUN make install 

RUN apt-get install pkg-config
RUN apt-get install libjsoncpp-dev
