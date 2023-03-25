#!/bin/bash

count="$(cat "$1" | grep glyph-count | tr -d -c 0-9)"

rects="$(cat out.font2 | egrep "rect \d+ \d+ \d+ \d+\)\) ;; ." -o | sed "s/rect //" | sed "s/)) ;;//")"

printf "%s\n%s" "$count" "$rects" > "$2"
