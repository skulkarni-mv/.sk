
# OUTPUT cmd: skgcc   (~/.bashrc file alias -> alias skgcc='~/.sk/gcc_new_op.sh')

gcc ~/.sk/bb_build_sk.c 	-o ~/.sk/bbb; 		sudo mv ~/.sk/bbb 	/bin/
gcc ~/.sk/bb_clean_sk.c 	-o ~/.sk/bbc; 		sudo mv ~/.sk/bbc 	/bin/
gcc ~/.sk/devtool_extract_cgx.c	-o ~/.sk/sdevtool; 	sudo mv ~/.sk/sdevtool	/bin/
gcc ~/.sk/loop_cmd_sk.c 	-o ~/.sk/loop; 		sudo mv ~/.sk/loop 	/bin/
gcc ~/.sk/newline.c 		-o ~/.sk/newl; 		sudo mv ~/.sk/newl 	/bin/
gcc ~/.sk/sk.c 			-o ~/.sk/sk; 		sudo mv ~/.sk/sk 	/bin/
gcc ~/.sk/tsk_timetaken.c 	-o ~/.sk/tsk; 		sudo mv ~/.sk/tsk 	/bin/
gcc ~/.sk/tsk_time_redirectOP.c -o ~/.sk/tskr;  	sudo mv ~/.sk/tskr 	/bin/
gcc ~/.sk/smake_build.c 	-o ~/.sk/smake; 	sudo mv ~/.sk/smake 	/bin/
gcc ~/.sk/git_desc_code.c 	-o ~/.sk/gitdsk2; 	sudo mv ~/.sk/gitdsk2 	/bin/

sudo cp ~/.sk/clone_script.sh /bin/clone_script

# EXCLUDED FILES
# git_desc_code.c	// INCLUDED
# z_ReplaceEnterINfile.c

