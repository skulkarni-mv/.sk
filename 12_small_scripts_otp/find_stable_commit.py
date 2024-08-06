#!/usr/bin/python3

import time
import webbrowser
import sys
import argparse

try:
    parser = argparse.ArgumentParser()
    parser.add_argument("MasterCommit_ID", type=str)
    args = parser.parse_args()

    print("Argument passed: ", sys.argv[1])

except:
    e = sys.exc_info()[0]
    print(e)
    sys.exit(1)



commit = sys.argv[1]


webbrowser.open('https://www.google.com/')
time.sleep(0.5)

url4_14 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-4.14.y&qt=grep&q={commit}"
url4_19 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-4.19.y&qt=grep&q={commit}"
url5_4  = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-5.4.y&qt=grep&q={commit}"
url5_10 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-5.10.y&qt=grep&q={commit}"
url5_15 = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-5.15.y&qt=grep&q={commit}"
url6_6  = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-6.6.y&qt=grep&q={commit}"
url6_9  = f"https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/?h=linux-6.9.y&qt=grep&q={commit}"

webbrowser.open(url4_14)
time.sleep(0.1)

webbrowser.open(url4_19)
time.sleep(0.1)

webbrowser.open(url5_4)
time.sleep(0.1)

webbrowser.open(url5_10)
time.sleep(0.1)

webbrowser.open(url5_15)
time.sleep(0.1)

webbrowser.open(url6_6)
time.sleep(0.1)

webbrowser.open(url6_9)
time.sleep(0.1)

