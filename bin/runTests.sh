#!/bin/bash

export PATH=$OMTLMSimulatorPath/bin:$PATH


if [ $# -eq 0 ]
then
	declare -a models=("FmuHopsanHydraulic" "FmiTestLinux" "HopsanHopsanSignal" "HopsanHopsanOneWaySignal" "OmcOmcSignals" "OMFMISimulatorTest")
else
	declare -a models=("$1")
fi

monitorport=12111
let "success=1"

for i in "${models[@]}"
do
	let "success = 1"
  echo "Checking $i..."

	while lsof -i :$monitorport > /dev/null 2>&1
	do
		let "monitorport += 1"		
	done
	#echo "Monitor port: $monitorport"
	

	cd $OMTLMSimulatorPath/CompositeModels/$i/

	# Check if fetch interfaces works
	rm interfaceData.xml > /dev/null 2>&1
	$OMTLMSimulatorPath/bin/tlmmanager -r $OMTLMSimulatorPath/CompositeModels/$i/$i.xml > /dev/null 2>&1

	if ! [ -s interfaceData.xml ]
	then
		tput setaf 1
		let "success = 0"
		echo Fetch interface data failed!
		tput sgr0
	fi

	$OMTLMSimulatorPath/bin/tlmmanager -p 11111 -m $monitorport $OMTLMSimulatorPath/CompositeModels/$i/$i.xml > /dev/null 2>&1 &
	$OMTLMSimulatorPath/bin/tlmmonitor -n 1000 127.0.1.1:$monitorport $OMTLMSimulatorPath/CompositeModels/$i/$i.xml

	if [ $? -gt 0 ]
	then
		tput setaf 1
		echo Simulation failed!
		let "success = 0"
		tput sgr0
		exit
	fi

	$OPENMODELICAHOME/bin/omc-diff 0.001 $i.csv ${i}_ref.csv > /dev/null 2>&1
	
        if [ $? -gt 0 ]
	then
		tput setaf 1
		echo Simulation results differ!
		let "success = 0"
		tput sgr0
	fi

	if [ $success -gt 0 ]
	then
		tput setaf 2
		echo Success!
		tput sgr0
	fi

done














