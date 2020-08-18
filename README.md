![](preview.gif)

# tide: A minimal Transmission CLI client

tide is a minimal TUI for the BitTorrent client [Transmission](https://transmissionbt.com/).  
tide is the successor to [tide.sh](https://github.com/salman-abedin/tide.sh)

## Features

-  Tiny & Fast
-  Vim bindings
-  Stopping torrents
-  Starting torrents
-  Deleting torrents (Including Files)
-  Remote access (consult **config.h**)

## Libraries

-  ncurses

## Installation

```sh
git clone https://github.com/salman-abedin/tide.git && cd tide && sudo make install
```

## FAQ

### How are you sending the magnet link to the server?

Using **xdg-open** (shamefully).  
Here is my setup (Exhibit A of my shamefulness) ...  

https://github.com/salman-abedin/magpie/blob/master/.config/mimeapps.list  
https://github.com/salman-abedin/magpie/blob/master/.local/share/applications/transmission.desktop  
https://github.com/salman-abedin/alfred/blob/master/src/torrent.sh

## Uninstallation

```sh
sudo make uninstall
```

## Patches

-  **27/07/20**:- Added scrolling support
-  **15/08/20**:- Rewrote the shellscript in C

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
| [devour](https://github.com/salman-abedin/devour)       | Terminal swallowing    |
| [crystal](https://github.com/salman-abedin/crystal)     | The transparent setup  |
| [Magpie](https://github.com/salman-abedin/magpie)       | The dotfiles           |
| [Alfred](https://github.com/salman-abedin/alfred)       | The scripts            |

## Contact

SalmanAbedin@disroot.org
