# QtWebMConverter
QtWebMConverter is a simple, cross-platform, Qt-based frontend for ffmpeg aimed at making WebM videos.

Currently it supports VP8/VP9 and Vorbis/Opus in a WebM container. It only supports CRF encoding mode (because that's the one you want to use 9 out of 10 times anyway). More support (including other containers/codecs) might be added in the future.

The Windows version comes with a copy of ffmpeg included.

## Usage

[Download a release](https://github.com/Compizfox/QtWebMConverter/releases) or clone the repository.

### Windows
Extract the ZIP archive and run `QtWebMConverter.exe`. That's all.

### Linux

Before building, you should have the following prerequisites installed: 

- Build tools and compiler
- Qt5
- ffmpeg

On Debian and Ubuntu, you can use the following command to install them:

    # apt install build-essential qt5-default qt5-qmake qttools5-dev ffmpeg

On Arch Linux, you can use this:

    # pacman -S base-devel qt5 ffmpeg

To build QtWebMConverter, do

    # qmake
    # make install

## Screenshots

Windows 8.1

![](https://host.tuxplace.nl/screenshots/2016-03-28-23-25-38.png)

Linux (KDE, Breeze theme)

![](https://host.tuxplace.nl/screenshots/Screenshot_20160328_232817.png)
