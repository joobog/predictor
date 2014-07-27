# http://www.cyclismo.org/tutorial/R/tables.html#id4

#install.packages("optparse")
suppressPackageStartupMessages(library("optparse"))



option_list <- list(
										make_option(c("-n", "--add_numbers"), action="store_true", default=FALSE,
																help="Print line number at the beginning of each line [default]")
										)
parser <- OptionParser(usage = "%prog [options] file", option_list=option_list)
arguments <- parse_args(parser, positional_arguments = 1)
opt <- arguments$options
file <- arguments$args




#d = read.csv("../data/dataset3-McSvmOVATrainer-output.csv", sep = ";", header=T)

d = read.csv(file, sep = ";", header=T)

#Data.in.Byte Gap.in.Byte Datasieving.1.on Buffer.in.MByte Performance.in.MByte.s Prediction.in.MByte.s


plot(d)
plot(d$Prediction.in.MByte.s)

delta = d$Prediction.in.MByte.s - d$Performance.in.MByte.s
print("Abweichungen (positive)")
summary(abs(delta))

print("Abweichungen")
summary(delta)
png('delta.png', width=1000, height=800)
plot(delta)
hist(delta, breaks=100)
print("Standard Abweichung (angewendet auf Delta)")
sd(delta)

# select all cases in which the prediction and the real value differ by at least 15
#d1 = subset(d, abs(d[,5] - d[,6]) > 15)
# Alternative, all jene die um 20 % Abweichen
d1 = subset(d, abs(d[,5] - d[,6]) > 0.2* d[,5])

# how many cases do not match:
nrow(d1)

# compare both cases
plot( d1$Prediction.in.MByte.s, d1$Performance.in.MByte.s)



# Outliers Plotten
png('outliers.png', width=2000, height=2000)
plot(d1)
dev.off()


# Overlay graph to reveal which points are broken.

colors <- rainbow(3)
linetype <- c(1:3)
plotchar <- seq(18,18+3,1)

xrange <- range( d$Data.in.Byte )
yrange <- range( d$Performance.in.MByte.s)
plot(xrange, yrange, type="n", xlab="Data size in Byte", ylab="Performance in MByte")

# plot those points that are wrongly predicted
lines( d$Data.in.Byte, d$Performance.in.MByte.s, type="p", lwd=1.5, lty=linetype[1], col=colors[1], pch=plotchar[1])
lines( d1$Data.in.Byte, d1$Performance.in.MByte.s, type="p", lwd=1.5, lty=linetype[2], col=colors[2], pch=plotchar[2])



#Plot To File

png('holeVSOld.png', width=1000, height=800)
xrange <- range( d$Gap.in.Byte )
yrange <- range( d$Performance.in.MByte.s)
plot(xrange, yrange, type="n", xlab="Hole size in Byte", ylab="Performance in MByte")

# plot those points that are wrongly predicted
lines( d$Gap.in.Byte, d$Performance.in.MByte.s, type="p", lwd=1.5, lty=linetype[1], col=colors[1], pch=plotchar[1])
lines( d1$Gap.in.Byte, d1$Performance.in.MByte.s, type="p", lwd=1.5, lty=linetype[2], col=colors[2], pch=plotchar[2])
dev.off()


xrange <- range( d$Datasieving.1.on )
yrange <- range( d$Performance.in.MByte.s)
plot(xrange, yrange, type="n", xlab="Datasieving", ylab="Performance in MByte")
lines( d$Datasieving.1.on, d$Performance.in.MByte.s, type="p", lwd=1.5, lty=linetype[1], col=colors[1], pch=plotchar[1])
lines( d1$Datasieving.1.on, d1$Performance.in.MByte.s, type="p", lwd=1.5, lty=linetype[2], col=colors[2], pch=plotchar[2])



# identify histogram differences
#install.packages("plotrix")
require(plotrix)
multhist( list(d$Performance.in.MByte.s, d$Prediction.in.MByte.s), breaks=100 )

# The ones that are > 15 MiB different:
multhist( list(d1$Performance.in.MByte.s, d1$Prediction.in.MByte.s), breaks=30 )

png('direct-comparison.png', width=1000, height=800)
xvec <- seq(from=1, to=nrow(d1))
xrange <- c(1, nrow(d1))
yrange <- range( d1$Performance.in.MByte.s)
plot(xrange, yrange, type="n", xlab="Index", ylab="Performance in MByte")
lines( xvec, d1$Prediction.in.MByte.s, type="p", lwd=1.5,
lty=linetype[2], col=colors[2], pch=plotchar[2])
lines( xvec, d1$Performance.in.MByte.s, type="p", lwd=1.5,
lty=linetype[1], col=colors[1], pch=plotchar[1])
dev.off()


png('direct-comparison-all.png', width=1000, height=800)
xvec <- seq(from=1, to=nrow(d))
xrange <- c(1, nrow(d))
yrange <- range( d$Performance.in.MByte.s)
plot(xrange, yrange, type="n", xlab="Index", ylab="Performance in MByte")
lines( xvec, d$Prediction.in.MByte.s, type="p", lwd=1.5,
lty=linetype[2], col=colors[2], pch=plotchar[2])
lines( xvec, d$Performance.in.MByte.s, type="p", lwd=1.5,
lty=linetype[1], col=colors[1], pch=plotchar[1])
dev.off()



d1 = subset(d, d[,2] < 524288)

xvec <- seq(from=1, to=nrow(d1))
xrange <- c(1, nrow(d1))
yrange <- range( d1$Performance.in.MByte.s)
png('small_gaps.png', width=1000, height=800)
plot(xrange, yrange, type="n", xlab="Index", ylab="Performance in MByte")
lines( xvec, d1$Prediction.in.MByte.s, type="p", lwd=1.5,
lty=linetype[2], col=colors[2], pch=plotchar[2])
lines( xvec, d1$Performance.in.MByte.s, type="p", lwd=1.5,
lty=linetype[1], col=colors[1], pch=plotchar[1])
