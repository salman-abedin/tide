#!/bin/sh
#
# Minimal Transmission TUI
# Dependencies: stty, head, read, printf, echo, cut, seq, cat

cursor=/tmp/tide_cursor
mark=/tmp/mark

quit() {
    printf "\033[?7h\033[?25h\033[2J\033[H"
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
        j)
            sendargs -t "$(cat $mark)" -S
            ;;
        ';')
            sendargs -t "$(cat $mark)" -s
            ;;
        d)
            sendargs -t "$(cat $mark)" -r
            setscreen
            setheader
            setfooter
            drawtorrents
            echo $(($(cat $cursor) - 1)) > "$cursor"
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
    case $1 in
        -r) printf "\033[31m" ;;
        -g) printf "\033[32m" ;;
        -y) printf "\033[33m" ;;
        -b) printf "\033[34m" ;;
        -w) printf "\033[39m" ;;
        -i) printf "\033[7m" ;;
        -d) printf "\033[2m" ;;
    esac
    printf "%s\033[0m\n" "$2"
}

mark() {
    printf "\033[7m%s\033[27m\n" "$1"
    echo "${1%% *}" > "$mark"
}

goto() { printf "\033[%s;%sH" "$1" "$2"; }

setfooter() {
    printf "\033[2m"
    goto "$((LINES - 2))" 0
    for i in $(seq "$COLUMNS"); do printf "%s" "-"; done
    goto "$((LINES - 1))" "$((COLUMNS / 2 - 20))"
    echo "h:Pause   j:Down   k:Up   l:Start   d:Delete   q:Quit"
    printf "\033[0m"
}

setheader() {
    printf "\033[2m"
    goto 2 "$((COLUMNS / 2 - 10))"
    echo "tide: Transmission Client"
    goto 3 0
    for i in $(seq "$COLUMNS"); do printf "%s" "-"; done
    printf "\033[0m"
}

setscreen() {
    printf "\033[?7l\033[?25l\033[2J\033[H"
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
