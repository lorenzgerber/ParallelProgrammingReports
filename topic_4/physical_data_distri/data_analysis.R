setwd("~/git/ParallelProgrammingReports/topic_4/physical_data_distri")
data_non<-read.csv("non_distr.txt", header=FALSE)
data_distr<-read.csv("distr.txt", header=FALSE)
data<-data.frame(data_non, data_distr)
colnames(data)<-c("non_distributed","distributed")
boxplot(data, ylab="seconds")