gcc code_for_automation.c -o automate_run

if [ -f "git-commit-diff.patch" ];
then

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

fi
