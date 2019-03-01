#!/bin/bash

testFileCount=$(find . -name "*.test.cpp" | wc -l)

testCount=0
for testFile in $(cat "tests.txt")
do
  if [[ $testFile =~ \# ]] || [[ $testFile != *".test.cpp" ]]; then continue; fi
  (( testCount += 1 ))
  echo ">${testFile}"
  outputFile="${testFile}.out"
  g++ -m32 -I.. -D__TESTING__ $1 $testFile -o $outputFile && $outputFile
  result=$?
  if [[ $result != 0 ]]; then exit $result; fi
done

echo "OK ${testCount}"

if [[ $testCount !=  $testFileCount ]]
then
  echo "WARNING: Only ${testCount} of the ${testFileCount} *.test.cpp files are registered in tests.txt"
fi

exit 0
