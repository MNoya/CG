#!/bin/bash

if [ `uname` == "Linux" ]
then
	make -f Makefile.linux run
else
	make -f Makefile.mac run
fi
