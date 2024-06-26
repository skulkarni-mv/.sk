#!/usr/bin/python3

import time
import webbrowser

#  1
webbrowser.open('http://bugz.mvista.com/show_bug.cgi?id=149917')
time.sleep(0.5)
webbrowser.open('file://gitcgx.mvista.com/cgit/CGX5.0/kernel/linux-mvista-5.0.git/log/?h=mvl-6.6/msd.cgx&qt=grep&q=be3f3042391d061cfca2bd22630e0d101acea5fc')
time.sleep(0.5)

