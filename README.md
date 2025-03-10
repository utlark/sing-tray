# sing-tray

A small Linux utility for convenient management of **sing-box** through the system tray

## Features

- Dynamic modification of the configuration file
- Convenient launch of **sing-box** via the tray icon
- Automatic proxy configuration for mixed input in **Gnome** and **KDE** environments

## Dependencies

- **Qt6** with the *Core*, *Gui*, and *Widgets* components
- System utilities: `getcap`, `setcap`, and `pkexec`

## Launch

1. Place the [sing-box](https://github.com/SagerNet/sing-box/releases/latest) core in the `Core` folder
2. Create JSON files with **sing-box** configurations in the `Routes` folder

## Usage

After launch, an icon appears in the system tray through which you can:

- Start or stop **sing-box**
- Modify the **sing-box** configuration on the fly
- Enable application autostart at system startup

## Acknowledgements

Special thanks to [MatsuriDayo/nekoray](https://github.com/MatsuriDayo/nekoray) – parts of the code and the application icon were borrowed from this project
