
# This script plots power data colloect by 'powmon'. It reads the CSV files of a single run, where each 
# file holds power data of a node run the tasks. It plots power data per node, where there is a figure for each node. 
#  It also plot the discriptive stats of the power data of all nodes.
# Requirements:
#   pandas>=1.5.0
#   numpy>=1.23.3
#   matplotlib>=3.6.0
#   argparse>=1.4.0
# 
# How to run the script?
#   python3 plotting.py [options]
#  
# Options:
#   --input or -i: required to specify power data path. Make sure the power data in the
#     the directory is from a single run.
#   --output or -o: optional to specify where to save the plots. If not defined, the input path
#     is used.
#   --type or -t: optional to select what to plot. 
#     per-node: plot power data per node only. If the app were run on 4 nodes, 4 plots
#     are created. 
#     aggregate: plot discriptive stats of the power data of all nodes. There should be four plots, mean, max, min, median. 
# 
# Examples:
#   1. python3 plotting.py --input "/path/to/PowerData" --type per-node
#     To plot power data located in "/path/to/PowerData" per node and save plots in 
#     "/path/to/PowerData".
#   2. python3 plotting.py -i /path/to/PowerData/ -o /path/where/to/SavePlots -t aggregate
#     To plot descriptive stats of power data located in /path/to/PowerData/, and save the plots in
#     "/path/where/to/SavePlots".
                                 

import pandas as pd
import matplotlib.pyplot as plt
from os import listdir
import argparse
import os


#---------------------------------------------------------------------
# Read csv files
#---------------------------------------------------------------------
def readCsvFile(csvFile):
    powData = pd.read_csv(csvFile,delimiter= ',')
    return powData

#---------------------------------------------------------------------
# plot descriptive stats of power data 
#---------------------------------------------------------------------
def plotAggregatedData(aggData, outputPath,dState):
    col = ['host', 'Node Power (W)', 'Socket 0 CPU Power (W)', \
        'Socket 0 GPU Power (W)', 'Socket 0 Mem Power (W)', \
        'Socket 1 CPU Power (W)', 'Socket 1 GPU Power (W)', \
        'Socket 1 Mem Power (W)']
    # convert list of list to a df
    df = pd.DataFrame(aggData, columns=col)
    plt.plot(df['host'], df['Node Power (W)'],label = "Node Power",color='green')
    plt.plot(df['host'], df['Socket 0 CPU Power (W)'],label = "Socket 0 CPU Power",color='blue')
    plt.plot(df['host'], df['Socket 0 GPU Power (W)'],label = "Socket 0 GPU Power",color='black')
    plt.plot(df['host'], df['Socket 0 Mem Power (W)'],label = "Socket 0 Mem Power",color='red')
    plt.plot(df['host'], df['Socket 1 CPU Power (W)'],label = "Socket 1 CPU Power",color='m')
    plt.plot(df['host'], df['Socket 1 GPU Power (W)'],label = "Socket 1 GPU Power",color='brown')
    plt.plot(df['host'], df['Socket 1 Mem Power (W)'],label = "Socket 1 Mem Power",color='grey')
    plt.xlabel("Host")
    plt.ylabel("Power(Watts)")
    plt.legend(loc='best',prop={'size': 8})
    plt.title("{0} Power data of AMG run on. 16 nodes and 20 cores per node.".format(dState))
    plt.savefig('{0}/aggregated.png'.format(outputPath))
    plt.show()



