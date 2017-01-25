setwd("~/git/ParallelProgrammingReports/topic_2")
test<-read.table(file='data.txt', sep='\t', dec='.')
test<-test[,1:8]
par(mfcol=c(2,1))
plot(c(10,20,40,80,160,320), test[,1], type='l', ylim=c(0,130), xlab="size of n", ylab="Speedup")
points(c(10,20,40,80,160,320), test[,2], type='l', col='red')
points(c(10,20,40,80,160,320), test[,3], type='l',col='blue')
points(c(10,20,40,80,160,320), test[,4], type='l', col='green')
points(c(10,20,40,80,160,320), test[,5], type='l', col='purple')
points(c(10,20,40,80,160,320), test[,6], type='l', col='pink')
points(c(10,20,40,80,160,320), test[,7], type='l', col='yellow')
points(c(10,20,40,80,160,320), test[,8], type='l', col='brown')
legend(250, 100, 
       c('p = 1', 'p = 2', 'p = 4','p = 8','p = 16','p = 32','p = 64','p = 128' ), 
       lty=c(1,1,1,1,1,1,1,1),
       lwd=c(2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5),
       col=c('black', 'red', 'blue', 'green', 'purple', 'pink', 'yellow', 'brown'))

plot(c(1,2,4,8,16,32, 64, 128), test[1,], type='l', ylim=c(0,130), xlab="size of p", ylab="Speedup")
points(c(1,2,4,8,16,32, 64, 128), test[2,], type='l', col='red')
points(c(1,2,4,8,16,32, 64, 128), test[3,], type='l',col='blue')
points(c(1,2,4,8,16,32, 64, 128), test[4,], type='l', col='green')
points(c(1,2,4,8,16,32, 64, 128), test[5,], type='l', col='purple')
points(c(1,2,4,8,16,32, 64, 128), test[6,], type='l', col='pink')

legend(0, 100, 
       c('n = 10', 'n = 20', 'n = 40','n = 80','n = 160','n = 320'), 
       lty=c(1,1,1,1,1,1),
       lwd=c(2.5, 2.5, 2.5, 2.5, 2.5, 2.5),
       col=c('black', 'red', 'blue', 'green', 'purple', 'pink'))