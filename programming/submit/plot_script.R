#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

# test if there is at least one argument: if not, return an error
if (length(args)==0) {
  stop("At least one argument must be supplied (input file).n", call.=FALSE)
} else if (length(args)==1) {
  # default output file
  args[2] = "life_image.pdf"
}

## preparing data
data<-read.csv(file=args[1], sep=" ", header=FALSE)
image_data<-matrix(data[1,1]*data[1,2], data[1,1], data[1,2])

for(i in 2:dim(data)[1]){
  image_data[data[i,1], data[i,2]] <- 1
}

# plotting to file
pdf(file = args[2])
image(image_data, main="Conway's Game of Life", xaxt='n', yaxt='n', ylab=paste("y size: ", data[1,2]), xlab=paste("x size: ", data[1,1]))
dev.off()