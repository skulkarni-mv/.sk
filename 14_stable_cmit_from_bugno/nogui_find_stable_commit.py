#!/usr/bin/python3

import time
import webbrowser
import sys
import argparse

from bs4 import BeautifulSoup
import requests
from colorama import Fore


def check_stable_fix(kern_vers, url):

    response = requests.get(url)

    if response.status_code == 200:
        soup = BeautifulSoup(response.content, 'html.parser')

        count_insertions = len( soup.find_all('span', {'class': 'insertions'} ) )	# insertions in commit
        count_deletions  = len( soup.find_all('span', {'class': 'deletions'} ) )	# deletions in commit


        maximum = max(count_insertions, count_deletions)	# Possibly some commits might have only instns/deltns. Get max(0,1) OR max(1,0)

        if maximum > 1:									# Multiple(in case of Revert)
            print(kern_vers, '->', Fore.RED+' Found total :', maximum , '. PLEASE CHECK MANUALLY. Latest commit out of them:', Fore.RESET)
        elif maximum == 1:
            print(kern_vers, '-> Found total :'+Fore.GREEN, maximum , Fore.RESET)	# Single commit
        else:
            print(kern_vers, '-> Found total :'  +Fore.RED, maximum , Fore.RESET)	# No git commit available for the requested MasterCmitID


        if maximum > 0:

            link_text = soup.find('div', { 'class': 'content'})		# Will have 2 or more. 1st-> request data  2nd+ -> required cmit link/s


            if link_text:
                git_link_all = link_text.find_all('a')

                git_link = git_link_all[1]				# At least 2 expected as max>0. Take 1st cmit if many available

                if git_link:
                  print(Fore.GREEN + " https://git.kernel.org"+f"{git_link['href']}" + Fore.RESET)

                  fp=open("dumped_data.txt", "a+")
                  fp.write(" "+"https://git.kernel.org"+f"{git_link['href']}")
                  fp.close()

                else:
                  print(Fore.RED + f"\t Fix not found" + Fore.RESET)

                  fp=open("dumped_data.txt", "a+")
                  fp.write(" -")
                  fp.close()
#                  sys.exit(1)

        else:
            print(Fore.RED + f"\t No fix found." + Fore.RESET)
            fp=open("dumped_data.txt", "a+")
            fp.write(" -")
            fp.close()
#            sys.exit(1)

    else:
        print(f"\t Failed to retrieve webpage. Status code: {response.status_code}")
        fp=open("dumped_data.txt", "a+")
        fp.write(" -")
        fp.close()
#        sys.exit(1)



def check_fix_mvista_gitcgx(cgx_vers_num, msd_branch, mast_cmit):

    url="https://gitcgx.mvista.com/cgit/CGX"+cgx_vers_num+"/kernel/linux-mvista-"+cgx_vers_num+".git/log/?h="+msd_branch+"&qt=grep&q="+mast_cmit
    response = requests.get(url)

    if response.status_code == 200:
        soup = BeautifulSoup(response.content, 'html.parser')

        count_insertions = len( soup.find_all('span', {'class': 'insertions'} ) )	# insertions in commit
        count_deletions  = len( soup.find_all('span', {'class': 'deletions'} ) )	# deletions in commit

        maximum = max(count_insertions, count_deletions)	# Possibly some commits might have only instns/deltns. Get max(0,1) OR max(1,0)

        if maximum > 1:
            print(cgx_vers_num, '-> Found total :'+Fore.GREEN, maximum , Fore.RESET+'. Latest commit out of them:') # Multiple(in case of Revert)
        elif maximum == 1:
            print(cgx_vers_num, '-> Found total :'+Fore.GREEN, maximum , Fore.RESET)	# Single commit
        else:
            print(cgx_vers_num, '-> Found total :'  +Fore.RED, maximum , Fore.RESET)	# No git commit available for the requested MasterCmitID


        if maximum > 0:

            link_text = soup.find('div', { 'class': 'content'})		# Will have 2 or more. 1st-> request data  2nd+ -> required cmit link/s


            if link_text:
                git_link_all = link_text.find_all('a')

                git_link = git_link_all[1]				# At least 2 expected as max>0. Take 1st cmit if many available
                print(git_link)				# At least 2 expected as max>0. Take 1st cmit if many available

                if git_link:
                  print(Fore.YELLOW + " https://git.kernel.org"+f"{git_link['href']}" + Fore.RESET)

                else:
                  print(Fore.RED + f"\t Fix not found" + Fore.RESET)

        else:
            print(Fore.RED + f"\t No fix found." + Fore.RESET)

    else:
        print(f"\t Failed to retrieve webpage. Status code: {response.status_code}")




### MAIN

try:
    parser = argparse.ArgumentParser()
    parser.add_argument("MasterCommit_ID", type=str)
    args = parser.parse_args()

    print("\n"+"Argument passed: ", sys.argv[1])

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)



commit = sys.argv[1]


#webbrowser.open('https://www.google.com/')
#time.sleep(0.5)

url4_14 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-4.14.y&qt=grep&q={commit}"
url4_19 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-4.19.y&qt=grep&q={commit}"
url5_4  = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-5.4.y&qt=grep&q={commit}"
url5_10 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-5.10.y&qt=grep&q={commit}"
url5_15 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-5.15.y&qt=grep&q={commit}"
url6_6  = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-6.6.y&qt=grep&q={commit}"
url6_9  = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-6.9.y&qt=grep&q={commit}"

###webbrowser.open(f"https://github.com/gregkh/linux/commit/{commit}")
#time.sleep(0.5)

#check_stable_fix('4.14', url4_14)		# CGX2.4
#time.sleep(0.1)

#check_stable_fix('4.19', url4_19)		# CGX2.6
#time.sleep(0.1)

#check_stable_fix('5.4 ', url5_4)		# CGX3.1
#time.sleep(0.1)

#check_stable_fix('5.10', url5_10)		# CGX4.0
#time.sleep(0.1)

#check_stable_fix('5.15', url5_15)
#time.sleep(0.1)

check_stable_fix('6.6', url6_6)		# CGX5.0
time.sleep(0.1)

#check_fix_mvista_gitcgx('5.0', 'mvl-6.6/msd.cgx', commit)
#time.sleep(0.1)

###webbrowser.open(url6_9)
#time.sleep(0.1)


## Test data: 25 Jun 2024 [class="age-days" availability might change acc to the date on which prog is ran]
# one link: nogui_find_stable_commit.py 2f945a792f67815abca26fa8a5e863ccf3fa1181 (1+/1-) [class="age-days" NotAvailable]
# one link: nogui_find_stable_commit.py 9fe2816816a3c765dff3b88af5b5c3d9bbb911ce (1+/2-) [class="age-days" Available]
#  no fix : nogui_find_stable_commit.py e9edc188fc76499b0b9bd60364084037f6d03773

