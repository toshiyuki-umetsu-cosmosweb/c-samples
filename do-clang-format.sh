#!/bin/sh
TargetFiles=""

if [ ${#} -lt 1 ] ; then
    TargetFiles="${TargetFiles} `find . -name "*.c"`"
    TargetFiles="${TargetFiles} `find . -name "*.h"`"
else
    Path=$1
    if [ -d ${Path} ] ; then
	TargetFiles="${TargetFiles} `find ${Path} -name "*.c"`"
	TargetFiles="${TargetFiles} `find ${Path} -name "*.h"`"
    else
	TargetFiles=${Path}
    fi
fi



for File in ${TargetFiles}
do
    echo ${File}
    BackupFile="~{File}"
    mv "${File}" "${BackupFile}"
    if [ $? -eq 0 ] ; then
	clang-format "${BackupFile}" > ${File}
	if [ $? -eq 0 ] ; then
	    rm -f ${BackupFile}
	else
	    mv "${BackupFile}" "${File}"
	fi
    fi
done
