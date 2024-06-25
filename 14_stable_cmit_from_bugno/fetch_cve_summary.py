#!/usr/bin/python3

from bs4 import BeautifulSoup as bs
import sys
import argparse
import requests
import subprocess
import re
import os

mvista_id='skulkarni'

def fetch_cve_summary_from_bugz_number(uname, pword, bug_no):
    bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no
    bugz_post_url = 'http://bugz.mvista.com/process_bug.cgi'

    acc_details = {
        'Bugzilla_login': uname,
        'Bugzilla_password': pword,
    }

    with requests.session() as s:
        try:
            r = s.post(bugz_login_url, data=acc_details)
            soup = bs(r.text, 'lxml')
            if "Invalid Username Or Password" == soup.title.string:
                print("Invalid Username Or Password")
                print("Exiting..")
                sys.exit(1)

            result = soup.find('span', {'id': 'short_desc_nonedit_display'})
            if result:
                result_str = ' '.join(result.get_text().split())
                print(result_str)

            return

        except requests.exceptions.Timeout:
            print("Connection timed out")
            print("Exiting..")
            sys.exit(1)
        except requests.exceptions.RequestException as e:
            print(e)
            print("Exiting..")
            sys.exit(1)


### Main

try:
	parser = argparse.ArgumentParser()
	parser.add_argument("bugz_num", type=str)
	args = parser.parse_args()

except:
	e = sys.exc_info()[0]
	print(e)
	sys.exit(1)


bugz_num = sys.argv[1]

fp = open('.bugzpass.txt', 'r')
bugz_pword=fp.read()
fp.close()


fetch_cve_summary_from_bugz_number(mvista_id, bugz_pword, bugz_num)

