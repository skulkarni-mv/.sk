echo "Deleting the existing data in ~/Downloads/ folder in 3 seconds..." && \
echo "" && \
sleep 3 && \
rm -rf ~/Downloads/skulkarni-mv* && \
rm -rf ./cgx_kernel_automate/ && \
./code_web.py && \
sleep 1 &&

unzip ~/Downloads/skulkarni-mv*.zip -d ~/Downloads/cgx_kernel_automate/ && \

mv ~/Downloads/cgx_kernel_automate/ . && \

echo "" && \
ls -A ./cgx_kernel_automate/ && \
echo "" && \

rm ~/Downloads/skulkarni-mv*.zip && \

chmod a+x ./cgx_kernel_automate/make.sh

