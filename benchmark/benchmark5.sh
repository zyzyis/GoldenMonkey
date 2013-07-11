#! /bin/bash

cmd="./samples/gm"

c11="./samples/complex_1_1_2_2.xml"
c12="./samples/complex_1_2_2_2.xml"
c13="./samples/complex_1_3_2_2.xml"
c14="./samples/complex_1_4_2_2.xml"
c15="./samples/complex_1_5_2_2.xml"
c16="./samples/complex_1_6_2_2.xml"

c01="./samples/complex_0_1_2_2.xml"
c02="./samples/complex_0_2_2_2.xml"
c03="./samples/complex_0_3_2_2.xml"
c04="./samples/complex_0_4_2_2.xml"
c05="./samples/complex_0_5_2_2.xml"
c06="./samples/complex_0_6_2_2.xml"

s11="./samples/simple_1_1_2_2.xml"
s12="./samples/simple_1_2_2_2.xml"
s13="./samples/simple_1_3_2_2.xml"
s14="./samples/simple_1_4_2_2.xml"
s15="./samples/simple_1_5_2_2.xml"
s16="./samples/simple_1_6_2_2.xml"

s01="./samples/simple_0_1_2_2.xml"
s02="./samples/simple_0_2_2_2.xml"
s03="./samples/simple_0_3_2_2.xml"
s04="./samples/simple_0_4_2_2.xml"
s05="./samples/simple_0_5_2_2.xml"
s06="./samples/simple_0_6_2_2.xml"

m11="./samples/medium_1_1_2_2.xml"
m12="./samples/medium_1_2_2_2.xml"
m13="./samples/medium_1_3_2_2.xml"
m14="./samples/medium_1_4_2_2.xml"
m15="./samples/medium_1_5_2_2.xml"
m16="./samples/medium_1_6_2_2.xml"

m01="./samples/medium_0_1_2_2.xml"
m02="./samples/medium_0_2_2_2.xml"
m03="./samples/medium_0_3_2_2.xml"
m04="./samples/medium_0_4_2_2.xml"
m05="./samples/medium_0_5_2_2.xml"
m06="./samples/medium_0_6_2_2.xml"

output="temp.exr"
logfile="log5.dat"
datafile="data5.dat"
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
	
	tail -$times $logfile |	awk '{print "a =", $1, "; b =", $2, "; c = a - b; d = 100.0-", $3, "/c*100.0;d"}' | bc -l \
		| awk '{print "a += " $1, ";i += 1.0;a/i;" }' | bc -l | tail -1 >> $datafile
    echo >> $logfile
}

echo 'test simple scene with opti'
start_test $c11
start_test $c12
start_test $c13
start_test $c14
start_test $c15
start_test $c16

echo 'test simple scene without opti'
start_test $c01
start_test $c02
start_test $c03
start_test $c04
start_test $c05
start_test $c06

