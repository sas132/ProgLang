make clean
make
echo "--------------------------------------------------------------------------------------------------------------------------"
echo "							testing legal1"
./main legal1
echo "--------------------------------------------------------------------------------------------------------------------------"
echo "							testing legal2"
./main legal2
echo "--------------------------------------------------------------------------------------------------------------------------"
echo "							testing legal3"
./main legal3
echo "--------------------------------------------------------------------------------------------------------------------------"
echo "							testing legal4"
./main legal4
echo "--------------------------------------------------------------------------------------------------------------------------"
echo "							testing illegal1"
./main illegal1
echo "--------------------------------------------------------------------------------------------------------------------------"
echo "							testing illegal2"
./main illegal2
echo "--------------------------------------------------------------------------------------------------------------------------"
echo "							testing illegal3"
./main illegal3
echo "--------------------------------------------------------------------------------------------------------------------------"
echo "							testing illegal4"
./main illegal4
