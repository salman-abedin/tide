# tide: Minimal Transmission CLI client

tide is a minimal TUI for the BitTorrent client [Transmission](https://transmissionbt.com/).  
tide is the successor to [tide.sh](https://github.com/salman-abedin/tide.sh)

![](https://gitlab.com/salman-abedin/assets/-/raw/master/tide.gif)

## Features

-  Tiny & Fast
-  Vim bindings (consult **config.h**)
-  Stopping torrents
-  Starting torrents
-  Deleting torrents (Including Files)
-  Remote access (through patching)

## Libraries

-  ncurses
-  pthread

## Installation

```sh
git clone https://github.com/salman-abedin/tide.git && cd tide && sudo make install
```

## Patches

-  **Remote Access**. (dependencies: ssh)

```sh
cd tide
patch -s -p0 < tide-remote_access-8.0.diff    # Add the feature
patch -s -R  < tide-remote_access-8.0.diff # Remove the feature
sudo make install                      # Reinstall
```

## FAQ

### How are you sending the magnet link to the daemon?

Using **xdg-open** (shamefully).  
Here is my setup (Exhibit A of my shamefulness) ...

https://github.com/salman-abedin/magpie/blob/master/.config/mimeapps.list  
https://github.com/salman-abedin/magpie/blob/master/.local/share/applications/transmission.desktop  
https://github.com/salman-abedin/alfred/blob/master/src/torrent.sh

## Update

```sh
git pull --no-rebase && sudo make install
```

## Uninstallation

```sh
sudo make uninstall
```

## Major Revisions

-  **15/08/20**:- Rewrote the shellscript in C
-  **02/09/20**:- Added remote access patch
-  **06/12/20**:- Added some major performance optimizations

## TODOs

-  Delete confirmation
-  Marking
-  Direct magnet adding

---

## Repos you might be interested in

| Name                                                    | Description            |
| ------------------------------------------------------- | ---------------------- |
| [bolt](https://github.com/salman-abedin/bolt)           | The launcher wrapper   |
| [uniblocks](https://github.com/salman-abedin/uniblocks) | The status bar wrapper |
| [puri](https://github.com/salman-abedin/puri)           | Minimal URL launcher   |
| [devour](https://github.com/salman-abedin/devour)       | X11 window swallower   |
| [crystal](https://github.com/salman-abedin/crystal)     | The transparent setup  |
| [Magpie](https://github.com/salman-abedin/magpie)       | The dotfiles           |
| [Alfred](https://github.com/salman-abedin/alfred)       | The scripts            |

## Contact

SalmanAbedin@disroot.org
