#!/bin/bash

files=`find . -type f -name "*.h"`


for f in ${files};do
    inputChar=`chardetect $f | grep -o -E "(: )[^ ]+" | cut -d " " -f 2`
    iconv -f "$inputChar" -t UTF-8 ${f} > tmp.file && mv tmp.file ${f}
done