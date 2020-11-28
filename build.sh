#!/usr/bin/env bash
set -euo pipefail

if [ $# -eq 0 ]; then
    mkdir -p build
    cd build
    cmake ..
    make
else
    for i in "$@"
    do
    case $i in
        -f=*|--filename=*)
        FILE="${i#*=}"
        shift
        ;;
        -s=*|--size=*)
        SIZE="${i#*=}"
        shift
        ;;
#        -v=*|--verbose=*)
#        VERBOSE="${i#*=}"
#        shift
#        ;;
        *)
        ;;
    esac
    done

    echo "FILENAME   = ${FILE}"
    echo "IMAGE SIZE = ${SIZE}"
#    echo "VERBOSE    = ${VERBOSE}"
    echo " "

    ./build/src/create_raster -f ${FILE} -s ${SIZE}
fi

