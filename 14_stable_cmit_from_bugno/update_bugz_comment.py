#!/usr/bin/python3

from bs4 import BeautifulSoup as bs
import sys
import argparse
import requests
import subprocess
import re
import os
from colorama import Fore
import time


mvista_id='skulkarni'

flags_bugz=""
flag_assigne_to_change=0
flag_status_change_resolved=0
flag_status_change_sync_req=0


def update_bugz_comment(uname, pword, bug_no, comment):
    bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no
    bugz_post_url = 'http://bugz.mvista.com/process_bug.cgi'

    acc_details = {
        'Bugzilla_login': uname,
        'Bugzilla_password': pword,
    }

    upd_d = {
      'id': bug_no,
    }

    if flag_assigne_to_change == 1:
        upd_d['assigned_to'] = uname			# ASSIGN on uname
        upd_d['newcc']       = uname

    if flag_status_change_resolved == 1:
        upd_d['status_whiteboard'] = ''
        upd_d['bug_status']        = 'RESOLVED'
        upd_d['resolution']        = 'FIXED'

    if flag_status_change_sync_req == 1:
        upd_d['status_whiteboard'] = 'cgx_sync'
        upd_d['bug_status']        = 'SYNC_REQ'


    with requests.session() as s:
        try:
            r = s.post(bugz_login_url, data=acc_details)
            soup = bs(r.text, 'lxml')
            if "Invalid Username Or Password" == soup.title.string:
                print(Fore.RED, "Invalid Username Or Password", "\n", "Exiting..", Fore.RESET)
                sys.exit(1)

            r = s.get(bugz_login_url)
            soup = bs(r.text, 'lxml')
            upd_d['delta_ts'] = soup.find('input', {'name': 'delta_ts'}).get('value')
            upd_d['token'] = soup.find('input', {'name': 'token'}).get('value')
            upd_d['comment'] = comment

            r = s.post(bugz_post_url, data=upd_d)

        except requests.exceptions.Timeout:
            print(Fore.RED, "Connection timed out", "\n", "Exiting..", Fore.RESET)
            sys.exit(1)
        except requests.exceptions.RequestException as e:
            print(Fore.RED, e, "\n", "Exiting..", Fore.RESET)
            sys.exit(1)


def update_bugz_status_assigned(uname, pword, bug_no):
    bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no
    bugz_post_url = 'http://bugz.mvista.com/process_bug.cgi'

    acc_details = { 'Bugzilla_login': uname, 'Bugzilla_password': pword, }

    upd_d = {  'id': bug_no, 'bug_status': 'ASSIGNED', }

    with requests.session() as s:
        try:
            r = s.post(bugz_login_url, data=acc_details)
            soup = bs(r.text, 'lxml')
            if "Invalid Username Or Password" == soup.title.string:
                print(Fore.RED, "Invalid Username Or Password", "\n", "Exiting..", Fore.RESET)
                sys.exit(1)

            r = s.get(bugz_login_url)
            soup = bs(r.text, 'lxml')
            upd_d['delta_ts'] = soup.find('input', {'name': 'delta_ts'}).get('value')
            upd_d['token'] = soup.find('input', {'name': 'token'}).get('value')
##            upd_d['comment'] = 'changing status to ASSIGNED'

            r = s.post(bugz_post_url, data=upd_d)

        except requests.exceptions.Timeout:
            print(Fore.RED, "Connection timed out", "\n", "Exiting..", Fore.RESET)
            sys.exit(1)
        except requests.exceptions.RequestException as e:
            print(Fore.RED, e, "\n", "Exiting..", Fore.RESET)
            sys.exit(1)


### Main

try:
	parser = argparse.ArgumentParser()
	parser.add_argument("bugz_num", type=str)
	parser.add_argument("formatted_comment", type=str)
	parser.add_argument('--optional', type=str, help='Flags for state changes')
	args = parser.parse_args()

except:
	e = sys.exc_info()[0]
	print(Fore.RED, e, Fore.RESET)
	sys.exit(1)


bugz_num  = sys.argv[1]
comment   = sys.argv[2]

if args.optional:			## if --optional is provided
	flags_bugz = args.optional

if flags_bugz:				## if flags_bugz is not empty
					## 3 options: "_assignee=uname_" / "_status=Reso__whiteb=clear_" / "_status=Sync_Req__whiteb=cgx_sync_"

	if flags_bugz.find("_assignee=uname_") != -1:				## FOUND
		flag_assigne_to_change=1

	if   flags_bugz.find("_status=Reso__whiteb=clear_") != -1:		## FOUND
		flag_status_change_resolved=1
	elif flags_bugz.find("_status=Sync_Req__whiteb=cgx_sync_") != -1:	## FOUND
		flag_status_change_sync_req=1


fp = open('.bugzpass.txt', 'r')
bugz_pword=fp.read()
fp.close()


if flag_status_change_sync_req == 1:						# No need to check current status, as filtered data inputed
	update_bugz_status_assigned(mvista_id, bugz_pword, bugz_num)		# Change to Assigned mode directly -> Sync_Req in next step
	time.sleep(1)

update_bugz_comment(mvista_id, bugz_pword, bugz_num, comment)

print(Fore.GREEN+ "\n\t\t Updated the comment on the bugz: \"http://bugz.mvista.com/show_bug.cgi?id="+bugz_num+"\"" +Fore.RESET)

