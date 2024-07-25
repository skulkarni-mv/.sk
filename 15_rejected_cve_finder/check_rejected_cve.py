#!/usr/bin/python3

from bs4 import BeautifulSoup
import sys
import requests
import argparse
from colorama import Fore
from colorama import Style

from selenium import webdriver
from time import sleep


def check_if_cve_rejected(cve):

    url = f"https://www.cve.org/CVERecord?id={cve}"

    # Initialize a headless browser
    options = webdriver.ChromeOptions()

    options.add_argument('headless')  # Run in headless mode, i.e., without a GUI
    sleep(1)

    driver = webdriver.Chrome(options=options)

    driver.get(url)
    sleep(1)

    driver.implicitly_wait(10)		# Adjust time
    sleep(1)

    html = driver.page_source
    sleep(1)

    soup = BeautifulSoup(html, 'html.parser')

    driver.quit()

    link_text = soup.find_all('span', { 'class': 'has-text-weight-semibold'})
    print(link_text)
#    print("\n\n")
#    print(soup)



### Main
try:
    parser = argparse.ArgumentParser()
    parser.add_argument("CVE_num", type=str)
    args = parser.parse_args()

#    print("Argument passed: ", sys.argv[1], " -  ... "+ Fore.CYAN+ "https://ubuntu.com/security/"+sys.argv[1] +Fore.RESET)
    print("\t\t\t\t\t "+ Fore.CYAN+ "https://www.cve.org/CVERecord?id="+sys.argv[1] +Fore.RESET)

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)

cve = sys.argv[1]
check_if_cve_rejected(cve)

