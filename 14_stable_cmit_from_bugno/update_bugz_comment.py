#!/usr/bin/python3

from bs4 import BeautifulSoup as bs
import sys
import argparse
import requests
import subprocess
import re
import os
from colorama import Fore


mvista_id='skulkarni'

def update_bugz_comment(uname, pword, bug_no, comment):
    bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no
    bugz_post_url = 'http://bugz.mvista.com/process_bug.cgi'

    acc_details = {
        'Bugzilla_login': uname,
        'Bugzilla_password': pword,
    }

    upd_d = {
               'id': bug_no,
#      'assigned_to': uname,				# ASSIGN on uname
            'newcc': uname,
'status_whiteboard':'cgx_sync',
    }


    with requests.session() as s:
        try:
            r = s.post(bugz_login_url, data=acc_details)
            soup = bs(r.text, 'lxml')
            if "Invalid Username Or Password" == soup.title.string:
                print("Invalid Username Or Password")
                print("Exiting..")
                sys.exit(1)

            r = s.get(bugz_login_url)
            soup = bs(r.text, 'lxml')
            upd_d['delta_ts'] = soup.find('input', {'name': 'delta_ts'}).get('value')
            upd_d['token'] = soup.find('input', {'name': 'token'}).get('value')
            upd_d['comment'] = comment

            r = s.post(bugz_post_url, data=upd_d)

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
	parser.add_argument("formatted_comment", type=str)
	args = parser.parse_args()

except:
	e = sys.exc_info()[0]
	print(e)
	sys.exit(1)


bugz_num = sys.argv[1]
comment  = sys.argv[2]


fp = open('.bugzpass.txt', 'r')
bugz_pword=fp.read()
fp.close()

update_bugz_comment(mvista_id, bugz_pword, bugz_num, comment)

print(Fore.GREEN+ "\n\t\t Updated the comment on the bugz: \"http://bugz.mvista.com/show_bug.cgi?id="+bugz_num+"\"" +Fore.RESET)
