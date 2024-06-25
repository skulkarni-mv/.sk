#!/usr/bin/python3

from bs4 import BeautifulSoup
import sys
import requests
import argparse

## Usage: ...$ python3 ubuntu-fetch.py CVE-2024-23851

def check_cve_fix(cve):
    # Ubuntu security page
    url = f"https://ubuntu.com/security/{cve}"

    # GET request to the URL
    response = requests.get(url)

    # Check if the request was successful (status code 200)
    if response.status_code == 200:
        #Parse the HTML pagei content
        soup = BeautifulSoup(response.content, 'html.parser')

        #Find the "Fixed by" text
        fixed_by_text = soup.find(string=lambda text: text and "Fixed by" in text)

        # If "Fixed by" text is found then extract the commit hash
        if fixed_by_text:
            fix_link = fixed_by_text.find_next('a')
            print(f"I got : {fix_link}")
            if fix_link:
              print(f"Fix for CVE {cve} found: {fix_link['href']}")
              sys.exit(0)
            else:
              print(f"Fix for CVE {cve} not found")
              sys.exit(1)
        else:
            print(f"No fix found for CVE {cve} on the Ubuntu security page.")
            sys.exit(1)
    else:
        print(f"Failed to retrieve webpage. Status code: {response.status_code}")
        sys.exit(1)


### Main

try:
    parser = argparse.ArgumentParser()
    parser.add_argument("CVE_num", type=str)
    args = parser.parse_args()

    print("Argument passed: ", sys.argv[1]) #print the second argument passed from cmd line; Note it starts from ZERO

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)



cve = sys.argv[1]

check_cve_fix(cve)

