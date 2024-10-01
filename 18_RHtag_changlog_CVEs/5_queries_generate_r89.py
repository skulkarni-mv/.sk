#!/usr/bin/python3

import webbrowser
import argparse
import time

print("\n Usage: ./5_queries_generate_r89.py <Common CVEs extracted for branch r8 / r9>\n")

parser = argparse.ArgumentParser(description="Create a bugz / Redhat query for the Rocky r8 / r9 branch Common CVEs")
parser.add_argument('branch', type=str, choices=['r8', 'r9'], help="Branch for the common CVEs (r8 or r9)")
args = parser.parse_args()

print(f" This code will show Redhat & Bugz querries links for file 'file_common_cves' for Rocky branch '{args.branch}' \n\n")
time.sleep(2)

# Read CVE data from file.txt
with open('file_common_cves', 'r') as file:
    data = file.read()

# Extract CVEs from the file content
cve_list = data.split()


if args.branch == 'r8':

    print("------------------------------------------------------------------------------------------------------------------------------\n")
    for cve in cve_list:
        print("  ", cve, "\t\t" , f"https://access.redhat.com/security/cve/{cve}")
        print(f"http://bugz.mvista.com/buglist.cgi?component=Kernel&keywords=Security&keywords_type=allwords&product=Rocky&short_desc={cve}&short_desc_type=allwordssubstr&version=8.4%20Standard&version=8.5%20Standard&version=8.6%20Standard&version=8.7%20Standard&version=8.8%20Standard&version=8.9%20Standard&version=8.10%20Standard&order=version")
        print("\n------------------------------------------------------------------------------------------------------------------------------\n")

elif args.branch == 'r9':

    print("------------------------------------------------------------------------------------------------------------------------------\n")
    for cve in cve_list:
        print("  ", cve, "\t\t" , f"https://access.redhat.com/security/cve/{cve}")
        print(f"http://bugz.mvista.com/buglist.cgi?component=Kernel&keywords=Security&keywords_type=allwords&product=Rocky&short_desc={cve}&short_desc_type=allwordssubstr&version=9.1%20Standard&version=9.2%20Standard&version=9.3%20Standard&version=9.4%20Standard&order=version")
        print("\n------------------------------------------------------------------------------------------------------------------------------\n")

