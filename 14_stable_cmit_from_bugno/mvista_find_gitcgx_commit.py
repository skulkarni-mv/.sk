#!/usr/bin/python3

import time
import webbrowser
import sys
import argparse

from bs4 import BeautifulSoup
import requests
from colorama import Fore

import warnings
from requests.packages.urllib3.exceptions import InsecureRequestWarning


def check_fix_mvista_gitcgx(cgx_vers_num, mvl_branch, mast_cmit):

    url="https://gitcgx.mvista.com/cgit/CGX"+cgx_vers_num+"/kernel/linux-mvista-"+cgx_vers_num+".git/log/?h="+mvl_branch+"&qt=grep&q="+mast_cmit

    # Suppress only the InsecureRequestWarning
    warnings.simplefilter('ignore', InsecureRequestWarning)

    response = requests.get(url, verify=False)

    if response.status_code == 200:

        soup = BeautifulSoup(response.content, 'html.parser')

        count_insertions = len( soup.find_all('span', {'class': 'insertions'} ) )	# insertions in commit
        count_deletions  = len( soup.find_all('span', {'class': 'deletions'} ) )	# deletions in commit


        maximum = max(count_insertions, count_deletions)	# Possibly some commits might have only instns/deltns. Get max(0,1) OR max(1,0)

        if maximum > 1:									# Multiple(in case of Revert)
            print(Fore.YELLOW+ mvl_branch ,'->',Fore.RED+' Found total :', maximum,'. Latest/REVERT out of them:', Fore.RESET)
        elif maximum == 1:								# Single commit
            print(Fore.YELLOW+ mvl_branch , Fore.RESET, '-> Found total :'+Fore.GREEN, maximum , Fore.RESET)
        else:										# No git commit available for the requested MasterCmitID
            print(Fore.YELLOW+ mvl_branch , Fore.RESET, '-> Found total :'  +Fore.RED, maximum , Fore.RESET)


        if maximum > 0:

            link_text = soup.find('div', { 'class': 'content'})		# Will have 2 or more. 1st-> request data  2nd+ -> required cmit link/s


            if link_text:
                git_link_all = link_text.find_all('a')			# At least 2, 1st search querry, 2nd is commit output link

                git_link = git_link_all[1]				# At least 2 expected as max>0. Take 1st cmit if many available

                if git_link:
                    if maximum == 1:
                        print(Fore.GREEN + " https://gitcgx.mvista.com"+f"{git_link['href']}" + Fore.RESET)
                    else:
                        print(Fore.RED + " https://gitcgx.mvista.com"+f"{git_link['href']}" + Fore.RESET)
                        print("\t\t Above commit WON'T be included as a successful search result.",Fore.RED+ "Please CHECK MANUALLY."+Fore.RESET)


                    fp=open("dumped_data.txt", "a+")
                    if maximum == 1:
                        fp.write("       "+"https://gitcgx.mvista.com"+f"{git_link['href']}"+"\n")
                    else:
                        fp.write("       NA_gitcgx-1"+"\n")
                    fp.close()

                else:
                  print(Fore.RED + f"\t Fix not found" + Fore.RESET)

                  fp=open("dumped_data.txt", "a+")
                  fp.write("       NA_gitcgx-2"+"\n")
                  fp.close()
        else:
            print(Fore.RED + f"\t No fix found." + Fore.RESET)
            fp=open("dumped_data.txt", "a+")
            fp.write("       NA_gitcgx-3"+"\n")
            fp.close()

    else:
        print(f"\t Failed to retrieve webpage. Status code: {response.status_code}")
        fp=open("dumped_data.txt", "a+")
        fp.write("       NA_gitcgx-Failed_to_retrieve_webpage"+"\n")
        fp.close()



### MAIN

try:
    parser = argparse.ArgumentParser()
    parser.add_argument("MasterCommit_ID", type=str)
    parser.add_argument("Bugz_Product/'-'", type=str)
    args = parser.parse_args()

    print("")

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)



commit = sys.argv[1]
product = sys.argv[2]

if   product == 'CGX2.4':
    check_fix_mvista_gitcgx('2.4', 'mvl-4.14/msd.cgx', commit)

elif product == 'CGX2.6':
    check_fix_mvista_gitcgx('2.6', 'mvl-4.19/msd.cgx', commit)

elif product == 'CGX3.1':
    check_fix_mvista_gitcgx('3.1', 'mvl-5.4/msd.cgx', commit)

elif product == 'CGX4.0':
    check_fix_mvista_gitcgx('4.0', 'mvl-5.10/msd.cgx', commit)

elif product == 'CGX5.0':
    check_fix_mvista_gitcgx('5.0', 'mvl-6.6/msd.cgx', commit)

elif product == 'CentOS' or product == 'Rocky':				# Unsupported Products

    print(Fore.RED + "\t Only CGX products are supported. Passed product", product , "is Invalid.\n" + Fore.RESET)
#    fp=open("dumped_data.txt", "a+");fp.write("       NA_gitcgx"+"\n");fp.close()	# NO NEED, as other products gets filtered in 'code1.*c

else:									# Manual Product checking

    print("Checking manualy, but Bugz Product '" +product+"'")

#    check_fix_mvista_gitcgx('2.4', 'mvl-4.14/msd.cgx', commit)		# CGX2.4
#    check_fix_mvista_gitcgx('2.6', 'mvl-4.19/msd.cgx', commit)		# CGX2.6
#    check_fix_mvista_gitcgx('3.1', 'mvl-5.4/msd.cgx', commit)		# CGX3.1
    check_fix_mvista_gitcgx('4.0', 'mvl-5.10/msd.cgx', commit)		# CGX4.0
#    check_fix_mvista_gitcgx('5.0', 'mvl-6.6/msd.cgx', commit)		# CGX5.0

