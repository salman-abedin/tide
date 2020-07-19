#!/bin/sh
#
# Minimal Transmission TUI

SHOWCURSOR="\033[?25h"
HIDECURSOR="\033[?25l"
ENABLEWRAP="\033[?7h"
DISABLEWRAP="\033[?7l"
CLEAR="\033[2J\033[H"

export ITEMS=0
export cursor=1

quit() {
    printf "%b" "$CLEAR$SHOWCURSOR"
    kill -- -$$
}

handleinput() {
    case $1 in
        l) [ "$cursor" -lt "$ITEMS" ] && export cursor=$((cursor + 1)) ;;
        k) [ "$cursor" -gt 1 ] && export cursor=$((cursor - 1)) ;;
        n) quit ;;
    esac
}

getkey() {
    stty -icanon -echo
    dd bs=1 count=1 2> /dev/null
    stty icanon echo
}

showui() {
    ITEMS=$(transmission-remote -l | sed '1d;$d' | wc -l)
    goto 3 0
    transmission-remote -l |
        sed '1d;$d' |
        while read -r line; do
            i=$((i + 1))
            if [ "$i" = "$cursor" ]; then
                mark "$line"
            else
                echo "$line"
            fi
        done
}

mark() {
    printf "\033[7m"
    echo "$@"
    printf "\033[27m"
}

goto() { printf "%b" "\033[${1};${2}H"; }

header() {
    goto 1 "$((COLUMNS / 2 - 10))"
    mark "$@"
    printf "\n\n\n"
}

footer() {
    goto "$((LINES - 1))" "$((COLUMNS / 2 - 10))"
    mark "$@"
}

setscreen() {
    printf "%b" "$HIDECURSOR$CLEAR"
    LINES=$(stty size | cut -d' ' -f1)
    COLUMNS=$(stty size | cut -d' ' -f2)
}

trap 'setscreen' WINCH
trap 'quit' INT

setscreen
header Tide: Transmission TUI
footer l:Down k:Up n:Quit

while :; do
    showui
    sleep 1
done &

while :; do
    handleinput "$(getkey)"
done
