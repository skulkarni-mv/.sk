

gcc ~/.sk/sk.c 			-o ~/.sk/sk; 	sudo mv ~/.sk/sk   /bin/
gcc ~/.sk/tsk_timetaken.c	-o ~/.sk/tsk;	sudo mv ~/.sk/tsk  /bin/
gcc ~/.sk/tsk_time_redirectOP.c	-o ~/.sk/tskr;	sudo mv ~/.sk/tskr /bin/
gcc ~/.sk/loop_cmd_sk.c 	-o ~/.sk/loop; 	sudo mv ~/.sk/loop /bin/
gcc ~/.sk/newline.c 		-o ~/.sk/newl; 	sudo mv ~/.sk/newl /bin/

gcc ~/.sk/copy2local.c 		-o ~/.sk/c2l;	sudo mv ~/.sk/c2l  /bin/
gcc ~/.sk/copy2vm.c 		-o ~/.sk/c2v;	sudo mv ~/.sk/c2v  /bin/

gcc ~/.sk/bulk_cp2loc.c         -o ~/.sk/bulkcp2;       sudo mv ~/.sk/bulkcp2 /bin/

gcc ~/.sk/send_text_to_Laptop.c -o ~/.sk/sendtoLAP; 	sudo mv ~/.sk/sendtoLAP /bin/

### EXCLUDED FILES
# z_ReplaceEnterINfile.c

### -------------------    From VM   -------------------------------------- ###

#gcc ~/.sk/VM_.sk_dir/sk.c                  -o ~/.sk/sk;            sudo mv ~/.sk/sk        /bin/	# Duplicates
#gcc ~/.sk/VM_.sk_dir/tsk_timetaken.c       -o ~/.sk/tsk;           sudo mv ~/.sk/tsk       /bin/	# Duplicates
#gcc ~/.sk/VM_.sk_dir/tsk_time_redirectOP.c -o ~/.sk/tskr;          sudo mv ~/.sk/tskr      /bin/	# Duplicates
#gcc ~/.sk/VM_.sk_dir/loop_cmd_sk.c         -o ~/.sk/loop;          sudo mv ~/.sk/loop      /bin/	# Duplicates
#gcc ~/.sk/VM_.sk_dir/newline.c             -o ~/.sk/newl;          sudo mv ~/.sk/newl      /bin/	# Duplicates

sudo cp ~/.sk/VM_.sk_dir/clone_script.sh   /bin/clone_script

gcc ~/.sk/VM_.sk_dir/bb_build_sk.c         -o ~/.sk/bbb;           sudo mv ~/.sk/bbb       /bin/
gcc ~/.sk/VM_.sk_dir/bb_clean_sk.c         -o ~/.sk/bbc;           sudo mv ~/.sk/bbc       /bin/
gcc ~/.sk/VM_.sk_dir/devtool_extract_cgx.c -o ~/.sk/sdevtool;      sudo mv ~/.sk/sdevtool  /bin/
gcc ~/.sk/VM_.sk_dir/smake_build.c         -o ~/.sk/smake;         sudo mv ~/.sk/smake     /bin/

### EXCLUDED FILES
#gcc ~/.sk/VM_.sk_dir/git_desc_code.c      -o ~/.sk/gitdsk2;       sudo mv ~/.sk/gitdsk2   /bin/	##Not used frquently