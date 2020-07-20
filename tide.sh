#!/bin/sh
#
# Minimal Transmission TUI

# ENABLEWRAP="\033[?7h"
# DISABLEWRAP="\033[?7l"

ITEMS=0
cursor=1

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
    case $(getkey) in
        l) [ "$cursor" -lt "$ITEMS" ] && cursor=$((cursor + 1)) ;;
        k) [ "$cursor" -gt 1 ] && cursor=$((cursor - 1)) ;;
        n) quit ;;
    esac
}

drawtorrents() {
    goto 5 0
    i=0
    ITEMS=$(transmission-remote -l | sed '1d;$d' | wc -l)
    transmission-remote -l | sed '1d;$d' |
        while read -r line; do
            i=$((i + 1))
            # ns "$i $cursor"
            if [ "$i" = "$cursor" ]; then
                mark "$line"
            else
                echo "$line"
            fi
        done

    # transmission-remote -l | sed '1d;$d' > "$TIDEFIFO" &
    # while read -r line; do
    #     i=$((i + 1))
    #     # ns "$i $cursor"
    #     if [ "$i" = "$cursor" ]; then
    #         mark "$line"
    #     else
    #         echo "$line"
    #     fi
    # done < "$TIDEFIFO"

    # done < /tmp/tide
}

mark() {
    printf "\033[7m"
    echo "$@"
    printf "\033[27m"
}

setscreen() {
    printf "\033[?25l\033[2J\033[H"
    LINES=$(stty size | cut -d' ' -f1)
    COLUMNS=$(stty size | cut -d' ' -f2)
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

init() {
    # if ! pidof transmission-daemon > /dev/null; then
    #     transmission-daemon
    #     sleep 1
    # fi
}

main() {
    init
    setscreen
    setborder
    setheader
    setfooter

    trap 'quit' INT EXIT

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
