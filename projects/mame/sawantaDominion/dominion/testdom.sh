for a in {1..20}
do
    echo "Random seed $a:"
    ./playdom $a
    echo
done
