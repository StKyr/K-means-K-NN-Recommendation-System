cd ./libs/boost_1_68_0/
./bootstrap.sh --with-libraries=program_options --prefix=./
./b2
rm b2
rm bootstrap.log
rm bjam
echo "BOOST LIBRARY IS SET UP"