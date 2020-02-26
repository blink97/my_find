#!/bin/sh

touch expected.txt
touch output.txt
touch diff.txt

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'


echo -e "${YELLOW}-----Threshold 0-----${NC}"
while read -r line; do
  find $line > expected.txt
  ./myfind $line > output.txt
  diff expected.txt output.txt > diff.txt
  if [ $(wc -l < diff.txt) -eq 0 ]; then 
    echo -e $line test ${GREEN}passed.${NC}
  else
    echo -e $line test ${RED}failed.${NC}
  fi
  
done < "tests/threshold0.cmd"

echo -e "${YELLOW}-----Threshold 1-----${NC}"
while read -r line; do
  find $line > expected.txt
  ./myfind $line > output.txt
  diff expected.txt output.txt > diff.txt
  if [ $(wc -l < diff.txt) -eq 0 ]; then 
    echo -e $line test ${GREEN}passed.${NC}
  else
    echo -e $line test ${RED}failed.${NC}
  fi
  
done < "tests/threshold1.cmd"

echo -e "${YELLOW}-----Threshold 2-----${NC}"
while read -r line; do
  find $line > expected.txt
  ./myfind $line > output.txt
  diff expected.txt output.txt > diff.txt
  if [ $(wc -l < diff.txt) -eq 0 ]; then 
    echo -e $line test ${GREEN}passed.${NC}
  else
    echo -e $line test ${RED}failed.${NC}
  fi
  
done < "tests/threshold2.cmd"

echo -e "${YELLOW}-----Threshold 3-----${NC}"
while read -r line; do
  find $line > expected.txt
  ./myfind $line > output.txt
  diff expected.txt output.txt > diff.txt
  if [ $(wc -l < diff.txt) -eq 0 ]; then 
    echo -e $line test ${GREEN}passed.${NC}
  else
    echo -e $line test ${RED}failed.${NC}
  fi
  
done < "tests/threshold3.cmd"

echo -e "${YELLOW}-----Threshold 4-----${NC}"
while read -r line; do
  find $line > expected.txt
  ./myfind $line > output.txt
  diff expected.txt output.txt > diff.txt
  if [ $(wc -l < diff.txt) -eq 0 ]; then 
    echo -e $line test ${GREEN}passed.${NC}
  else
    echo -e $line test ${RED}failed.${NC}
  fi
  
done < "tests/threshold4.cmd"
