#!/usr/bin/env/Rscript

powmon.plot.ibm <- function(fname_skt0, fname_skt1)
{
    d0 <- read.table(fname_skt0,
                     header = T,
                     sep = "|",
                     comment.char = "",
                     check.names = F)
    d1 <- read.table(fname_skt1,
                     header = T,
                     sep = "|",
                     comment.char = "",
                     check.names = F)

    prec = 3

    cat("****Sample Counters****\n")
    cat("               Socket 0    Socket 1\n")
    cat("==================================\n")
    cat("PWRSYS Scale   ", round(mean(d0$PWRSYS_Scale_W), digits = prec), "W  ",
                           "  NA\n",
        sep="")
    cat("PWRSYS Energy  ", round(mean(d0$PWRSYS_Energy_J), digits = prec), "J  ",
                           "NA\n",
        sep="")
    cat("PWRGPU Scale   ", round(mean(d0$PWRGPU_Scale_W), digits = prec), "W     ",
                           round(mean(d1$PWRGPU_Scale_W), digits = prec), "W\n",
        sep="")
    cat("PWRGPU Energy  ", round(mean(d0$PWRGPU_Energy_J), digits = prec), "J  ",
                           round(mean(d1$PWRGPU_Energy_J), digits = prec), "J\n",
        sep="")
    cat("PWRPROC Scale  ", round(mean(d0$PWRPROC_Scale_W), digits = prec), "W    ",
                           round(mean(d1$PWRPROC_Scale_W), digits = prec), "W\n",
        sep="")
    cat("PWRPROC Energy ", round(mean(d0$PWRPROC_Energy_J), digits = prec), "J  ",
                           round(mean(d1$PWRPROC_Energy_J), digits = prec), "J\n",
        sep="")
    cat("PWRVDD Scale   ", round(mean(d0$PWRVDD_Scale_W), digits = prec), "W     ",
                           round(mean(d1$PWRVDD_Scale_W), digits = prec), "W\n",
        sep="")
    cat("PWRVDD Energy  ", round(mean(d0$PWRVDD_Energy_J), digits = prec), "J   ",
                           round(mean(d1$PWRVDD_Energy_J), digits = prec), "J\n",
        sep="")
    cat("PWRVDN Scale   ", round(mean(d0$PWRVDN_Scale_W), digits = prec), "W         ",
                           round(mean(d1$PWRVDN_Scale_W), digits = prec), "W\n",
        sep="")
    cat("PWRVDN Energy  ", round(mean(d0$PWRVDN_Energy_J), digits = prec), "J   ",
                           round(mean(d1$PWRVDN_Energy_J), digits = prec), "J\n",
        sep="")
    cat("PWRMEM Scale   ", round(mean(d0$PWRMEM_Scale_W), digits = prec), "W     ",
                           round(mean(d1$PWRMEM_Scale_W), digits = prec), "W\n",
        sep="")
    cat("PWRMEM Energy  ", round(mean(d0$PWRMEM_Energy_J), digits = prec), "J   ",
                           round(mean(d1$PWRMEM_Energy_J), digits = prec), "J\n",
        sep="")
    cat("TEMPNEST       ", round(mean(d0$TEMPNEST_C), digits = prec), "C      ",
                           round(mean(d1$TEMPNEST_C), digits = prec), "C\n",
        sep="")
    cat("UTIL           ", round(mean(d0[["UTIL_%"]]), digits = prec), "%     ",
                           round(mean(d1[["UTIL_%"]]), digits = prec), "%\n",
        sep="")
    cat("FREQA          ", round(mean(d0$FREQA_MHz), digits = prec), "MHz     ",
                           round(mean(d1$FREQA_MHz), digits = prec), "MHz\n",
        sep="")
    cat("IPS            ", round(mean(d0$IPS_MIP), digits = prec), "MIP ",
                           round(mean(d1$IPS_MIP), digits = prec), "MIP\n",
        sep="")
    cat("MEMOTTHROT     ", round(mean(d0[["MEMOTTHROT_#"]]), digits = prec), "#          ",
                           round(mean(d1[["MEMOTTHROT_#"]]), digits = prec), "#\n",
        sep="")
}

args <- commandArgs(trailingOnly = T)
if (length(args) != 2) {
    stop("Missing powmon dat files (per socket).")
}
powmon.plot.ibm(args[1], args[2])

#
