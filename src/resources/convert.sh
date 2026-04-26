#!/bin/bash

rm -f frame_*.png

magick pixel-cat.gif -coalesce frame_%03d.png
