#/bin/bash

echo ""
echo "1 to clone oe-core-contrib /	2 to clone oe-core /	3 to clone meta-oe-contrib /	4 to clone meta-oe"
echo "5 to clone yocto-poky      /	6 to clone bitbake"

read option
echo ""

case $option in
	1) #echo "one" ;;
			echo "git clone git://git.openembedded.org/openembedded-core-contrib && cd openembedded-core-contrib"
			git clone git://git.openembedded.org/openembedded-core-contrib && cd openembedded-core-contrib
			;;

	2) #echo "two" ;;
			echo "git clone git://git.openembedded.org/openembedded-core && cd openembedded-core"
			git clone git://git.openembedded.org/openembedded-core && cd openembedded-core			
			;;

	3) #echo "three" ;;
			echo "git clone git://git.openembedded.org/meta-openembedded-contrib && cd meta-openembedded-contrib"
			git clone git://git.openembedded.org/meta-openembedded-contrib && cd meta-openembedded-contrib
			;;

	4) #echo "four" ;;
			echo "git clone git://git.openembedded.org/meta-openembedded && cd meta-openembedded"
			git clone git://git.openembedded.org/meta-openembedded && cd meta-openembedded
			;;

	5) #echo "five" ;;
			echo "git clone https://github.com/yoctoproject/poky.git && cd poky"
			git clone https://github.com/yoctoproject/poky.git && cd poky
			;;

	6) #echo "six" ;;
			echo "git clone git://git.openembedded.org/bitbake && cd bitbake"
			git clone git://git.openembedded.org/bitbake && cd bitbake
			;;

	*) echo "unknown option" ;;
esac

