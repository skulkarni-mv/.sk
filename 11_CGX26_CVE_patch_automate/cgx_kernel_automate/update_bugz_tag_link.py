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
##		print(e)				## outputs:"Command 'git config --get remote.origin.url' returned non-zero exit status 1."
		print("Exiting...")
		sys.exit(1)

#	print("repo details:", repo_details)

	if not (re.match(r'.*.+@gitcgx.mvista.com+.*.', repo_details)):
		print("Unable to retrieve UserId from the cloned repo. Have you cloned from username@gitcgx.mvista.com ?")
		print("Exiting...")
		sys.exit(1)

	mvista_id = repo_details.split('@')[0]
#	print("mvista id is:", mvista_id)


def update_bugz_with_tag_link(uname, pword, bug_no):
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

            fp = open('cgx_kernel_automate/generated_details.txt', 'r')
            cmnt_tag_link=fp.read()
            fp.close()

            print("Python reads file as:")
            print(cmnt_tag_link)

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
##	print(sys.argv[1])			#print the second argument i.e. 'bugz_num' passed from cmd line; Note it starts from ZERO

except:
	e = sys.exc_info()[0]
	print(e)
	sys.exit(1)


bugz_num = sys.argv[1]


fp = open('cgx_kernel_automate/bugzpass.txt', 'r')
bugz_pword=fp.read()
fp.close()

identify_user()
update_bugz_with_tag_link(mvista_id, bugz_pword, bugz_num)

