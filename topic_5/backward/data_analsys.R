setwd("~/git/ParallelProgrammingReports/topic_5/backward")
data_auto<-read.csv("auto_series.txt", header=FALSE, sep=" ")
data_static_one<-read.csv("static_series_1x.txt", header=FALSE, sep=" ")
data_static<-read.csv("static_series_2x.txt", header=FALSE, sep=" ")
data_guided_one<-read.csv("guided_series_1x.txt", header=FALSE, sep=" ")
data_guided<-read.csv("guided_series_2x.txt", header=FALSE, sep=" ")
data_dynamic_one<-read.csv("dynamic_series_1x.txt", header=FALSE, sep=" ")
data_dynamic<-read.csv("dynamic_series_2x.txt", header=FALSE, sep=" ")

data_auto2<-cbind(rep(0,10), data_auto[,2])

data_static<-rbind(data_static_one, data_static)
data_guided<-rbind(data_guided_one, data_guided)
data_dynamic<-rbind(data_dynamic_one, data_dynamic)
colnames(data_static)<-c("chunk_size", "runtimes")
colnames(data_auto)<-c("chunk_size", "runtimes")
colnames(data_dynamic)<-c("chunk_size", "runtimes")
colnames(data_guided)<-c("chunk_size", "runtimes")
colnames(data_auto2)<-c("chunk_size", "runtimes")

boxplot(runtimes~chunk_size, data_static, ylab="time in seconds", xlab="n chunk size", main="static")
boxplot(runtimes~chunk_size, data_dynamic, ylab="time in seconds", xlab="n chunk size", main="dynamic")
boxplot(runtimes~chunk_size, data_guided, ylab="time in seconds", xlab="n chunk size", main="guided")
boxplot(runtimes~chunk_size, data_auto, ylab="time in seconds", xlab="n chunk size")

data_static<-rbind(data_static, data_auto2)
data_dynamic<-rbind(data_dynamic, data_auto2)
data_guided<-rbind(data_guided, data_auto2)

boxplot(runtimes~chunk_size, data_static, ylab="time in seconds", xlab="n chunk size")
boxplot(runtimes~chunk_size, data_dynamic, ylab="time in seconds", xlab="n chunk size")
boxplot(runtimes~chunk_size, data_guided, ylab="time in seconds", xlab="n chunk size")

best_guided<-data_guided[which(data_guided[,1]==32),]
best_dynamic<-data_dynamic[which(data_dynamic[,1]==128),]
best_static<-data_static[which(data_static[,1]==32),]

best_guided<-data.frame("scheduling"= rep("guided",10), "runtimes"=best_guided[,2])
best_dynamic<-data.frame("scheduling" = rep("dynamic",10), "runtimes" = best_dynamic[,2])
best_static<-data.frame("scheduling" = rep("static",10), "runtimes" = best_static[,2])
best_auto<-data.frame("scheduling" = rep("auto", 10), "runtimes" = data_auto2[,2])
total<-rbind(best_guided, best_dynamic)
total<-rbind(total, best_static)
total<-rbind(total, best_auto)

boxplot(runtimes~scheduling, total, ylab="time in seconds", xlab="type scheduling", main="total")

par(mfcol=c(2,2))
