#!/bin/sh

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
else
    echo "No TLMPlugin/bin found at ${TLMPLUGINDIR}/bin, try anyway...";
fi

# Start tlmmanager and monitor.
tlmmanager -p 8668 -m 8669 HangingMass.xml&
sleep 1;
tlmmonitor -d -n 1000 localhost:8669 HangingMass.xml
