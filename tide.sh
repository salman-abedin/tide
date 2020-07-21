#!/bin/sh
#
# Minimal Transmission TUI

# ENABLEWRAP="\033[?7h"
# DISABLEWRAP="\033[?7l"

cursor=/tmp/tide_cursor
mark=/tmp/mark

quit() {
    printf "\033[?25h\033[2J\033[H"
    kill -- -$$
}

getkey() {
    CURRENT_TTY_SETTINGS=$(stty -g)
    stty -icanon -echo
    head -c1
    stty "$CURRENT_TTY_SETTINGS"
}

handleinput() {
    sendargs() { transmission-remote "$@" > /dev/null; }
    case $(getkey) in
        p)
            sendargs -t "$(cat $mark)" -S
            ;;
        s)
            sendargs -t "$(cat $mark)" -s
            ;;
        d)
            sendargs -t "$(cat $mark)" -r
            ;;
        k)
            ITEMS=$(transmission-remote -l | sed '1d;$d' | wc -l)
            [ "$(cat $cursor) " -gt 1 ] &&
                echo $(($(cat $cursor) - 1)) > "$cursor"
            ;;
        l)
            ITEMS=$(transmission-remote -l | sed '1d;$d' | wc -l)
            [ "$(cat $cursor) " -lt "$ITEMS" ] &&
                echo $(($(cat $cursor) + 1)) > "$cursor"
            ;;
        n) quit ;;
    esac
}

drawtorrents() {
    goto 5 0
    i=0
    transmission-remote -l | sed '1d;$d' |
        while read -r line; do
            i=$((i + 1))
            if [ "$i" = "$(cat $cursor)" ]; then
                mark "$line"
            else
                echo "$line"
            fi
        done
}

mark() {
    printf "\033[7m"
    echo "$1"
    printf "\033[27m"
    echo "${1%% *}" > "$mark"
}

goto() { printf "\033[%s;%sH" "$1" "$2"; }

setfooter() {
    goto "$((LINES - 1))" "$((COLUMNS / 2 - 10))"
    mark "h:Quit j:Down k:Up l:launch"
}

setborder() {
    goto 3 0
    for i in $(seq "$COLUMNS"); do printf "%s" "-"; done
    goto "$((LINES - 2))" 0
    for i in $(seq "$COLUMNS"); do printf "%s" "-"; done
}

setheader() {
    goto 2 "$((COLUMNS / 2 - 10))"
    mark "tide: Transmission Client"
    printf "\n\n\n"
}

setscreen() {
    printf "\033[?25l\033[2J\033[H"
    LINES=$(stty size | cut -d' ' -f1)
    COLUMNS=$(stty size | cut -d' ' -f2)
}

init() {

    # if ! pidof transmission-daemon > /dev/null; then
    #     transmission-daemon
    #     sleep 1
    # fi

    echo 1 > "$cursor"
}

main() {
    init
    setscreen
    setborder
    setheader
    setfooter

    trap 'quit' INT EXIT
    trap 'notify-send wc' WINCH

    while :; do
        drawtorrents
        sleep 1
    done &

    while :; do
        handleinput
        drawtorrents
    done

}
main
