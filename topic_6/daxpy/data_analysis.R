setwd("~/git/ParallelProgrammingReports/topic_6/daxpy")
data<-read.csv("slurm-6330834.txt", header=FALSE, sep=" ")
data_assembled<-data.frame("cores"=data[1:40,1], "block"=data[which(data[,2]>1),3], "cycle"=data[which(data[,2]==1),3])
boxplot(cycle~cores, data=data_assembled, ylim=c(0.002, 0.03), col='red', xlab="cores", ylab="runtime")
boxplot(block~cores, data=data_assembled, add=TRUE, col='blue')
legend(0.5,0.03, c("cyclic","block"), lty=c(1,1), lwd=c(5,5),col=c("red", "blue"))
