#!/usr/bin/python3

from bs4 import BeautifulSoup as bs
import sys
import argparse
import requests
import getpass


# Function to check current bugz status (either of Resolved/Closed) --------------------------------------------------#

def check_bugz_current_status(uname, pword, bug_no, flag_resolved0_closed1):
    bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no

    acc_details = {
        'Bugzilla_login': uname,
        'Bugzilla_password': pword,
    }

    with requests.session() as s:
        try:
            r = s.post(bugz_login_url, data=acc_details)
            soup = bs(r.text, 'lxml')
            if "Invalid Username Or Password" == soup.title.string:
                print("Invalid Username Or Password", "\n", "Exiting..")
                sys.exit(1)

            result = soup.find('div', {'id': 'status'}).find('select', {'name': 'bug_status'})
            if result:
                result_str = ' '.join(result.get_text().split())
                result_cur = result_str.split()[0]
                result_fut = result_str.split(' ', 1)[1]

                if flag_resolved0_closed1 == 0:
                    print("\n\t Current status of the bug :", result_cur)
                    print(  "\t Possible changes in status:", result_fut)

                    if result_cur=='RESOLVED' and (result_fut.find('CLOSED') != -1):
                        return 1
                    else:
                        print("\n Bugz status is not in 'RESOLVED' state. Unable to close the bug. Exiting... \n")
                        sys.exit(1)

                elif flag_resolved0_closed1 == 1:
                    if result_cur=='CLOSED':
                        return 1
                    else:
                        print("\n Bugz status was not updated as 'CLOSED'. Error occurred. Exiting... \n")
                        sys.exit(1)

                else:
                    print("\n Unexpected input flag to the function 'check_bugz_current_status'. Error occurred. Exiting... \n")
                    sys.exit(1)


        except requests.exceptions.Timeout:
            print("Connection timed out", "\n", "Exiting..")
            sys.exit(1)
        except requests.exceptions.RequestException as e:
            print(e, "\n", "Exiting..")
            sys.exit(1)

# Function to Update bugz status as 'Closed' -------------------------------------------------------------------------#

def update_bugz_status_closed(uname, pword, bug_no):
    bugz_login_url = 'http://bugz.mvista.com/show_bug.cgi?id='+bug_no
    bugz_post_url = 'http://bugz.mvista.com/process_bug.cgi'

    acc_details = {
        'Bugzilla_login': uname,
        'Bugzilla_password': pword,
    }

    upd_d = {
              'id': bug_no,
      'bug_status': 'CLOSED',
    }

    with requests.session() as s:
        try:
            r = s.post(bugz_login_url, data=acc_details)
            soup = bs(r.text, 'lxml')
            if "Invalid Username Or Password" == soup.title.string:
                print("Invalid Username Or Password", "\n", "Exiting..")
                sys.exit(1)

            r = s.get(bugz_login_url)
            soup = bs(r.text, 'lxml')
            upd_d['delta_ts'] = soup.find('input', {'name': 'delta_ts'}).get('value')
            upd_d['token']    = soup.find('input', {'name': 'token'}).get('value')
            upd_d['comment']  = 'From AutoScript: closing'

            r = s.post(bugz_post_url, data=upd_d)

        except requests.exceptions.Timeout:
            print("Connection timed out", "\n", "Exiting..")
            sys.exit(1)
        except requests.exceptions.RequestException as e:
            print(e, "\n", "Exiting..")
            sys.exit(1)

# Main ---------------------------------------------------------------------------------------------------------------#

try:
	parser = argparse.ArgumentParser()
	parser.add_argument("bugz_num", type=str, help="The Bugzilla bug number to process")
	args = parser.parse_args()

except:
	e = sys.exc_info()[0]
	print(e)
	sys.exit(1)

print("")

bugz_num = sys.argv[1]

if not bugz_num.isdigit() or len(bugz_num) not in [5, 6]:
    raise argparse.ArgumentTypeError("Error: Invalid Bugz number. Exiting... \n")
    sys.exit(1)


mvista_id  = input("Please enter your Bugzilla-ID : ")			# Comment this line if you hardcode your id
#mvista_id = '<hardcode_your_id>'

if mvista_id.strip() == "" or not mvista_id.isalpha():			# Invalid input check
    print("Error: Invalid 'Bugzilla-ID' input. Exiting... \n")
    sys.exit(1)

bugz_pword = getpass.getpass("Please enter bugzilla password for '%s': " % (mvista_id))	# Comment this line if you hardcode your password
#bugz_pword = '<hardcode_your_password>'

print("")
define_flag_check_resolved=0						# flag to check if the bug status is 'Resolved'
define_flag_check_closed = 1						# flag to check if the bug status is 'Closed'


## Check if the current status of the bugz is 'Resolved' or not. Script will only work for 'Resolved' bugs
proceed_okay = check_bugz_current_status(mvista_id, bugz_pword, bugz_num, define_flag_check_resolved)

if proceed_okay == 1:

    ## If the bugz status is 'Resolved', 'proceed_okay' eq 1
    print("\n Updating the bug status as 'CLOSED'...")
    update_bugz_status_closed(mvista_id, bugz_pword, bugz_num)

    ## check again if the status was updated as 'Closed' or not
    check_bugz_current_status(mvista_id, bugz_pword, bugz_num, define_flag_check_closed)
    print(" Successfully updated bugz status: http://bugz.mvista.com/show_bug.cgi?id="+bugz_num, "\n")

