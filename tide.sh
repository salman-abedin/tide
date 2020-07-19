#!/bin/sh

SHOWCURSOR="\033[?25h"
HIDECURSOR="\033[?25l"
ENABLEWRAP="\033[?7h"
DISABLEWRAP="\033[?7l"
CLEAR="\033[2J\033[H"
cursor=1
items=$(transmission-remote -l | sed '1d;$d' | wc -l)

getkey() {
    stty -icanon -echo
    dd bs=1 count=1 2> /dev/null
    stty icanon echo
}

quit() {
    printf "%b" "$CLEAR$SHOWCURSOR"
    kill -9 $(pgrep -f "$0")
}

handleinput() {
    case $1 in
        d) : ;;
        p) : ;;
        s) : ;;
        l) [ "$cursor" -lt "$items" ] && cursor=$((cursor + 1)) ;;
        k) [ "$cursor" -gt 1 ] && cursor=$((cursor - 1)) ;;
        n) quit ;;
    esac
}

goto() {
    printf "%b" "\033[${1};${2}H"
}

mark() {
    printf "\033[7m"
    echo "$@"
    printf "\033[27m"
}

showui() {
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
    printf "%b" "$DISABLEWRAP$HIDECURSOR$CLEAR"
    LINES=$(stty size | cut -d' ' -f1)
    COLUMNS=$(stty size | cut -d' ' -f2)
}

trap 'setscreen' WINCH

setscreen
header Tide: Transmission TUI
footer l:Down k:Up n:Quit
while :; do
    showui
    handleinput "$(getkey)"
done
