#! /bin/bash

cmd="./samples/gm"


c10="./samples/complex_1_6_0_2.xml"
c11="./samples/complex_1_6_1_2.xml"
c12="./samples/complex_1_6_2_2.xml"

c00="./samples/complex_0_6_0_2.xml"
c01="./samples/complex_0_6_1_2.xml"
c02="./samples/complex_0_6_2_2.xml"

output="temp.exr"
logfile="log4.dat"
datafile="data4.dat"
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
	
    tail -$times $logfile | awk '{print "a =", $1, "; b =", $2, "; c = a - b; 1000.0/c"}' | bc -l \
	| awk '{print "a += " $1, ";i += 1.0;a/i;" }' | bc -l | tail -1 >> $datafile
    echo >> $logfile
}

echo 'test simple scene with opti'
start_test $c10
start_test $c11
start_test $c12

echo 'test simple scene without opti'
start_test $c00
start_test $c01
start_test $c02


