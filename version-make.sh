#!/bin/sh

# make current git version (overwrites version_auto.h)
VERSION_DEFAULT=unknown
VERSION_NAME=VGMSTREAM_VERSION
VERSION_FILE=version_auto.h


# try get version from Git (dynamic)
if ! command -v git > /dev/null ; then
    VERSION=""
else
    VERSION=$(git describe --always 2>&1 | tr : _ )
fi


# ignore git stderr "fatal:*" or blank
if  [[ $VERSION != fatal* ]] && [ ! -z "$VERSION" ] ; then
    LINE="#define $VERSION_NAME \"$VERSION\" /* autogenerated */"
else
    # try to get version from version.h (static)
    echo "Git version not found, can't autogenerate version (using default)"
    LINE="#define $VERSION_NAME \"$VERSION_DEFAULT\" /* autogenerated */"

    while IFS= read -r -u3 item; do
        COMP="#define $VERSION_NAME*"
        if [[ $item == $COMP ]] ; then
            LINE=$item
        fi
    done 3< "version.h"
fi


# avoid overwritting if contents are the same, as some systems rebuild on timestamp
LINE_ORIGINAL="none"
if test -f "version_auto.h"; then
    LINE_ORIGINAL=$(<version_auto.h)
fi
if [[ $LINE != $LINE_ORIGINAL ]] ; then
    echo "$LINE" > "$VERSION_FILE"
fi
