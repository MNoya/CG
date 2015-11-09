#!/bin/bash

if [ `uname` == "Linux" ]
then
	make -f Makefile.linux clean
else
	make -f Makefile.mac clean
fi

