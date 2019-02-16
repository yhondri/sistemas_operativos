#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"


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
    while ((c = getc(origin)) != EOF) {

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
    // Complete the function
    return NULL;
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
    /* Allocate memory for the array */
    headerEntryArray = malloc(sizeof(stHeaderEntry)*nr_files);
    /*... Read the (pathname,size) pairs from tarFile and
     store them in the array ...*/
    /* Store the number of files in the output parameter */
    (*nFiles) = nr_files;

    return NULL;
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

//    if ((tarFile = fopen(tarName, "r")) == NULL) {
//        fprintf(stderr,"The input file %s could not be opened \n", tarName);
//        exit(EXIT_FAILURE);
//    }

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
    int offData = sizeof(int) + (nFiles *sizeof(unsigned int)) + fileNamesLenght;

    fseek(tarFile, offData, SEEK_SET); //Nos colocamos en el inicio de la región de datos.

    for (int i = 0; i < nFiles; i++) {

        FILE *file = NULL;

        if ((file = fopen(fileNames[i], "r")) == NULL) {
            fprintf(stderr,"The input file %s could not be opened \n", fileNames[i]);
            exit(EXIT_FAILURE);
        }

        int numBytesCopied = copynFile(file, tarFile, UINT_MAX); //pasamos el TarFile justo en el punto donde queremos que empiece a escribir el siguiente fichero.
        printf("NumBytes %d", numBytesCopied);
    }

    fclose(tarFile);

    // Complete the function
    return EXIT_FAILURE;
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
int extractTar(char tarName[])
{
    // Complete the function
    return EXIT_FAILURE;
}
