# DDE Smart Directory Dock Plugin for Deepin

Support V20 & V15.11

Inspired by Smart Folder on macOS.

You can set a list of folders you want to watch, and this plugin can show the file list in this files ordered by Modification time.

* Left double click to open file
* drag & move support
* right click to open folder
* quick folder list

## Screenshot

<img src="./screenshots/screenshot1.png" width="400px" /> <img src="./screenshots/screenshot1-2.png" width="400px" />
<img src="./screenshots/screenshot2.png" width="400px" /> <img src="./screenshots/fan_style.png" width="400px" />

> fan style is hardcoded so it is coded for panel.

## How to install (Deepin V20)

Download the release file and put the `.so` file to the plugin path.

Or install the `.deb` package

## How to Run

* git clone https://github.com/SeptemberHX/dde-smart-dir.git
* `cd dde-smart-dir`
* `mkdir build`
* `cd build`
* `cmake ..`
* `make`
* `cp libdde-smart-dir.so /usr/lib/dde-dock/plugins`
* `killall dde-dock`
