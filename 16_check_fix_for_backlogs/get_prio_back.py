#!/usr/bin/python3

from bs4 import BeautifulSoup as bs
import sys
import argparse
import requests
import subprocess
import re
import os
from colorama import Fore


def check_original_priority(uname, pword, bug_no):
	bugz_login_url = 'http://bugz.mvista.com/show_activity.cgi?id='+bug_no

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

			result_soupfind = soup.find_all(string=lambda text: text and "Priority" in text) # If multiple changes, get last change
#			print(result_soupfind)									# Shows List if multiple changes
			if result_soupfind:
				count_prio_changes = result_soupfind.count("\n              Priority\n            ")	# Count Priority changes
#				print(count_prio_changes)

				result_soupfind = result_soupfind[count_prio_changes-1]			# -1 as list will start from 0

				if result_soupfind:
					result_prio_txt = ' '.join(result_soupfind.get_text().split())
#					print(result_prio_txt)								# o/p => "Priority"

					if result_soupfind:
						result_findtd = result_soupfind.find_next('td')
						result_oldprio_txt = ' '.join(result_findtd.get_text().split())
#						print(result_oldprio_txt)						# o/p => "P2"

						result_findtd = result_findtd.find_next('td')
						result_newprio_txt = ' '.join(result_findtd.get_text().split())
#						print(result_newprio_txt)						# o/p => "Backlog"

#						print(result_prio_txt+' was changed from  : '+ result_oldprio_txt+' -> '+result_newprio_txt)
						print("\t\t\t\t\t\t "+Fore.YELLOW+ result_oldprio_txt+' -> '+result_newprio_txt +Fore.RESET)

						if(result_newprio_txt != 'Backlog'):
							print('Invalid Bug number. Bug is not in "Backlog" state.')
							sys.exit(1)
						else:
							fp = open("gitlink_bugzprio.tmp", "w")
							fp.write(result_oldprio_txt);
							fp.close()

							sys.exit(0)
					else:
						sys.exit(1)
				else:
					sys.exit(1)
			else:
				print('Unable to get bug history for "Priority" changes')
				sys.exit(1)

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

#print the second argument passed from cmd line; Note it starts from ZERO
#	print("Argument passed: ", sys.argv[1], "  - http://bugz.mvista.com/show_activity.cgi?id="+sys.argv[1])
	print("\t\t\t\t\t\t "+Fore.CYAN+ "http://bugz.mvista.com/show_activity.cgi?id="+sys.argv[1] +Fore.RESET)

except:
	e = sys.exc_info()[0]
	print(e)
	sys.exit(1)

bugz_num = sys.argv[1]

fp = open('.mvista_userid.txt', 'r')
mvista_id=fp.read()
fp.close()

fp = open('.bugzpass.txt', 'r')
bugz_pword=fp.read()
fp.close()

check_original_priority(mvista_id, bugz_pword, bugz_num)

