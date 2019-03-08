echo "Rebuilding..."
make clean
make all
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
echo "Running testdom.sh"
./testdom.sh > test
echo "Running diff"
diff reference test
