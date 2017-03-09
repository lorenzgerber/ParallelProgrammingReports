setwd("~/git/ParallelProgrammingReports/topic_6/tsp")
raw_data<-read.csv("slurm-6340294.txt", header=FALSE, sep=" ")
data<-data.frame("method"=c(rep(1,20), rep(2, 20), rep(3, 20), rep(4,20)), "cores"=raw_data[,1], "runtime"=raw_data[,2])
