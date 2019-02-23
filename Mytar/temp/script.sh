#!/bin/bash

TAR_FILE_NAME='mytar'

if [[ -e "${TAR_FILE_NAME}" ]] 
then
    echo "File '${TAR_FILE_NAME}' exist"
else
    echo "File '${TAR_FILE_NAME}' is not exist"
    exit 1
fi

if [[ -x "${TAR_FILE_NAME}" ]] 
then
    echo "File '${TAR_FILE_NAME}' is executable"
else
    echo "File '${TAR_FILE_NAME}' is not executable"
    exit 1
fi