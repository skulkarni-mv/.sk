#!/usr/bin/python3

import webbrowser
import argparse
import time


print("\n Usage: ./2_weblinks_bug_csv.py <r8 / r9>\n")

parser = argparse.ArgumentParser(description="Open Webbrowser kernel CVE list for Rocky product")
parser.add_argument('branch', type=str, choices=['r8', 'r9'], help="Branch for the Rocky Kernel CVEs (r8 or r9)")
args = parser.parse_args()

print(" This code will OPEN(NO download) the bugz query page to show open kernel CVEs for required Rocky branch 'r8' OR 'r9'\n\n")
time.sleep(2)

if args.branch == 'r8':
    url = "http://bugz.mvista.com/buglist.cgi?keywords=Security;keywords_type=allwords;version=8.4%20Standard;version=8.5%20Standard;version=8.6%20Standard;version=8.7%20Standard;version=8.8%20Standard;version=8.9%20Standard;version=8.10%20Standard;bug_status=NEW;bug_status=ASSIGNED;bug_status=IN_REVIEW;bug_status=REOPENED;component=Kernel;product=Rocky"


elif args.branch == 'r9':
    url = "http://bugz.mvista.com/buglist.cgi?keywords=Security;keywords_type=allwords;version=9.1%20Standard;version=9.2%20Standard;version=9.3%20Standard;version=9.4%20Standard;bug_status=NEW;bug_status=ASSIGNED;bug_status=IN_REVIEW;bug_status=REOPENED;component=Kernel;product=Rocky"

# Open the web browser with the constructed URL
webbrowser.open(url)

print(" TODO: Opened the Bugz Page, Download the CSV file and Move the file in this Directory.")
print(" Run './3_CVEs_from_CSV_bugz.py' with Argument: ~/Downloads/bugs-2024- <Tab> \n")

