#!/bin/bash

echo 'please modify the $names variable to be your own id'
names=simplespy #modify to be your own id
echo $names
sleep 2
timeo=180s

function _SC() #safe call
{
echo "[RUNNING] : $*"
eval $*
if [ $? -ne 0 ]; then
echo "[ERROR] : command $* failed, please check, exiting..."
exit
fi
}

outdir=$(pwd)

mkdir -p students

if [ ! -d mips-simulator-testcases ]; then
_SC git clone git@github.com:BreakVoid/mips-simulator-testcases.git
else
_SC cd mips-simulator-testcases
_SC git pull
_SC cd ..
fi

normaldir=$(pwd)/mips-simulator-testcases/testsuit-1
resdir=$(pwd)/results
mkdir -p $resdir
statistics=$resdir/statistics
#rm -rf $statistics

for name in ${names[@]}; do
cd $outdir
diff_result_dir=$resdir/diff_result_$name
rm -rf $diff_result_dir
mkdir -p $diff_result_dir
log_file=$resdir/${name}.LOG
rm -rf $log_file
score=0
full_score=0
echo now testing ${name}...
_SC cd students
if [ ! -d $name ]; then
_SC mkdir $name "&&" cd $name
_SC git clone "git@bitbucket.org:${name}/mips-simulator.git"
_SC cd ..
fi
_SC cd $name/mips-simulator
_SC git checkout -f master
_SC git pull
_SC git fetch --tags
_SC git checkout -f final #pay attention to your tag! someone don't have the midterm tag
_SC make clean #some one don't have make clean
_SC make
_SC cat ./setvars.sh

unset -v MSCK
set -x
source ./setvars.sh #can't ensure SC here
set +x
echo MSCK=$MSCK

_SC cd bin
for filec in $(ls $normaldir/*.s); do
filein=${filec%.s}.in
fileout=${filec%.s}.ans
_SC cp $filec data.s
echo "TESTING " + $(filec)
if [ ! -f $filein ]; then
timeout $timeo $MSCK data.s 1>msckout 2>/dev/null
else
timeout $timeo $MSCK data.s <$filein 1>msckout 2>/dev/null
fi
pure_file_name=$(basename $filec)
pure_file_name=${pure_file_name%.s}
diff msckout $fileout >$diff_result_dir/$pure_file_name
full_score=$((full_score+1))
file_limit=${filec%.mx}.limit
if [ ! -s $diff_result_dir/$pure_file_name ]; then
score=$((score+1))
else
echo ${filec%.mx} : FAILED >>$log_file
echo FAILED
fi
sleep 1
done
echo count: $score/$full_score >>$log_file
echo count: $score/$full_score
echo $name $score/$full_score >>$statistics
_SC cd ..
_SC git checkout -f master
cd $outdir
done