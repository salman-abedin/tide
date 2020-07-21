#!/bin/sh

marks=/tmp/marks

quit() {
    printf "\033[?7h\033[?25h\033[2J\033[H"
    rm -f $marks $cursor
    kill -- -$$
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

getkey() {
    CURRENT_TTY_SETTINGS=$(stty -g)
    stty -icanon -echo
    head -c1
    stty "$CURRENT_TTY_SETTINGS"
}

handleinput() {
    sendargs() { transmission-remote "$@" > /dev/null; }
    navigate() {
        ITEMS=$(transmission-remote -l | sed '1d;$d' | wc -l)
        CURSOR=$(cat <&4)
        if [ "$1" = -u ]; then
            echo $((CURSOR > 1 ? CURSOR - 1 : ITEMS)) > "$cursor"
        else
            echo $((CURSOR < ITEMS ? CURSOR + 1 : 1)) > "$cursor"
        fi
    }
    case $(getkey) in
        j)
            sendargs -t "$(cat $marks)" -S
            ;;
        ';')
            sendargs -t "$(cat $marks)" -s
            ;;
        d)
            sendargs -t "$(cat $marks)" -r
            setscreen
            drawtorrents
            navigate -u
            # echo $(($(cat $cursor) - 1)) > "$cursor"
            ;;
        k)
            navigate -u
            # ITEMS=$(transmission-remote -l | sed '1d;$d' | wc -l)
            # CURSOR=$(cat <&4)
            # echo $((CURSOR > 1 ? CURSOR - 1 : ITEMS)) > "$cursor"
            ;;
        l)
            navigate -d
            # ITEMS=$(transmission-remote -l | sed '1d;$d' | wc -l)
            # CURSOR=$(cat <&4)
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
            if [ "$i" = "$(cat <&4)" ]; then
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
    goto "$((LINES - 1))" "$((COLUMNS / 2 - 15))"
    echo "You know the drill"

    printf "\033[m"
}

init() {

    # if ! pidof transmission-daemon > /dev/null; then
    #     transmission-daemon
    #     sleep 1
    # fi

    exec 3> mktemp # Cursor
    exec 4< mktemp
    echo 1 >&3
}

main() {
    init
    setscreen

    trap 'quit' INT EXIT
    trap 'setscreen' WINCH

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
