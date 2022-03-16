#!/bin/bash

THIS_DIR=`cd "$(dirname $0)"/.. && pwd`

HAS_BAD_STYLE=""

function checkBadStyle() {
    for i in $@;
    do
        cat $i | grep --color -n -A 2 -B 2 "using namespace";

        if [[ "$?" == "0" ]]; then
           echo "### Bad style file: $i"
           echo "### You should never using namespace in a header file!"
           HAS_BAD_STYLE="1";
        fi
    done
}

##########
FM_CHECKING_FILES=(`find $THIS_DIR/facemagic -type f | egrep -e "(h|hpp)$"`)
checkBadStyle ${FM_CHECKING_FILES[@]}

########## 
CGE_DIR="$THIS_DIR/dependencies/cge/source";
CGE_CHECKING_FILES=(`find $CGE_DIR -type f | grep -v "cgePlatform_QT" | egrep -e "(h|hpp)$"`)
checkBadStyle ${CGE_CHECKING_FILES[@]}

if [[ "$HAS_BAD_STYLE" == "1" ]]; then
    echo "Bad style detected!";
    exit 1;
else
    echo "No bad style, good!"
fi

# echo ${CHECKING_FILES[@]} | tr ' ' '\n'
# echo ${CGE_CHECKING_FILES[@]} | tr ' ' '\n'