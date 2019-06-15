# a)

echo -e "\n ===== a) ===== \n"

ps aux | awk \
'
NR != 1 { printf "User %-9s - PID %5d ( CPU: %4g, MEM: %g)\n",$1,$2,$3,$4; }
'

# b)

echo -e "\n ===== b) ===== \n"

ps aux | awk \
'
NR != 1 {
    split($10, time, ":");
    if(time[1] == 0 && time[2] >= 15 && time[2] <= 45) {
        printf "Process %-25s - time %s\n",$11,$10;
    }
}
'

# c)

echo -e "\n ===== c) ===== \n"

ps aux | awk \
'
BEGIN { sumCpu = 0; sumMem = 0 }
NR != 1 { sumCpu += $3; sumMem += $4 }
END { printf "Sum CPU: %g\nSum MEM: %g\n",sumCpu,sumMem }
'

# d)

echo -e "\n ===== d) ===== \n"

ps aux | awk \
'
NR != 1 { usersCpu[$1] += $3; usersMem[$1] += $4 }
END {
    for(user in usersCpu) {
        printf "User %-10s - Sum CPU: %5g - Sum MEM: %g\n",user,usersCpu[user],usersMem[user];
    }
}
'

# e)

echo -e "\n ===== e) ===== \n"

ps aux | awk \
'
BEGIN { 
    maxCpu = -1;  maxCpuPid = "NONE";
    maxMem = -1; maxMemPid = "NONE";
    maxTime = -1; maxTimePid = "NONE";
    maxPid = -1;
}
NR != 1 { 
    pidsName[$2] = $11;
    
    if($3 > maxCpu) {
        maxCpuPid = $2;
        maxCpu = $3;
    }
    if($4 > maxMem) {
        maxMemPid = $2;
        maxMem = $4;
    }
    split($10, time, ":");
    mins = time[1]*60 + time[2];
    if(mins > maxTime) {
        maxTimePid = $2;
        maxTime = mins;
    }
    if($2 > maxPid) {
        maxPid = $2;
    }
}
END {
    printf "Process %-34s PID %-5s - Max CPU: %g\n",pidsName[maxCpuPid],maxCpuPid,maxCpu;
    printf "Process %-34s PID %-5s - Max MEM: %g\n",pidsName[maxMemPid],maxMemPid,maxMem;
    hrs = int(maxTime / 60);
    min = maxTime - hrs * 60;
    printf "Process %-34s PID %-5s - Max Time: %g:%g\n",pidsName[maxMemPid],maxMemPid,hrs,min;
    printf "Process %-34s PID %-5s - Max PID: %g\n",pidsName[maxPid],maxPid,maxPid;
}
'
