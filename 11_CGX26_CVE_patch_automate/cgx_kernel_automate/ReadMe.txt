
WorkDirectory is the kernel clone directory i.e. it can be 'linux-mvista-2.4' , 'linux-mvista-2.6', etc.

---------------------------------------------------------------------------------------------------------------------------------------------

Directory Structure:

       WorkDirectory
          /    \
gregkh-linux  linux-mvista-2.6
	/	 \
       /          \
     linux       cgx_kernel_automate
                   |
                   |
                 automation files


user@group:WorkDirectory$ git clone -b mvl-4.19/common skulkarni@gitcgx.mvista.com:/mvista/git/cgx/CGX2.6/kernel/linux-mvista-2.6.git
user@group:WorkDirectory$ ls
			$ linux-mvista-2.6

user@group:WorkDirectory$ mkdir gregkh-linux					// This can be done while running the script, not mandatory
user@group:WorkDirectory$ cd gregkh-linux/					// but it takes time to clone this repo
user@group:gregkh-linux $ git clone https://github.com/gregkh/linux.git		// around 2 hours, with good internet connection
user@group:gregkh-linux $ ls
			$ linux


Directory 'cgx_kernel_automate' needs to be present in 'linux-mvista-2.6' dir.
Directory 'cgx_kernel_automate' contains following automation files:

	automate_run
	bugzpass.txt			-> Add your bugz password in this file
	code_for_automation.c
	fetch_cve_summary.py
	generated_details.txt
	git-commit-mvauto		-> need to move to /usr/local/bin/
	make.sh
	ReadMe.txt
	send_pull_req_automate.py


user@group:WorkDirectory$ gcc code_for_automation.c -o automate_run	OR use	$ ./make.sh


Usage: ./cgx_kernel_automate/automate_run <bugz_num> <stable_commit_id>

user@group:WorkDirectory$ ./cgx_kernel_automate/automate_run		OR 	$ ./cgx_kernel_automate/automate_run bugz_num stable_commit_id

---------------------------------------------------------------------------------------------------------------------------------------------

Dependencies:

user@group:WorkDirectory/cgx_kernel_automate $ sudo mv git-commit-mvauto /usr/local/bin		// move the file to /usr/local/bin
user@group:WorkDirectory/cgx_kernel_automate $ sudo pip install BeautifulSoup4
user@group:WorkDirectory/cgx_kernel_automate $ sudo pip install lxml


