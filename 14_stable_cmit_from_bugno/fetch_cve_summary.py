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

            cve_summ = soup.find('span', {'id': 'short_desc_nonedit_display'})
            if cve_summ:
                cve_summ_str = ' '.join(cve_summ.get_text().split())
                print(cve_summ_str)


            prod_list = soup.find('select', {'id': 'product'})
            if prod_list:
                prod_selected = prod_list.find('option', selected=True)['value']
                print(prod_selected)

            prio_list = soup.find('select', {'id': 'priority'})
            if prio_list:
                prio_selected = prio_list.find('option', selected=True)['value']
                print(prio_selected)

            status_list = soup.find('select', {'id': 'bug_status'})
            if status_list:
                status_selected = status_list.find('option', selected=True)['value']
                print(status_selected)

            assignee = soup.find('input', {'name': 'assigned_to'})
            if assignee:
                assignee_str = assignee.get('value').split('@mvista')[0]
                print(assignee_str)


            reported_extra = soup.find('td', {'id': 'bz_show_bug_column_2'})
            if reported_extra:
                part1_reportedLabel = reported_extra.find_next('td')
                reportedLabel = part1_reportedLabel.find('b')

                if reportedLabel:
                    reportedLabel_str = reportedLabel.get_text(strip=True)

                part2_reportedValue = part1_reportedLabel.find_next('td')
                reportedValue_str = part2_reportedValue.get_text(strip=True).split('by')[0]

#                reported_str = reportedLabel_str +': '+ reportedValue_str
                reported_str = reportedValue_str
                print(reported_str)

            whiteboard = soup.find('input', {'id': 'status_whiteboard'})
            if whiteboard:
                whiteboard_str = whiteboard.get('value')
                print(whiteboard_str)

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

