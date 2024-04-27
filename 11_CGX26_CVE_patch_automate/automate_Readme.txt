
Dependency:

PWD: /usr/local/bin

user@group:/usr/local/bin$ sudo cp git-commit-mv git-commit-mvauto
user@group:/usr/local/bin$ echo "<copy the diff below and create a diff.patch file>"
user@group:/usr/local/bin$
user@group:/usr/local/bin$ cat diff.patch
diff --git a/git-commit-mvauto b/git-commit-mvauto
index f8d8676..6dbdf6d 100755
--- a/git-commit-mvauto
+++ b/git-commit-mvauto
@@ -372,6 +372,12 @@ Disposition: %s
     if not body.startswith("Description:\n"):
         header += "Description:\n\n"
 
+        fp = open('cgx_kernel_automate/generated_details.txt', 'r')       ## -> Added by Shubham
+        cve_summary=fp.read()
+        fp.close()
+        header += cve_summary
+        header += "\n\n"                                                  ## <- Added by Shubham
+
     return header + body
 
 
user@group:/usr/local/bin$ sudo patch -p1 < diff.patch
user@group:/usr/local/bin$ echo "<It will add the changes in the git-commit-mvauto file. Confirm by running the below diff cmd>"
user@group:/usr/local/bin$ git diff git-commit-mv git-commit-mvauto

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
                 automate_* files


user@group:WorkDirectory$ git clone -b mvl-4.19/common skulkarni@gitcgx.mvista.com:/mvista/git/cgx/CGX2.6/kernel/linux-mvista-2.6.git
user@group:WorkDirectory$ ls
			$ linux-mvista-2.6
				   
user@group:WorkDirectory$ mkdir gregkh-linux
user@group:WorkDirectory$ cd gregkh-linux/
user@group:gregkh-linux $ git clone https://github.com/gregkh/linux.git
user@group:gregkh-linux $ ls
			$ linux


Files needs to be present in 'linux-mvista-2.6' dir:
i.e. automate_* files

	automate_Readme.txt
	automate_bugzpass.txt			-> Add your bugz password
	automate_commits.c
	generated_details.txt
	automate_cve_summary.py
	automate_run.out
	automate_send_pull_req.py
	make.sh


user@group:WorkDirectory$ gcc automate_commits.c -o automate_run.out    OR use	$ ./make.sh
user@group:WorkDirectory$ ./automate_run.out				OR 	$ ./automate_run.out bugz_num stable_commit_id

	 Usage: ./automate_run.out <bugz_num> <stable_commit_id> 


