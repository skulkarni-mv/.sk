rm .tar_cgx_kernel_automate.tar ;

wget "https://github.com/skulkarni-mv/.sk/raw/master/11_CGX26_CVE_patch_automate/.tar_cgx_kernel_automate.tar" &&

tar xf .tar_cgx_kernel_automate.tar &&
rm .tar_cgx_kernel_automate.tar     &&

echo ""                             &&
ls -A ./cgx_kernel_automate/        &&
echo ""                             &&

echo "Downloaded folder \"cgx_kernel_automate\". Check \"ReadMe.txt\" file inside for details and run \"make.sh\" script to start" &&
echo ""

