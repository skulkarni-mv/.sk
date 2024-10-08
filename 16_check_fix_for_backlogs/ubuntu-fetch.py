#!/usr/bin/python3

from bs4 import BeautifulSoup
import sys
import requests
import argparse
from colorama import Fore
from colorama import Style

## Usage: ...$ python3 ubuntu-fetch.py CVE-2023-52429

def check_cve_fix(cve):
    # Ubuntu security page
    url = f"https://ubuntu.com/security/{cve}"

    # GET request to the URL
    response = requests.get(url)

    if response.status_code == 200:
        soup = BeautifulSoup(response.content, 'html.parser')

        fixed_by_text = soup.find(string=lambda text: text and "fixed by" in text)

        if fixed_by_text:
            fix_link = fixed_by_text.find_next('a')
            if fix_link:
              print(Fore.GREEN + f"\t\t\t\t\t {fix_link['href']}" + Fore.RESET)

              fp = open("gitlink_bugzprio.tmp", "w")
              fp.write(f"{fix_link['href']}");
              fp.close()

              sys.exit(0)
            else:
              print(Fore.RED + f"\t Fix for CVE {cve} not found" + Fore.RESET)
              sys.exit(1)
        else:
            print(Fore.RED + f"\t No fix found for CVE {cve} on the Ubuntu security page." + Fore.RESET)
            sys.exit(1)
    else:
        print(f"\t Failed to retrieve webpage. Status code: {response.status_code}")
        sys.exit(1)


### Main
try:
    parser = argparse.ArgumentParser()
    parser.add_argument("CVE_num", type=str)
    args = parser.parse_args()

#    print("Argument passed: ", sys.argv[1], " -  ... "+ Fore.CYAN+ "https://ubuntu.com/security/"+sys.argv[1] +Fore.RESET)
    print("\t\t\t\t\t "+ Fore.CYAN+ "https://ubuntu.com/security/"+sys.argv[1] +Fore.RESET)

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)

cve = sys.argv[1]
check_cve_fix(cve)

