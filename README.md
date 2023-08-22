ecapfil
=======
An ART file viewer and interpreter for `ECAPFIL.TXT` files, that automate the
generation of tiles from images in a textual specification. The images can be
of any format supported by the SDL2_image libary (e.g. GIF, PCX, PNG), as long
as the corresponding pixel data is in chunky format.

ART files are used by applications powered by the BUILD engine for displaying
textures and sprites. The `ECAPFIL.TXT` file format is inspired by `CAPFIL.TXT`
file that the `EDITART` editor uses to script the import of tiles.

The BUILD engine is used by a number of popular games from the mid 90s, such as
Duke Nukem 3D, Shadow Warrior, Blood and Redneck Rampage.

Prerequisites
=============
* An ANSI C compiler and Make
* [SDL2](http://libsdl.org)
* [SDL2_image](https://wiki.libsdl.org/SDL2_image)

Installing the project
======================
You can build and install the project from source as follows:

```bash
$ make
$ make install
```

To use the project on build big-endian systems, edit `src/Makefile` and
uncomment the big endian flag. Then the I/O operations will automatically byte
swap all numeric values.

If you have the [Nix package manager](http://nixos.org/nix) installed, then you
can install the project with:

```bash
$ nix-env -f default.nix -i ecapfil
```

Usage
=====
Viewing tiles in ART files
--------------------------
The most basic use case is to use the tool as a viewer for tiles. To use it,
switch the current working directory to a folder that provides graphics data,
such as the `DUKE3D` folder:

```bash
$ cd duke3d
```

Then, the viewer can be opened with the following command:

```bash
$ ecapfil
```

The viewer can be controlled with the following keys:

* `UP`. Move to the previous tile.
* `DOWN`. Move to the next tile.
* `PAGEUP` Move to the following page of tiles
* `PAGEDOWN`. Move to the previous page of tiles
* `HOME`. Move to the first tile.
* `END`. Move to the last file.
* `U`. Process the `ECAPFILE.TXT` file
* `P`. Prints a textual representation of the tile catalog.
* `ESC`. Exits the viewer

Pretty printing the tile catalog
--------------------------------
It is also possible to directly invoke the pretty print feature on the
command-line:

```bash
$ ecapfil --operation pretty-print
```

The above command displays a textual representation of the entire tile catalog.

Processing an ECAPFIL.TXT file
------------------------------
We can also directly process an `ECAPFIL.TXT` file with the following command:

```bash
$ ecapfil --operation process-capfil
```

Advanced options
----------------
You can change the filename of the palette and capfil files:

```bash
$ ecapfil --palette-file MYPAL.DAT --capfil-file CAPFIL2.TXT
```

More options can be discovered by using the `--help` parameter.

ECAPFIL.TXT
===========
An `ECAPFIL.TXT` file has the following format:

```
5121,tree2.gif,0,0,222,256
# Water
5122,mwater1.gif,0,0,128,128,animSpeed=6;animateType=2;animateNumber=2
5123,mwater2.gif,0,0,128,128
5124,mwater3.gif,0,0,128,128
```

An `ECAPFIL.TXT` is a linefeed separated file in which each line is comma `,`
separated. The above file format is compatible with the original `CAPFIL.TXT`
file format used by the `EDITART` editor, that is included with the BUILD
editor.

Each column has the following purpose:

* Tile number
* Path to the image to import. Contrary to the `EDITART` (that only supports PCX
  and GIF files), this tool can work with any file format supported by the
  `SDL2_image` library.
* The top X coordinate of the surface to extract
* The top Y coordinate of the surface to extract
* The bottom X coordinate of the surface to extract
* The bottom Y coordinate of the surface to extract
* The last column is optional and specifies the animation properties of a tile.
  Animation properties are semi-colon separated: `;`. This is an extension to
  the original `CAPFIL.TXT` format implemented by this tool.

The following animation properties are supported:
* `animSpeed`. Specifies the animate speed. It can be a number between `0` and `255`
* `animateType`. Specifies the animation type. It can be one of the following
  values:
  * `0`: No animation
  * `1`: Oscillate
  * `2`: Forwards
  * `3`: Backwards
* `animateNumber`. Specifies the amount of tiles that are part of the animation.
  It can be a number between `0` and `63`

In addition to data, lines that start with a `#` symbol are considered comments.
These features are also an extension to the regular `CAPFIL.TXT` format.

License
=======
This library is available under the MIT license
