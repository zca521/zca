#!/bin/bash
# 声明一个函数
test() {

  input="$1"

    make clean
    make
    echo   "-----------------test:"${input##*/}"----------------- "
    ./all "$input" || exit
    cd ./test/genASM
    ./test.sh
    cd ../../
}

test  './test/expr.c'
test  './test/jiecheng.c'
test  './test/funcall.c'
test  './test/sort.c'
test  './test/array.c'