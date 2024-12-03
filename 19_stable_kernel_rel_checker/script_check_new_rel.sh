#!/bin/bash


RED='\033[31m'
GREEN='\033[32m'
RESET='\033[0m'

echo ""

wget https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/patch/?h=linux-4.19.y -O dnloaded_4.19.y.patch 2> /dev/null
git diff --no-index prev_release_4.19.y.patch dnloaded_4.19.y.patch > file_gitdiff_4.19.y
cat file_gitdiff_4.19.y | grep Subject
rm -f check_n_Delete_4.19.y

if [ -s "file_gitdiff_4.19.y" ]; then
    echo -e "${RED}"
    (echo "Subject: linux-4.19.y New Stable Release Available"; echo ""; cat file_gitdiff_4.19.y | grep Subject) > output &&  git send-email --to=skulkarni@mvista.com --confirm=never --quiet output ; rm output
    echo -e "${RESET}"

    cp dnloaded_4.19.y.patch prev_release_4.19.y.patch       #| tee -a check_n_Delete_4.19.y
    echo "Updated the prev_release file as new update is available. Used cmd:" | tee check_n_Delete_4.19.y
    echo "cp dnloaded_4.19.y.patch prev_release_4.19.y.patch" | tee -a check_n_Delete_4.19.y
    echo ""

else
    rm file_gitdiff_4.19.y
    echo -e "${GREEN} No changes in stable release 4.19.y ${RESET}"
    cat prev_release_4.19.y.patch | grep Subject
    echo ""
fi

echo "---------------------------------------------------------------------------------------------------------------------------------"; echo ""
sleep 2

#################################################


wget https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/patch/?h=linux-5.4.y -O dnloaded_5.4.y.patch 2> /dev/null
git diff --no-index prev_release_5.4.y.patch dnloaded_5.4.y.patch > file_gitdiff_5.4.y
cat file_gitdiff_5.4.y | grep Subject
rm -f check_n_Delete_5.4.y

if [ -s "file_gitdiff_5.4.y" ]; then
    echo -e "${RED}"
    (echo "Subject: linux-5.4.y New Stable Release Available"; echo ""; cat file_gitdiff_5.4.y | grep Subject) > output &&  git send-email --to=skulkarni@mvista.com --confirm=never --quiet output ; rm output
    echo -e "${RESET}"

    cp dnloaded_5.4.y.patch prev_release_5.4.y.patch       #| tee -a check_n_Delete_5.4.y
    echo "Updated the prev_release file as new update is available. Used cmd:" | tee check_n_Delete_5.4.y
    echo "cp dnloaded_5.4.y.patch prev_release_5.4.y.patch" | tee -a check_n_Delete_5.4.y
    echo ""

else
    rm file_gitdiff_5.4.y
    echo -e "${GREEN} No changes in stable release 5.4.y ${RESET}"
    cat prev_release_5.4.y.patch | grep Subject
    echo ""
fi

echo "---------------------------------------------------------------------------------------------------------------------------------"; echo ""
sleep 2

#################################################


wget https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/patch/?h=linux-5.10.y -O dnloaded_5.10.y.patch 2> /dev/null
git diff --no-index prev_release_5.10.y.patch dnloaded_5.10.y.patch > file_gitdiff_5.10.y
cat file_gitdiff_5.10.y | grep Subject
rm -f check_n_Delete_5.10.y

if [ -s "file_gitdiff_5.10.y" ]; then
    echo -e "${RED}"
    (echo "Subject: linux-5.10.y New Stable Release Available"; echo ""; cat file_gitdiff_5.10.y | grep Subject) > output &&  git send-email --to=skulkarni@mvista.com --confirm=never --quiet output ; rm output
    echo -e "${RESET}"

    cp dnloaded_5.10.y.patch prev_release_5.10.y.patch       #| tee -a check_n_Delete_5.10.y
    echo "Updated the prev_release file as new update is available. Used cmd:" | tee check_n_Delete_5.10.y
    echo "cp dnloaded_5.10.y.patch prev_release_5.10.y.patch" | tee -a check_n_Delete_5.10.y
    echo ""

else
    rm file_gitdiff_5.10.y
    echo -e "${GREEN} No changes in stable release 5.10.y ${RESET}"
    cat prev_release_5.10.y.patch | grep Subject
    echo ""
fi

echo "---------------------------------------------------------------------------------------------------------------------------------"; echo ""
sleep 2

###################################################


wget https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/patch/?h=linux-6.6.y -O dnloaded_6.6.y.patch 2> /dev/null
git diff --no-index prev_release_6.6.y.patch dnloaded_6.6.y.patch > file_gitdiff_6.6.y
cat file_gitdiff_6.6.y | grep Subject
rm -f check_n_Delete_6.6.y

if [ -s "file_gitdiff_6.6.y" ]; then
    echo -e "${RED}"
    (echo "Subject: linux-6.6.y New Stable Release Available"; echo ""; cat file_gitdiff_6.6.y | grep Subject) > output &&  git send-email --to=skulkarni@mvista.com --confirm=never --quiet output ; rm output
    echo -e "${RESET}"

    cp dnloaded_6.6.y.patch prev_release_6.6.y.patch       #| tee -a check_n_Delete_6.6.y
    echo "Updated the prev_release file as new update is available. Used cmd:" | tee check_n_Delete_6.6.y
    echo "cp dnloaded_6.6.y.patch prev_release_6.6.y.patch" | tee -a check_n_Delete_6.6.y
    echo ""

else
    rm file_gitdiff_6.6.y
    echo -e "${GREEN} No changes in stable release 6.6.y ${RESET}"
    cat prev_release_6.6.y.patch | grep Subject
    echo ""
fi

echo "---------------------------------------------------------------------------------------------------------------------------------"; echo ""
sleep 2
