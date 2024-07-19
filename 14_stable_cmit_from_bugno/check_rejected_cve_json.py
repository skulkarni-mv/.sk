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
    url = f"https://cveawg.mitre.org/api/cve-id/{cve}"

    # GET request to the URL
    response = requests.get(url)

    if response.status_code == 200:
        soup = BeautifulSoup(response.content, 'html.parser')
#        print(soup)

        rejected_text  = soup.find(string=lambda text: text and "REJECTED" in text)
        published_text = soup.find(string=lambda text: text and "PUBLISHED" in text)


        if published_text:
#            print(Fore.GREEN , soup , Fore.RESET, '\n')
            sys.exit(0)

        elif rejected_text:
            print("\t\t\t\t\t "+ Fore.CYAN+ "https://cveawg.mitre.org/api/cve-id/"+sys.argv[1] +Fore.RESET)
            print(Fore.RED , soup , Fore.RESET, '\n')
            sys.exit(1)

        else:
            print('Unknown Response.', soup , 'Report Error', '\n')

        print('\n')
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
#    print("\t\t\t\t\t "+ Fore.CYAN+ "https://cveawg.mitre.org/api/cve-id/"+sys.argv[1] +Fore.RESET)

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)

cve = sys.argv[1]
check_cve_fix(cve)

