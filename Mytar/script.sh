#!/bin/bash
TAR_FILE_NAME='mytar'
TMP_DIR_NAME='tmp'
OUT_DIR_NAME='out'
FILETAR_MTAR_DIR_NAME="filetar.mtar"
HELLO_WORLD='Hello world!'

#FileNames
FILE1_TXT='file1.txt'
FILE2_TXT='file2.txt'
FILE3_TXT='file3.dat'

#Comprobamos si el fichero mytar existe. 
if [ -e "${TAR_FILE_NAME}" ] 
then
    echo "File '${TAR_FILE_NAME}' exist"
else
    echo "File '${TAR_FILE_NAME}' is not exist"
    exit 1
fi

#Comprobamos si el fichero mytar es ejecutable. 
if [ -x "${TAR_FILE_NAME}" ] 
then
    echo "File '${TAR_FILE_NAME}' is executable"
else
    echo "File '${TAR_FILE_NAME}' is not executable"
    exit 1
fi

#Comprobamos si el directorio tmp existe y lo borramos en caso de que exista. 
if [ -d "${TMP_DIR_NAME}" ]
then
    echo "The directory '${TMP_DIR_NAME}' exist, it will be removed."
    rm -rf ${TMP_DIR_NAME}
fi

mkdir "${TMP_DIR_NAME}"
cd "${TMP_DIR_NAME}"

#Creamos 3 ficheros:
#file1.txt con el contenido de HelloWorld!
#file2.txt con las primeras 10 líneas de etc/passwd
#file3.dat con contenido aleatorio.
echo "${HELLO_WORLD}" > "${FILE1_TXT}"
head -n 10 /etc/passwd > "${FILE2_TXT}"
head -c 1024 /dev/urandom > "${FILE3_TXT}"

../mytar -c -f "${FILETAR_MTAR_DIR_NAME}" "${FILE1_TXT}" "${FILE2_TXT}" "${FILE3_TXT}"
echo file1.txt
echo "${FILETAR_MTAR_DIR_NAME} has been created succefully.!"

cd .. #Volvemos al directorio donde se está ejecuntado el script.

if [ -d "${OUT_DIR_NAME}" ]
then
    echo "The directory '${OUT_DIR_NAME}' exist, it will be removed."
    rm -rf ${OUT_DIR_NAME}
fi

mkdir "${OUT_DIR_NAME}"

cp "tmp/${FILETAR_MTAR_DIR_NAME}" ${OUT_DIR_NAME}

cd ${OUT_DIR_NAME}

../mytar -x -f ${FILETAR_MTAR_DIR_NAME}
echo "\nContent of ${FILETAR_MTAR_DIR_NAME} has been created succefully.!\n"

fileNames=(${FILE1_TXT} ${FILE2_TXT} ${FILE3_TXT})  ##declare array

for i in ${fileNames[*]}
do
    #Si los archivos a comparar son diferentes, mostramos un mensaje
    if diff "$i" ../tmp/$i &> /dev/null ; then
        echo "Files '$i and ../tmp/$i' are equals"
    else
        echo "Files '$i and ../tmp/$i' differ"
        exit 1
    fi
done

echo "\nCorrect\n"

exit 0