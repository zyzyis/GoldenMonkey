#! /bin/bash

cmd="./samples/gm"

s1="./samples/simple_1_6_2_1.xml"
m1="./samples/medium_1_6_2_1.xml"
c1="./samples/complex_1_6_2_1.xml"
s0="./samples/simple_0_6_2_1.xml"
m0="./samples/medium_0_6_2_1.xml"
c0="./samples/complex_0_6_2_1.xml"

output="temp.exr"
logfile="log1.dat"
datafile="data1.dat"
times=1

# clean the old files
rm -f $datafile
rm -f $logfile

function start_test {
    for i in $(seq 1 $times);
    do
		echo Runing $i times...
		$cmd $1 $output 2>> $logfile
    done
    
    echo 'Testing ends.'
    tail -$times $logfile | awk '{print "a += ", $1, ";i += 1.0;a/i;" }' | bc -l | tail -1 >> $datafile
    echo >> $logfile
}    

start_test $s1 
start_test $s0
start_test $m1 
start_test $m0
start_test $c1
start_test $c0

