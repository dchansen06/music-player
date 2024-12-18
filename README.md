[![Build](https://github.com/dchansen06/music-player/actions/workflows/build.yml/badge.svg)](https://github.com/dchansen06/music-player/actions/workflows/build.yml)

# music-player
A C++ music player built off of SDL2 for playing and controlling music from the terminal

## License information
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

## Stability
This is not yet stable or finalized software.

Please report any bugs or security issues.

## Installation
Install the required dependencies with

```$ sudo apt-get install -y --no-install-suggests --no-install-recommends libsdl2-mixer-dev```

Then simply run

```$ make all```

Use the client and server programs as you would expect, as this is not finalized the runtime instructions are not yet stable, please review the source code for further instructions (sorry).

## To-Do
* Fundamentally rewrite client/server
* Basic GUI functionality
* Better comments (sorry)
* Better documentation
* Divide into more discrete functions and classes
* Setup a testing suite