#---------------------------------------------------------------------
# plot power data per node
#---------------------------------------------------------------------
def plotPowData(df,host, outputPath, desc):
    plt.plot(df['Timestamp (ms)'], df['Node Power (W)'],label = "Node Power",color='green')
    plt.plot(df['Timestamp (ms)'], df['Socket 0 CPU Power (W)'],label = "Socket 0 CPU Power",color='blue')
    plt.plot(df['Timestamp (ms)'], df['Socket 0 GPU Power (W)'],label = "Socket 0 GPU Power",color='black')
    plt.plot(df['Timestamp (ms)'], df['Socket 0 Mem Power (W)'],label = "Socket 0 Mem Power",color='red')
    plt.plot(df['Timestamp (ms)'], df['Socket 1 CPU Power (W)'],label = "Socket 1 CPU Power",color='m')
    plt.plot(df['Timestamp (ms)'], df['Socket 1 GPU Power (W)'],label = "Socket 1 GPU Power",color='brown')
    plt.plot(df['Timestamp (ms)'], df['Socket 1 Mem Power (W)'],label = "Socket 1 Mem Power",color='grey')
    plt.xlabel("Time(ms)")
    plt.ylabel("Power(Watts)")
    plt.legend(loc='best',prop={'size': 8})
    plt.title("{0} ({1})".format(desc, host))
    plt.savefig('{0}/{1}.png'.format(outputPath,host))
    plt.show()

#---------------------------------------------------------------------
# find files by extension
#---------------------------------------------------------------------
def list_files(dir_path, fileExt):
    if not(os.path.exists(dir_path)):
        print("Input dir {} not exist".format(dir_path))
        exit(1)
    files = listdir(dir_path)
    return [csv for csv in files if csv.endswith(fileExt)]

#---------------------------------------------------------------------
# convert csv to a dataframe
#---------------------------------------------------------------------
def extractDataFromCSV(dataFile):
    file2 = open(dataFile,"r")
    df = pd.read_csv(file2)
    return df

#---------------------------------------------------------------------
# find aggregate stats of power data of a node
#---------------------------------------------------------------------
def findStats(df, hostName):
    df = df.drop(['Timestamp (ms)'], axis=1)
    mean = [hostName]
    max = [hostName]
    min = [hostName]
    median = [hostName]
    for header in df.columns:
        mean.append(df[header].mean())
        max.append(df[header].max())
        min.append(df[header].min())
        median.append(df[header].median())
    return(mean, min, max, median)



#---------------------------------------------------------------------
# main
#---------------------------------------------------------------------
if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog="plotting", description = 'Plotting Power Data')
    parser.add_argument('--input', '-i', type=str, required=True, help = "Power data path. Each path should have the power data of a single run.")
    parser.add_argument('--type', '-t', type=str, required=False, choices = ['per-node', 'aggregate'], help="Select what plots to generate. Per node \
        or aggregated of all nodes. per-node is default")
    parser.add_argument('--output', '-o', type=str, required=False, help = "Output directory path. If not defined, plots will be saved in the input path.")
    parser.add_argument('--description', '-d', type=str, required=True, help = "Output directory path. If not defined, plots will be saved in the input path.")


    # create a new ArgumentParser object
    args = parser.parse_args()
    inputPath = args.input
    desc = args.description
    pltType = args.type
    if not inputPath.endswith('/'):
        inputPath += '/'
    if not os.path.exists(inputPath):
        print("No such directory, use a valid input path.")
    if args.output == None:
        outputPath = inputPath
    else:
        outputPath = args.output
    if not outputPath.endswith('/'):
        outputPath += '/'
    if not os.path.exists(outputPath):
        os.makedirs(outputPath)
    # find csv files 
    csvFiles = list_files(inputPath, ".dat")
    if pltType == 'per-node' or pltType == None:
        for csv in csvFiles:
            host = csv.split('.')[0]
            hostFile = "{0}/{1}".format(inputPath,csv)
            hostdf = readCsvFile(hostFile)
            plotPowData(hostdf, host, outputPath, desc)

    if pltType == 'aggregate':
        allMeans = []
        allMaxs = []
        allMins = []
        allMedians = []

        for csv in csvFiles:
            host = csv.split('.')[0]
            hostFile = "{0}/{1}".format(inputPath,csv)
            hostdf = readCsvFile(hostFile)
            hostMean, hostMin, hostMax, hostMedian = findStats(hostdf,host)
            allMeans.append(hostMean)
            allMaxs.append(hostMax)
            allMins.append(hostMin)
            allMedians.append(hostMedian)
        plotAggregatedData(allMeans, outputPath, 'Mean')
        plotAggregatedData(allMedians, outputPath, 'Median')
        plotAggregatedData(allMaxs, outputPath, 'Max')
        plotAggregatedData(allMins, outputPath, 'Min')



