#!/bin/bash


sudo apt install libglfw3 libglfw3-dev libfreetype6-dev -y
sudo apt-get install libglm-dev
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
sudo mv stb_image.h /usr/local/include/
 git clone https://github.com/nlohmann/json.git /tmp/json \
    && cd /tmp/json \
    && mkdir build && cd build \
    && cmake .. \
    && make install
