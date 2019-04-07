#!/bin/bash
RESULTS_FOLDER_NAME='results'

if [ -e "${RESULTS_FOLDER_NAME}" ]
then
	echo "The directory '${RESULTS_FOLDER_NAME}' exist, it will be removed."
	rm -rf ${RESULTS_FOLDER_NAME}
fi

mkdir ${RESULTS_FOLDER_NAME}

askAgain="false"

while [ $askAgain != "true" ];
do
    echo "Introduce el fichero que desea simular: "

    read exampleFile

    #Comprobamos si el fichero mytar existe. 
    if [ -f "${exampleFile}" ]; #El fichero existe y es regular.
    then
        askAgain="true"
        echo "File '${exampleFile}' exist \n"
    elif [ -e "${exampleFile}" ]; then 
        echo "'${exampleFile}' exists but is not regular. \n"
        exit 1
    else
        echo "I cannot tell if '${exampleFile}' exists. \n"
    fi

done


askAgain="false"
maxCPUs=0

while [ $askAgain != "true" ];
do
    echo "Introduce el número máximo de CPUs a utilizar: "

    read maxCPUs

    if [ "$maxCPUs" -le "0" ]; then
        echo "Debes introducir un número de CPUs > 0 \n"
    fi

    if [ "$maxCPUs" -gt "8" ]; then
        echo "Debes introducir un número entre 1 y 8 \n"
    else
        echo "Número máximos de CPUs a utilizar será: ${maxCPUs} \n"
        askAgain="true"
    fi
done

#Declaramos un array con los distintos planificadores.
declare -a schedules=("RR" "SJF" "FCFS" "PRIO")

for index in ${!schedules[@]}
do
    #echo $index "${schedules[index]} \n"
    for cpus in $(seq 1 $maxCPUs)
    do
        #echo -e "CPUs $cpus"
        #echo -e "./schedsim  -i $exampleFile -n $cpus -s ${schedules[index]}"
		./schedsim  -i $exampleFile -n $cpus -s ${schedules[index]}
        for (( logFileIndex = 0 ; logFileIndex < $cpus ; logFileIndex++ ));
        do
            mv "CPU_$logFileIndex.log" results/"${schedules[index]}-CPU-$logFileIndex.log";
            cd gantt-gplot
            ./generate_gantt_chart ../results/"${schedules[index]}-CPU-$logFileIndex.log";
        	cd ..
        done
    done
   
done












