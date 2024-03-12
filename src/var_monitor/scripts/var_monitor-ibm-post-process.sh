#!/bin/bash

if [ $# -eq 0 ]; then
    echo "$0 <path-to-var_monitor-dat>"
    exit 1
fi
VAR_MONITOR_DAT_FILE=$1

NAME=$(ls ${VAR_MONITOR_DAT_FILE} | cut -d "." -f 1)
END=$(ls ${VAR_MONITOR_DAT_FILE} | cut -d "." -f 2-3)

NEW_F1=${NAME}-socket0.${END}
NEW_F2=${NAME}-socket1.${END}

grep _IBMPOWER0 ${VAR_MONITOR_DAT_FILE} > ${NEW_F1}
grep _IBMPOWER1 ${VAR_MONITOR_DAT_FILE} > ${NEW_F2}

Rscript --vanilla var_monitor-ibm-plot.R ${NEW_F1} ${NEW_F2}

#
