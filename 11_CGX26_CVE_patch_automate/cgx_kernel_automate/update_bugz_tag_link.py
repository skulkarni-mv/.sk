#!/usr/bin/python3

from bs4 import BeautifulSoup as bs
import sys
import argparse
import requests
import subprocess
import re
import os

mvista_id=''

def identify_user():

	global mvista_id									## use the global version
	try:
		repo_details = subprocess.check_output('git config --get remote.origin.url', shell=True, universal_newlines=True).splitlines()[0]
	except subprocess.CalledProcessError as e:
		print("Please run from a valid git repository")
		print("Exiting...")
		sys.exit(1)

	if not (re.match(r'.*.+@gitcgx.mvista.com+.*.', repo_details)):
		print("Unable to retrieve UserId from the cloned repo. Have you cloned from username@gitcgx.mvista.com ?")
		print("Exiting...")
		sys.exit(1)

	mvista_id = repo_details.split('@')[0]


def update_bugz_with_tag_link(uname, pword, bug_no):
    bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no
    bugz_post_url = 'http://bugz.mvista.com/process_bug.cgi'

    acc_details = {
        'Bugzilla_login': uname,
        'Bugzilla_password': pword,
    }

    upd_d = {
           'id': bug_no,
        'newcc': uname,
    }

    fp = open('cgx_kernel_automate/generated_details.txt', 'r')
    cmnt_tag_link=fp.read()
    fp.close()

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
            upd_d['comment'] = cmnt_tag_link

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
	args = parser.parse_args()

except:
	e = sys.exc_info()[0]
	print(e)
	sys.exit(1)


bugz_num = sys.argv[1]


fp = open('cgx_kernel_automate/.bugzpass.txt', 'r')
bugz_pword=fp.read()
fp.close()

identify_user()
update_bugz_with_tag_link(mvista_id, bugz_pword, bugz_num)

print("\t\t Updated the comment on the bugz: \"http://bugz.mvista.com/show_bug.cgi?id="+bugz_num+"\"")
