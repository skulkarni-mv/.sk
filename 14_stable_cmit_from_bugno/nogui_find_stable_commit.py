#!/usr/bin/python3

import time
import webbrowser
import sys
import argparse

from bs4 import BeautifulSoup
import requests
from colorama import Fore


def check_stable_fix(cgx_vers, kern_vers, url):

    response = requests.get(url)

    if response.status_code == 200:
        soup = BeautifulSoup(response.content, 'html.parser')

        count_insertions = len( soup.find_all('span', {'class': 'insertions'} ) )	# insertions in commit
        count_deletions  = len( soup.find_all('span', {'class': 'deletions'} ) )	# deletions in commit


        maximum = max(count_insertions, count_deletions)	# Possibly some commits might have only instns/deltns. Get max(0,1) OR max(1,0)

        if maximum > 1:									# Multiple(in case of Revert)
            print(Fore.YELLOW+ cgx_vers +Fore.CYAN, kern_vers,'->',Fore.RED+' Found total :', maximum,'. Latest/REVERT out of them:', Fore.RESET)
        elif maximum == 1:								# Single commit
            print(Fore.YELLOW+ cgx_vers +Fore.CYAN, kern_vers +Fore.RESET, '-> Found total :'+Fore.GREEN, maximum , Fore.RESET)
        else:										# No git commit available for the requested MasterCmitID
            print(Fore.YELLOW+ cgx_vers +Fore.CYAN, kern_vers +Fore.RESET, '-> Found total :'  +Fore.RED, maximum , Fore.RESET)


        if maximum > 0:

            link_text = soup.find('div', { 'class': 'content'})		# Will have 2 or more. 1st-> request data  2nd+ -> required cmit link/s


            if link_text:
                git_link_all = link_text.find_all('a')			# At least 2, 1st search querry, 2nd is commit output link

                git_link = git_link_all[1]				# At least 2 expected as max>0. Take 1st cmit if many available

                if git_link:
                    if maximum == 1:
                        print(Fore.GREEN + " https://git.kernel.org"+f"{git_link['href']}" + Fore.RESET)
                    else:
                        print(Fore.RED + " https://git.kernel.org"+f"{git_link['href']}" + Fore.RESET)
                        print("\t\t Above commit WON'T be included as a successful search result.",Fore.RED+ "Please CHECK MANUALLY."+Fore.RESET)


                    fp=open("dumped_data.txt", "a+")
                    if maximum == 1:
                        fp.write(" "+"https://git.kernel.org"+f"{git_link['href']}")
                    else:
                        fp.write(" -")
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
    print(response.status_code)



### MAIN

try:
    parser = argparse.ArgumentParser()
    parser.add_argument("MasterCommit_ID", type=str)
    parser.add_argument("Bugz_Product/'-'", type=str)
    args = parser.parse_args()

    print("\n"+"Argument passed: ", sys.argv[1], sys.argv[2])

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)



commit  = sys.argv[1]
product = sys.argv[2]


url4_14 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-4.14.y&qt=grep&q={commit}"
url4_19 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-4.19.y&qt=grep&q={commit}"
url5_4  = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-5.4.y&qt=grep&q={commit}"
url5_10 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-5.10.y&qt=grep&q={commit}"
url5_15 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-5.15.y&qt=grep&q={commit}"
url6_6  = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-6.6.y&qt=grep&q={commit}"
url6_9  = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-6.9.y&qt=grep&q={commit}"


if   product == 'CGX2.4':
    check_stable_fix('CGX2.4', 'v4.14', url4_14)

elif product == 'CGX2.6':
    check_stable_fix('CGX2.6', 'v4.19', url4_19)

elif product == 'CGX3.1':
    check_stable_fix('CGX3.1', 'v5.4 ', url5_4)

elif product == 'CGX4.0':
    check_stable_fix('CGX4.0', 'v5.10', url5_10)

elif product == 'CGX5.0':
    check_stable_fix('CGX5.0', 'v6.6 ', url6_6)

elif product == 'CentOS' or product == 'Rocky':

    print(Fore.RED + "\t Only CGX products are supported. Passed product", product , "is Invalid.\n" + Fore.RESET)
    fp=open("dumped_data.txt", "a+")
    fp.write(" x")
    fp.close()

else:								# Manual Product Checking
    print("Checking manualy, but Bugz Product '" +product+ "'")

#    check_stable_fix('CGX2.4', 'v4.14', url4_14)		# CGX2.4
#    check_stable_fix('CGX2.6', 'v4.19', url4_19)		# CGX2.6
#    check_stable_fix('CGX3.1', 'v5.4 ', url5_4)		# CGX3.1
    check_stable_fix('CGX4.0', 'v5.10', url5_10)		# CGX4.0
#    check_stable_fix('CGX5.0', 'v6.6 ', url6_6)		# CGX5.0

time.sleep(1)

