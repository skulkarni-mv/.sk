echo "Deleting the existing data in ~/Downloads/ folder in 3 seconds..." && \
echo "" && \
sleep 3 && \
rm -rf ~/Downloads/skulkarni-mv* && \
rm -rf ./cgx_kernel_automate/ && \

echo "#!/usr/bin/python3" > code_web.py            && \
echo "" >> code_web.py                             && \
echo "import time" >> code_web.py                  && \
echo "import webbrowser" >> code_web.py            && \
echo "" >> code_web.py                             && \
echo "time.sleep(0.1)" >> code_web.py              && \
echo "webbrowser.open('https://download-directory.github.io/?url=https%3A%2F%2Fgithub.com%2Fskulkarni-mv%2F.sk%2Ftree%2Fmaster%2F11_CGX26_CVE_patch_automate%2Fcgx_kernel_automate')" >> code_web.py && \
echo "time.sleep(5)" >> code_web.py                && \

chmod 755 code_web.py                              && \
./code_web.py                                      && \

sleep 1                                            && \
rm code_web.py                                     && \

unzip ~/Downloads/skulkarni-mv*.zip -d ~/Downloads/cgx_kernel_automate/ && \

mv ~/Downloads/cgx_kernel_automate/ . && \

echo "" && \
ls -A ./cgx_kernel_automate/ && \
echo "" && \

rm ~/Downloads/skulkarni-mv*.zip && \

chmod a+x ./cgx_kernel_automate/make.sh

