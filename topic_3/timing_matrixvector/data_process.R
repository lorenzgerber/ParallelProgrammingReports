#Importing the data for trapezoidal execrise
setwd("~/git/ParallelProgrammingReports/topic_3/timing_trapezoidal")
data1<-read.csv("serialtime.txt", sep=" ", header=FALSE)
data2<-read.csv("slurm-6201736.txt", sep=" ", header=FALSE)
data4<-read.csv("slurm-6201737.txt", sep=" ", header=FALSE)
data8<-read.csv("slurm-6201739.txt", sep=" ", header=FALSE)
data16<-read.csv("slurm-6201740.txt", sep=" ", header=FALSE)
data1<-matrix(unlist(cbind(rep(1,500), data1)),500,3)
data2<-matrix(unlist(cbind(rep(2,500), data2)),500,3)
data4<-matrix(unlist(cbind(rep(4,500), data4)),500,3)
data8<-matrix(unlist(cbind(rep(8,500), data8)),500,3)
data16<-matrix(unlist(cbind(rep(16,500), data16)),500,3)
data<-rbind(data1, data2)
data<-rbind(data, data4)
data<-rbind(data, data8)
data<-rbind(data, data16)

# create matrix with mean values
mean_mat<-NULL
for(i in c(1, 2, 4, 8, 16)){
  for(j in c(65536, 131072, 262144, 524288, 1048576)){
    mean_mat<-c(mean_mat,round(mean(data[which(data[,1] == i & data[,2] == j), 3])*1000,2))
  }
}
mean_mat<-matrix(mean_mat, 5, 5, byrow=TRUE)

# create matrix with median values
median_mat<-NULL
for(i in c(1, 2, 4, 8, 16)){
  for(j in c(65536, 131072, 262144, 524288, 1048576)){
    median_mat<-c(median_mat,round(median(data[which(data[,1] == i & data[,2] == j), 3])*1000, 2))
  }
}
median_mat<-matrix(median_mat, 5, 5, byrow=TRUE)

# create matrix with min values
min_mat<-NULL
for(i in c(1, 2, 4, 8, 16)){
  for(j in c(65536, 131072, 262144, 524288, 1048576)){
    min_mat<-c(min_mat,round(min(data[which(data[,1] == i & data[,2] == j), 3])*1000, 2))
  }
}
min_mat<-matrix(min_mat, 5, 5, byrow=TRUE)



#Importing the data for matrix vector execrise
setwd("~/git/ParallelProgrammingReports/topic_3/timing_matrixvector")
data1<-read.csv("slurm-6146121.txt", sep=" ", header=FALSE)
data2<-read.csv("slurm-6146122.txt", sep=" ", header=FALSE)
data4<-read.csv("slurm-6146123.txt", sep=" ", header=FALSE)
data8<-read.csv("slurm-6146124.txt", sep=" ", header=FALSE)
data16<-read.csv("slurm-6146126.txt", sep=" ", header=FALSE)
data1<-matrix(unlist(cbind(rep(1,500), data1)),500,3)
data2<-matrix(unlist(cbind(rep(2,500), data2)),500,3)
data4<-matrix(unlist(cbind(rep(4,500), data4)),500,3)
data8<-matrix(unlist(cbind(rep(8,500), data8)),500,3)
data16<-matrix(unlist(cbind(rep(16,500), data16)),500,3)
data<-rbind(data1, data2)
data<-rbind(data, data4)
data<-rbind(data, data8)
data<-rbind(data, data16)

# create matrix with mean values
mean_mat<-NULL
for(i in c(1, 2, 4, 8, 16)){
  for(j in c(1024, 2048, 4096, 8192, 16384)){
    mean_mat<-c(mean_mat,round(mean(data[which(data[,1] == i & data[,2] == j), 3])*1000,2))
  }
}
mean_mat<-matrix(mean_mat, 5, 5, byrow=TRUE)

# create matrix with median values
median_mat<-NULL
for(i in c(1, 2, 4, 8, 16)){
  for(j in c(1024, 2048, 4096, 8192, 16384)){
    median_mat<-c(median_mat,round(median(data[which(data[,1] == i & data[,2] == j), 3])*1000,2))
  }
}
median_mat<-matrix(median_mat, 5, 5, byrow=TRUE)

hist(data[which(data[,1] == 2 & data[2] == 4096),3],nclass=100)

par(mfcol=c(1,2))
#Boxplot runtimes in relation to number of processes
boxplot(data[,3]~data[,1], ylim=c(0,0.0002), xlab='p Processes', ylab='runtime in milliseconds')

#Boxplot runtimes in relation to problem size
boxplot(data[,3]~data[,2], ylim=c(0,0.0002), xlab='n Problem Size', ylab='runtime in milliseconds')


range_value<-range(data[which(data[,1] == 2 & data[,2] == 4096),3])
median(data[which(data[,1] == 2 & data[,2] == 4096),3])*100/range_value[2]
mean(data[which(data[,1] == 2 & data[,2] == 4096),3])*100/range_value[2]
data[which(data[,1] == 2 & data[,2] == 4096),3]*100/range_value[2]
normalized<-data[which(data[,1] == 2 & data[,2] == 4096),3]*100/range_value[2]
plot(rep(1,length(normalized)), normalized,  xlim=c(0,10), ylim=c(0,100))

#spread of time measurements
xpos<-1
xvalues<-c(2,3,4,5,6,10,11,12,13,14,18, 19, 20, 21,22, 26,27,28,29,30, 34, 35,36,37,38)
plot(NULL, xlim=c(0,39), ylim=c(0,100), xaxt="n", xlab="cores p = 1, 2, 4, 8, 16", ylab="normalized runtime spread")
axis(1,at=xvalues,labels=c(rep(c("1024","2048","4096","8192", "16384"),5)), las=2)
for(i in c(1, 2, 4, 8, 16)){
  for(j in c(1024, 2048, 4096, 8192, 16384)){
    range_value<-range(data[which(data[,1] == i & data[,2] == j),3])
    normalized<-data[which(data[,1] == i & data[,2] == j),3]*100/range_value[2]
    points(rep(xvalues[xpos],length(normalized)), normalized,  xlim=c(0,10), ylim=c(0,100), col="grey")
    points(xvalues[xpos], mean(data[which(data[,1] == i & data[,2] == j),3])*100/range_value[2], pch=0, col="red")
    points(xvalues[xpos], median(data[which(data[,1] == i & data[,2] == j), 3])*100/range_value[2], pch=2, col = "blue")
    xpos<-xpos+1
  }
}

