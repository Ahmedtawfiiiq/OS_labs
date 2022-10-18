#!/bin/bash

#path of parent folder
f=/home/ahmed/Documents

#path of source files
dir=/home/ahmed/Documents/dir

#save directory info
ls -lR $dir > $f/directory-info.last

#calculate current time
currentTime=$(date +%Y-%m-%d-%H-%M-%S)

#path of destination folder
backupdir=/home/ahmed/Documents/backupdir

#copy source files into destination folder
cd $backupdir
mkdir $currentTime
cp -a $dir/. $currentTime

while true
do
	sleep 5s
	ls -lR $dir > $f/directory-info.new
	flag=$(diff $f/directory-info.last $f/directory-info.new)
	if [ "$flag" != "" ]
	then
		echo "file is modified"
		cd $backupdir
		currentTime=$(date +%Y-%m-%d-%H-%M-%S)
		mkdir $currentTime
		cp -a $dir/. $currentTime
		ls -lR $dir > $f/directory-info.last
	else
		echo "file is not modified"
	fi
	rm `ls -t | awk 'NR>5'`
done
