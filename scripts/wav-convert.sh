#!/bin/zsh



# 1. list afinfo xml format for the sample
# 2. grep the first line that has bitdepth in it
# 3. then remove xml tags
# 4. then remove whitespace
bitDepth=$(afinfo $1 -x | grep bit_depth -m 1 | sed -e 's/<[^>]*>//g' | sed -e 's/[[:blank:]]//g')
creationDate=$(creation-date.sh $1)
echo "==============================";
echo "$1"
echo "Creation Date: $creationDate"
echo "Bit depth: $bitDepth"

# riff date format is '2002-08-31'

# echo "LEI$bitDepth"
wav=${1%.*}.wav
afconvert -f 'WAVE' -d "LEI$bitDepth" $1 -o $wav
riffedit $wav -w 'DTIM' "$creationDate" 'IENG' 'Marek Bereza' 'ISFT' 'Koala Sampler' 'TAPE' "$1"