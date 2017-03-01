setwd("~/git/ParallelProgrammingReports/topic_4/threadtimer")
data<-read.csv("test.txt", header=FALSE, sep=" ")
colnames(data)<-c("threads", "runtimes")
boxplot(runtimes~threads, data[-c(1,155),], ylab="time in seconds", xlab="n threads")
