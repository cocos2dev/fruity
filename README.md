
# fruity

fruity is a Easy-To-Use Sprite animation player that can help you creating sprites for your Games. You can also convert your animation to a Spritesheet when its done.




# Installation

Heres a Guide on how to Install fruity on your Computer...

## Install MinGW-x64 on Windows
### Download-Link
[Here](https://github.com/msys2/msys2-installer/releases/download/2023-05-26/msys2-x86_64-20230526.exe) is a direct Link to the MinGW Installer.

### Install Toolchain
After the Installation is done run 'MSYS2 MSYS' on your Computer.

Run the following command in the MSYS-Console: \
`pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain`

Accept the default number of packages by pressing Enter.

Enter Y when prompted whether to proceed with the installation.

Add the path of your MinGW-w64 bin folder to the Windows PATH environment variable.

[Click here for a more detailed Tutorial on how to install MinGW](https://code.visualstudio.com/docs/languages/cpp/#_example-install-mingwx64-on-windows)

## Adding fruity.exe to PATH

Create a new directory in where you drop the fruity.exe.

Copy the Path for the Directory.

Add the Path in your Systemvariable called `PATH`

Open Command Prompt and type `fruity` to see if it worked.

## Usage

### General

fruity can only be used in the Command Prompt. Type in `fruity help` for a help list (no shit)

### Creating a new Animation

Heres a Example on how to create a new Animation that you can work with

`fruity create <name> <ms>` \
`fruity create animationName 85`

This command will create a new Directory called `animationName` and the number is the time in ms between frames.

### Playing your Animation

#### Note: You can only Play your animation if you already have 2 PNG files in the directory.

Heres a Example on how to play your Animation:

`fruity play <name>` \
`fruity play animationName`

This command will search for a directory called `animationName` and checks for a `PROP.txt`
It will then read the `pbDelay` value inside `PROP.txt` and will use that Value to play your animation on a SDL2 Window.

### Converting your Animation to a Spritesheet

Heres a Example on how to create a Spritesheet for you Animation:

`fruity sheet <name>` \
`fruity sheet animationName`

This command will create a Spritesheet and .plist file for your animation that you can easily use inside your games. If you work with Game Engines like Cocos2d-x you can use the .plist file for sprite sheets. This file is most likely not required for modern game engines like Unity.

### Refreshing Animation PROP file

Incase your animation will not work (playing, sheeting, etc) because of an fruity Update, you can do this to fix your PROP-file:

`fruity refresh <name>` \
`fruity refresh animationName`

This command will fix the PROP.txt file of your Animation so it works again.
## Configuring fruity
When you run fruity for the first time, it will generate a `config.txt`.
You can open it by typing `fruity config` inside the Command Prompt.

Inside you will find 2 Values...
`windowSize` and `windowFocus`

`windowSize` will determine the size of the Players SDL2 Window.
`windowFocus` when set to 1, it will always show the Players SDL2 window on top of every other window even if its not in focus.
