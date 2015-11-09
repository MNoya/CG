#!/bin/bash

if [ `uname` == "Linux" ]
then
	make -f Makefile.linux all
else
	make -f Makefile.mac all
fi

