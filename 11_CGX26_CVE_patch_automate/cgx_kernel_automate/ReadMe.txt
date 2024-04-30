
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
user@group:gregkh-linux $ git clone https://github.com/gregkh/linux.git		// But as this is not mandatory, it can be done runtime while
user@group:gregkh-linux $ ls							// you are running the 'automate_run' program
			$ linux

---------------------------------------------------------------------------------------------------------------------------------------------

Directory 'cgx_kernel_automate' needs to be present in 'linux-mvista-2.6' dir.
Directory 'cgx_kernel_automate' contains following automation files:

	automate_run
	.bugzpass.txt			-> Add your bugz password in this file
	code_for_automation.c
	fetch_cve_summary.py
	generated_details.txt
	git-commit-diff.patch		-> will be moved to /usr/local/bin/ by 'make.sh' script
	make.sh
	ReadMe.txt
	send_pull_req_automate.py
	update_bugz_tag_link.py

---------------------------------------------------------------------------------------------------------------------------------------------

Dependencies:	file '/usr/local/bin/git-commit-mvauto'

user@group:WorkDirectory/cgx_kernel_automate$ sudo cp /usr/local/bin/git-commit-mv /usr/local/bin/git-commit-mvauto
user@group:WorkDirectory/cgx_kernel_automate$ sudo mv git-commit-diff.patch /usr/local/bin/git-commit-diff.patch
user@group:WorkDirectory/cgx_kernel_automate$ cd /usr/local/bin/
user@group:/usr/local/bin/                  $ sudo patch -p1 < git-commit-diff.patch
user@group:/usr/local/bin/                  $ sudo rm git-commit-diff.patch
user@group:/usr/local/bin/                  $ cd -


Dependent Packages: Install following Python packages

user@group:WorkDirectory/cgx_kernel_automate$ sudo pip install BeautifulSoup4
user@group:WorkDirectory/cgx_kernel_automate$ sudo pip install lxml


*** NOTE: These Dependencies will be address once you run './make.sh'

---------------------------------------------------------------------------------------------------------------------------------------------

Compilation and Usage:

user@group:WorkDirectory/cgx_kernel_automate$ gcc code_for_automation.c -o automate_run		OR
user@group:WorkDirectory/cgx_kernel_automate$ ./make.sh

		Usage: ./cgx_kernel_automate/automate_run <bugz_num> <stable_commit_id>

