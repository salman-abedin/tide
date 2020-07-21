![](preview.gif)

# tide: A minimal CLI Transmission Client

tide is a minimal TUI for controlling Transmission daemon

## Dependencies

-  **transmission-cli**

-  grep, sed, sort, uniq, tr, wc, cut, stty, head, read, printf, echo, seq

## Installation

```sh
git clone https://github.com/salman-abedin/puri.git && cd puri && sudo make install
```

## Usage

#### Tmux

```tmux
bind u run 'tmux capture-pane -p > /tmp/tb' \; \
  new-window -n puri 'puri /tmp/tb && rm -f /tmp/tb'
```

#### Mutt/Neomutt

```muttrc
macro index,pager o "<pipe-entry>tee /tmp/muttb<enter><shell-escape>puri /tmp/muttb<enter>"
macro index,pager o "<pipe-message>tee /tmp/muttb<enter><shell-escape>puri /tmp/muttb<enter>"
```

#### Shell

```sh
puri [FILE...]
```

## Uninstallation

```sh
sudo make uninstall
```

## Similar Projects

-  [urlscan](https://github.com/firecat53/urlscan)
-  [urlview](https://github.com/sigpipe/urlview)

---

## Repos you might be interested in

[Bolt](https://github.com/salman-abedin/bolt)
: The lightning fast workflow creator

[Uniblocks](https://github.com/salman-abedin/uniblocks)
: The status bar wrapper

[Crystal](https://github.com/salman-abedin/crystal)
: The transparent setup

[Magpie](https://github.com/salman-abedin/magpie)
: The dotfiles

[Alfred](https://github.com/salman-abedin/alfred)
: The scripts

[Devour](https://github.com/salman-abedin/devour)
: Terminal swallowing

## Contact

SalmanAbedin@disroot.org
