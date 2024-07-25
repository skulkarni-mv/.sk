#!/usr/bin/python3

from bs4 import BeautifulSoup as bs
import sys
import argparse
import requests
import subprocess
import re
import os


def update_bugz_priority_details(uname, pword, bug_no, cve_id, git_link, old_prio):
	bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no
	bugz_post_url = 'http://bugz.mvista.com/process_bug.cgi'

	acc_details = {
	'Bugzilla_login': uname,
	'Bugzilla_password': pword,
	}

	upd_d = {
	   'id': bug_no,
     'priority': old_prio,
	}

	comment="From AutoScript\n\n"+"At least one fix was found at https://ubuntu.com/security/"+cve_id+":\n"+git_link+"\n\n"
	comment=comment+"Changing the priority back from Backlog to "+old_prio

#	print('\n\n'+comment+'\n\n')

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
			sys.exit(0)

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
	parser.add_argument("CVE_num", type=str)
	parser.add_argument("git_link", type=str)
	parser.add_argument("old_prio", type=str)
	args = parser.parse_args()

except:
	e = sys.exc_info()[0]
	print(e)
	sys.exit(1)


bug_no   = sys.argv[1]
cve_id   = sys.argv[2]
git_link = sys.argv[3]
old_prio = sys.argv[4]


fp = open('.mvista_userid.txt', 'r')
mvista_id=fp.read()
fp.close()

fp = open('.bugzpass.txt', 'r')
bugz_pword=fp.read()
fp.close()

update_bugz_priority_details(mvista_id, bugz_pword, bug_no, cve_id, git_link, old_prio)

