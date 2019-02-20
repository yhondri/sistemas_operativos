#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

//Leer y pasar a texto leíble.
//char buffer[100];
//int itemsScanned = 0;
//if(fgets(buffer, 100, p_finput) != NULL){
//    itemsScanned = sscanf(buffer, "%d", &a);
//}


/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
static int copynFile(FILE * origin, FILE * destination, unsigned int nBytes)
{

    int c, ret, numByteWrittens = 0;

    /* Read file byte by byte */
    while ((c = getc(origin)) != EOF && (numByteWrittens < nBytes)) {

        ret = putc((unsigned char) c, destination); //copia byte en el fichero de destino.
        numByteWrittens += sizeof(ret);

        if (ret == EOF){
            fclose(destination);

            return -1;
        }
    }

    return numByteWrittens;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor
 *
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE.
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc())
 *
 * Returns: !=NULL if success, NULL if error
 */
static char* loadstr(FILE * file)
{
    int c, numCharReads = 0;

    /* Read file byte by byte */
    while ((c = getc(file)) != EOF && (c != '\0')) {
        numCharReads++;
    }

    numCharReads++;

    int offData = sizeof(char)*numCharReads;
    char *readString = malloc(sizeof(char)*numCharReads);
    fseek(file, (0 - numCharReads), SEEK_CUR); //Nos colocamos en la posición que tenemos que leer.
    fread(readString, offData, 1, file); //Leemos.

    return readString;
}

static int loadNumber(FILE * file)
{
    int c, numCharReads = 1;

    /* Read file byte by byte */
    while ((c = getc(file)) != EOF && (c >= '0' && c <= '9')) {
        numCharReads++;
    }

    int *sum = malloc(sizeof(int)*numCharReads);

    fseek(file, (0 - numCharReads), SEEK_CUR); //Nos colocamos en la posición que tenemos que leer.

    if ((c = getc(file)) != EOF && (c >= '0' && c <= '9')) {
        do {
            *sum = ((*sum)*10);
            *sum = ((*sum)+ (c - '0'));
        } while ((c = getc(file)) != EOF && (c >= '0' && c <= '9'));
    }

    return *sum;
}


/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor
 * nFiles: output parameter. Used to return the number
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
static stHeaderEntry* readHeader(FILE * tarFile, unsigned int *nFiles)
{
    stHeaderEntry* headerEntryArray = NULL;
    unsigned int nr_files = 0;
    /* ... Read the number of files (N) from tarfile and store it in nr_files ... */
    nr_files = getc(tarFile)  - '0';

    /* Allocate memory for the array */
    headerEntryArray = (stHeaderEntry *) malloc(sizeof(stHeaderEntry)*nr_files);

    /*... Read the (pathname,size) pairs from tarFile and
     store them in the array ...*/
    for (int i = 0; i < nr_files; i++) {
        headerEntryArray[i].name = loadstr(tarFile);
        headerEntryArray[i].size = loadNumber(tarFile);
        //        fscanf(tarFile, "%d", &t);
        printf("Leído %d \n",  headerEntryArray[i].size);
    }

    /* Store the number of files in the output parameter */
    (*nFiles) = nr_files;

    return headerEntryArray;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE.
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive.
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size)
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int createTar(int nFiles, char *fileNames[], char tarName[])
{
    FILE *tarFile = NULL;

    if ((tarFile = fopen(tarName, "wb")) == NULL) {
        fprintf(stderr,"The input file %s could not be created \n", tarName);
        exit(EXIT_FAILURE);
    }

    stHeaderEntry* headerEntryArray = NULL;
    headerEntryArray = malloc(sizeof(stHeaderEntry)*nFiles);

    int fileNamesLenght = 0;

    for (int i = 0; i < nFiles; i++) {
        fileNamesLenght += strlen(fileNames[i])+1;
    }

    /** Para calcular el byte donde empieza la región de datos.
     * La región de cabecera se compone de 1 entero que contiene el número de documentos, nFiles para el header de cada fichero.
     * La cabecera de cada fichero se compone de un string (nombre fichero) y 1 entero (número bytes del fichero).
     */
    int offData = sizeof(int) + (nFiles * sizeof(unsigned int)) + fileNamesLenght;

    fseek(tarFile, offData, SEEK_SET); //Nos colocamos en el inicio de la región de datos.

    for (int i = 0; i < nFiles; i++) {

        FILE *file = NULL;

        if ((file = fopen(fileNames[i], "r")) == NULL) {
            fprintf(stderr,"The input file %s could not be opened \n", fileNames[i]);
            exit(EXIT_FAILURE);
        }

        int numBytesCopied = copynFile(file, tarFile, UINT_MAX); //Devuelve el número de ficheros copiados.

        headerEntryArray[i].name = fileNames[i];
        headerEntryArray[i].size = numBytesCopied;
    }

    /**Copiar cabeceras.*/
    //    rewind(tarFile);
    fseek(tarFile, 0, SEEK_SET); //Nos colocamos en el inicio del fichero.
    fprintf(tarFile, "%d",nFiles); //Escribimos el número de ficheros.

    for (int i = 0; i < nFiles; i++) {
        fprintf(tarFile, "%s", headerEntryArray[i].name);
        fprintf(tarFile, "%c", '\0');
        fprintf(tarFile, "%d", headerEntryArray[i].size);
    }

    fclose(tarFile); //Cerramos fichero.

    free(headerEntryArray); //Liberamos memoria.

    return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE.
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the
 * tarball's data section. By using information from the
 * header --number of files and (file name, file size) pairs--, extract files
 * stored in the data section of the tarball.
 *
 */
int extractTar(char tarName[]) {
    FILE *extractedFile;
    FILE *tarFile;

    if ((tarFile = fopen(tarName, "r")) == NULL) {
        fprintf(stderr,"The input file %s could not be created \n", tarName);
        exit(EXIT_FAILURE);
    }

    unsigned nFiles;

    stHeaderEntry *headerEntryArray = readHeader(tarFile, &nFiles); //Leemos la cabecera y calculamos el número de ficheros en el tar.

    if (headerEntryArray == NULL) {
        return EXIT_FAILURE;
    } else {
        for (int i = 0; i < nFiles; i++) {

            fprintf(stderr,"Cabecera de i %d \n", headerEntryArray[i].size);

            FILE *newFile = NULL;

            if ((newFile = fopen(headerEntryArray[i].name, "wb")) == NULL) { //Crea el fichero que se va a extraer.
                fclose(tarFile);

                fprintf(stderr,"The input file %s could not be created \n", tarName);
                exit(EXIT_FAILURE);
            }

            int numBytesCopied = copynFile(tarFile, newFile, headerEntryArray[i].size); //Devuelve el número de ficheros copiados.
            if (headerEntryArray[i].size != numBytesCopied) { //Crea el fichero que se va a extraer.
                fclose(newFile);
                fclose(tarFile);

                fprintf(stderr, "Error extracting file %s \n", headerEntryArray[i].name);
                exit(EXIT_FAILURE);
            }
        }

        fclose(tarFile);
    }
    // Complete the function
    return EXIT_FAILURE;
}
