#!/usr/bin/env bash
DIR=$1

if [ ! -d "$DIR" ]; then
    echo "Usage: ./sync <folder-to-sync>"
    echo "Directory \"$DIR\" does not exists"
    exit
fi

DIR=$(pwd)/$DIR

echo "Syncing folder - $DIR"

sync_folder() {
    rsync -avz --delete $DIR pi-dev:go/src
}

# initial sync
sync_folder

# keep pooling for data
while inotifywait -r -e modify,create,delete $DIR; do
    sync_folder
    # rsync -avz --delete $DIR pi@192.168.1.20:go/src
done
