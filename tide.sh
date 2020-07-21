#!/bin/sh
#
# Minimal Transmission Client
# Dependencies: stty, head, read, printf, echo, cut, seq, cat

cursor=/tmp/tide_cursor
marks=/tmp/tide_marks

quit() {
    printf "\033[?7h\033[?25h\033[2J\033[H"
    rm -f "$cursor" "$marks"
    kill -- -$$
}

getkey() {
    CURRENT_TTY_SETTINGS=$(stty -g)
    stty -icanon -echo
    head -c1
    stty "$CURRENT_TTY_SETTINGS"
}

handleinput() {
    navigate() {
        ITEMS=$(transmission-remote -l | sed '1d;$d' | wc -l)
        CURSOR=$(cat < "$cursor")
        if [ "$1" = -u ]; then
            echo $((CURSOR > 1 ? CURSOR - 1 : ITEMS)) > "$cursor"
        else
            echo $((CURSOR < ITEMS ? CURSOR + 1 : 1)) > "$cursor"
        fi
    }
    sendargs() { transmission-remote "$@" > /dev/null; }
    case $(getkey) in
        h) sendargs -t "$(cat $marks)" -S ;;
        l) sendargs -t "$(cat $marks)" -s ;;
        d)
            sendargs -t "$(cat $marks)" -rad
            setscreen
            drawtorrents
            navigate -u
            ;;
        k) navigate -u ;;
        j) navigate -d ;;
        q) quit ;;
    esac
}

drawtorrents() {
    goto 5 0
    i=0
    transmission-remote -l 2> /dev/null | sed '1d;$d' |
        while read -r line; do
            i=$((i + 1))
            if [ "$i" = "$(cat $cursor)" ]; then
                mark "$line"
            else
                case $line in
                    *\ \ 100%*) paint -g "$line" ;;
                    *\ \ Stopped*) paint -r "$line" ;;
                    *\ \ Idle*) paint -y "$line" ;;
                    *) paint -w "$line" ;;
                esac
            fi
        done
}

paint() {
    printf "\033["
    case $1 in
        -r) printf "31m" ;;
        -g) printf "32m" ;;
        -y) printf "33m" ;;
        -w) printf "39m" ;;
    esac
    printf "%s\033[m\n" "$2"
}

mark() {
    printf "\033[7m%s\033[27m\n" "$1"
    echo "${1%% *}" > "$marks"
}

goto() { printf "\033[%s;%sH" "$1" "$2"; }

setscreen() {
    printf "\033[?7l\033[?25l\033[2J\033[H"
    LINES=$(stty size | cut -d' ' -f1)
    COLUMNS=$(stty size | cut -d' ' -f2)

    printf "\033[2m"

    goto 2 "$((COLUMNS / 2 - 10))"
    echo "tide: Tiny Transmission Client"
    goto 3 0
    for i in $(seq "$COLUMNS"); do printf "%s" "-"; done
    goto "$((LINES - 2))" 0
    for i in $(seq "$COLUMNS"); do printf "%s" "-"; done
    goto "$((LINES - 1))" "$((COLUMNS / 2 - 20))"
    echo "h:Stop   j:Down   k:Up   l:Start   d:Delete   q:Quit"

    printf "\033[m"

}

init() {
    echo 1 > "$cursor"
    if ! pidof transmission-daemon > /dev/null; then
        transmission-daemon
        setscreen
        goto "$((LINES / 2))" "$((COLUMNS / 2 - 15))"
        echo Loading the daemon. Chill.
        sleep 3
    fi
}

main() {
    init
    setscreen

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
