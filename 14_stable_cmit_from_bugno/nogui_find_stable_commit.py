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

        print(kern_vers, '-> Found total :', len(soup.find_all('span', {'class': 'insertions'} )) , 'Latest commit out of them:')

        link_text = soup.find('span', { 'class': 'age-days'})

        if link_text:
            git_link = link_text.find_next('a')
            if git_link:
              print(Fore.GREEN + " https://git.kernel.org/"+f"{git_link['href']}" + Fore.RESET)

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

    print("Argument passed: ", sys.argv[1], '\n')

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

#webbrowser.open(f"https://github.com/gregkh/linux/commit/{commit}")
#time.sleep(0.5)

#webbrowser.open(url4_14)
check_stable_fix('4.14', url4_14)
time.sleep(0.1)

#webbrowser.open(url4_19)
check_stable_fix('4.19', url4_19)
time.sleep(0.1)

#webbrowser.open(url5_4)
check_stable_fix('5.4 ', url5_4)
time.sleep(0.1)

#webbrowser.open(url5_10)
check_stable_fix('5.10', url5_10)
time.sleep(0.1)

#webbrowser.open(url5_15)
time.sleep(0.1)

#webbrowser.open(url6_6)
time.sleep(0.1)

#webbrowser.open(url6_9)
time.sleep(0.1)





