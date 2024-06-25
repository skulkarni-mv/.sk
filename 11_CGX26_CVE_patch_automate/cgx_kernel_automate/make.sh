gcc code_for_automation.c -o automate_run

if [ -f "git-commit-diff.patch" ];
then

	chmod u+x fetch_cve_summary.py send_pull_req_automate.py update_bugz_tag_link.py;

	sudo cp /usr/local/bin/git-commit-mv /usr/local/bin/git-commit-mvauto;
	sudo mv git-commit-diff.patch /usr/local/bin/git-commit-diff.patch;
	cd /usr/local/bin/;
	sudo patch -p1 < git-commit-diff.patch;
	sudo rm git-commit-diff.patch;
	ls /usr/local/bin/git-commit-mvauto;

	echo ""
	echo "Installing Python Dependent Packages:"
	echo ""
	sudo pip install BeautifulSoup4;
	sudo pip install lxml;
	echo ""


	echo " ********** Make sure you have updated file '.bugzpass.txt' with your LDAP/bugz Password ********** "
	echo ""

fi
