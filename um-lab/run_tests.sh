#! /bin/sh

myvar="hello"
echo $myvar

echo $0
echo $1

testFiles=$(ls *.um)
echo $testFiles

for testFile in $testFiles ; do
    testName=$(echo $testFile | sed -E 's/(.*).um/\1/')
    echo $testName
done

for testFile in $testFiles ; do
    testName=$(echo $testFile | sed -E 's/(.*).um/\1/')
    dumpFileName=$testName".dump"
    expectedOutputFileName=$testName".1"
    if [ -f $expectedOutputFileName ] ; then 
        umdump $testFile > $dumpFileName        
    fi
done

if [ -f $outputFile ] ; then
    echo "exists!"
else 
    echo "does not exist!"
fi