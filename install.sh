#!/bin/bash
set -e
git clone https://github.com/SrTortugardo/CatRunner-Linux
cd CatRunner-Linux/src/
make
sudo make install
make install-desktop
echo "All should be ready!"
