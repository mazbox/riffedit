#!/bin/zsh


#!/bin/zsh
#
# 1. list afinfo xml format for the sample
# 2. grep the first line that has bitdepth in it
# 3. then remove xml tags
# 4. then remove whitespace
bitDepth=$(afinfo $1 -x | grep bit_depth -m 1 | sed -e 's/<[^>]*>//g' | sed -e 's/[[:blank:]]//g')

# riff date format is '2002-08-31'

# echo "LEI$bitDepth"
afconvert -f 'WAVE' -d "LEI$bitDepth" $1