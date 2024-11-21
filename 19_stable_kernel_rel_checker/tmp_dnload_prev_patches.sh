#!/bin/bash

echo ""

wget "https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/patch/?h=linux-4.19.y&id=a8c422f8f7233300271453594f40da83c513d22c" -O		prev_release_4.19.y.patch
echo "---------------------------------------------------------------------------------------------------------------------------------"; echo ""

wget "https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/patch/?h=linux-5.4.y&id=6b8cbbd5977d7d7a2d6a466b46875b18a98b53d0" -O		prev_release_5.4.y.patch
echo "---------------------------------------------------------------------------------------------------------------------------------"; echo ""

wget "https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/patch/?h=linux-5.10.y&id=b874a7e65b4f3cbd3cf979d653772732958eb443" -O prev_release_5.10.y.patch
echo "---------------------------------------------------------------------------------------------------------------------------------"; echo ""

wget "https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/patch/?h=linux-6.6.y&id=f1ab3a1bcbbc98427ec6dc4343fcb5a74bacea34" -O prev_release_6.6.y.patch
echo "---------------------------------------------------------------------------------------------------------------------------------"; echo ""
