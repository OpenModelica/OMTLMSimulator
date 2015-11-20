#!/bin/sh

DEBUGFLG=

if [ "$1" == "-d" ] ; then
    DEBUGFLG=-d
fi


# Execute locally, just to make sure log-files are located correctly
MYDIR=`dirname $0`
if [ "$MYDIR" != "." ] ; then
    echo "Script needs to be executed in MetaModel directory $MYDIR";
    cd $MYDIR;
fi

# Some attempt to add TLMPlugin to PATH
TLMPLUGINDIR=`pwd | awk -F "TLMPlugin" '{print $1 "TLMPlugin"}'`
if [ -d ${TLMPLUGINDIR}/bin ] ; then
    export PATH=${TLMPLUGINDIR}/bin:$PATH;

    # The following is for OpenModelica start script.
    CYGDRIVE=`echo $TLMPLUGINDIR | grep cygdrive | grep -v grep`;
    if [ -z "$CYGDRIVE" ]; then
        export TLMPluginPath=${TLMPLUGINDIR}/bin;
    else
        export TLMPluginPath=`echo $TLMPLUGINDIR | awk -F "/" '{print $3 ":/" $4 "/" $5 "/" $6 "/" $7}'`/bin;
    fi
    echo TLMPluginPath=$TLMPluginPath;

else
    echo "No TLMPlugin/bin found at ${TLMPLUGINDIR}/bin, try anyway...";
fi

# Start tlmmanager and monitor.
tlmmanager $DEBUGFLG -p 137 -m 138 pendulum.xml&
sleep 1;
tlmmonitor $DEBUGFLG -n 1000 192.168.56.1:138 pendulum.xml
