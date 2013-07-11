#! /bin/bash

cmd="./samples/gm"

s0="./samples/simple_1_6_0_2.xml"
m0="./samples/medium_1_6_0_2.xml"
c0="./samples/complex_1_6_0_2.xml"
s1="./samples/simple_1_6_1_2.xml"
m1="./samples/medium_1_6_1_2.xml"
c1="./samples/complex_1_6_1_2.xml"
s2="./samples/simple_1_6_2_2.xml"
m2="./samples/medium_1_6_2_2.xml"
c2="./samples/complex_1_6_2_2.xml"


output="temp.exr"
logfile="log2.dat"
datafile="data2.dat"
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
#    tail -5 $logfile | awk '{print "a += " $1, ";i += 1.0;a/i;" }' | bc -l | tail -1 >> $datafile
    echo >> $logfile
}

echo 'test simple scene 480p, XGA, 720p'
start_test $s0
start_test $s1
start_test $s2

echo 'test medium scene 480p, XGA, 720p'
start_test $m0
start_test $m1
start_test $m2

echo 'test complex scene 480p, XGA, 720p'
start_test $c0
start_test $c1
start_test $c2


