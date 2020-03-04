#!/bin/bash

if [ $# -eq 0 ]; then
    echo "$0 <path-to-powmon-dat>"
    exit 1
fi
POWMON_DAT_FILE=$1

NAME=$(ls ${POWMON_DAT_FILE} | cut -d "." -f 1)
END=$(ls ${POWMON_DAT_FILE} | cut -d "." -f 2-3)

NEW_F1=${NAME}-socket0.${END}
NEW_F2=${NAME}-socket1.${END}

grep _IBMPOWER0 ${POWMON_DAT_FILE} > ${NEW_F1}
grep _IBMPOWER1 ${POWMON_DAT_FILE} > ${NEW_F2}

Rscript --vanilla powmon-ibm-plot.R ${NEW_F1} ${NEW_F2}

#
