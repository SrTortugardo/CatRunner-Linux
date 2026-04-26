# Non oficial "port" of [CatRunner](https://github.com/kuma-sannn/CatRunner/tree/main) To GNU/Linux
**NOTE** : If you like the program, [Please support creator](https://htmlpreview.github.io/?https://github.com/kuma-sannn/CatRunner/blob/main/docs/SPONSORS.html)
This isn't a port, is a full rewrite on C, the reasons of making a rewrite are :
- The project is made on C# : C# on GNU/Linux, works, but support is not the best
- The code is designed to run on windows : Even if C# was a good idea, anyway we would need a full rewrite for almost every file.

## TODO
- Making the cat run on Waybar! : [WIP]

## Installing
There are no binarys for this, you need to compile it.
For Gentoo Users you can find an working ebuild on the [Pear Overlay](https://github.com/SrTortugardo/Pear-Overlay) and install this more easily :
```
sudo emerge --ask catrunner
```

### Deps
You only need X11 libs and nothing more, everything else is from this program. You probably have this
Debian/Ubuntu :
```
sudo apt install libx11-dev
```
Arch :
```
sudo pacman -S libx11
```

## After installing deps you can run this script which will install all itself
Just run :
```curl -fsSL https://github.com/SrTortugardo/CatRunner-Linux/blob/main/install.sh | bash```
And all should be ready!

## Thanks for reading!
If you experience an issue, feel free of open an Issue on **THIS REPO**.
