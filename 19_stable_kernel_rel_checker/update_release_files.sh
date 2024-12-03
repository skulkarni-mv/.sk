#!/bin/bash


chk_bugz_26=0
chk_bugz_31=0
chk_bugz_40=0
chk_bugz_50=0

rm dnloaded_4.19.y.patch
rm dnloaded_5.4.y.patch
rm dnloaded_5.10.y.patch
rm dnloaded_6.6.y.patch

echo ""
echo "Deleted downloaded patches: dnloaded_4.19.y.patch / dnloaded_5.4.y.patch / dnloaded_5.10.y.patch / dnloaded_6.6.y.patch"; echo ""


if [ -s "file_gitdiff_4.19.y" ]; then
    echo "removing file_gitdiff_4.19.y"
    rm file_gitdiff_4.19.y
    chk_bugz_26=1
fi
#################################################

if [ -s "file_gitdiff_5.4.y" ]; then
    echo "removing file_gitdiff_5.4.y"
    rm file_gitdiff_5.4.y
    chk_bugz_31=1
fi
#################################################

if [ -s "file_gitdiff_5.10.y" ]; then
    echo "removing file_gitdiff_5.10.y"
    rm file_gitdiff_5.10.y
    chk_bugz_40=1
fi
#################################################

if [ -s "file_gitdiff_6.6.y" ]; then
    echo "removing file_gitdiff_6.6.y"
    rm file_gitdiff_6.6.y
    chk_bugz_50=1
fi
#################################################


if [ $chk_bugz_26 -eq 1 ] || [ $chk_bugz_31 -eq 1 ] || [ $chk_bugz_40 -eq 1 ] || [ $chk_bugz_50 -eq 1 ]; then

	echo "In development..."
###  Still Open as of Now / UnResolved Yet
#webbrowser.open('http://bugz.mvista.com/buglist.cgi?keywords=Security;keywords_type=allwords;known_name=A1-Open_Still_bugs;bug_status=NEW;bug_status=ASSIGNED;bug_status=IN_REVIEW;bug_status=SYNC_REQ;bug_status=REOPENED;component=Kernel;component=Toolchain;component=userland;product=Carrier%20Grade;product=CentOS;product=CGX%202.0;product=CGX%202.2;product=CGX%202.4;product=CGX%202.6;product=CGX%203.1;product=CGX%204.0;product=CGX%205.0;product=Kubernetes;product=Rocky;product=Ubuntu')

else
	echo "No flag is enabled out of CGX2.6 / CGX3.1 / CGX4.0 / CGX5.0"
	echo "Not downloading CSV file to run automation script"
	echo ""
fi

