# zad 1

if [ -e Lab3 ]; then
    rm -rf Lab3
fi

mkdir Lab3

mkdir Lab3/K1
mkdir Lab3/K1/K2
mkdir Lab3/K3
mkdir Lab3/K3/K4
mkdir Lab3/K3/K4/K5
mkdir Lab3/K6
mkdir Lab3/K6/K7

cwd=Lab3/K1/K2
for idx in {1..9}; do
    echo "test "$idx > $cwd/"P"$idx
done

cwd=Lab3/K3/K4/K5
for idx in {10..11}; do
    echo "test "$idx > $cwd/"P"$idx
done

# zad 2

cwd=Lab3/K6/K7
ln -s ../../K3/K4/K5/P10 $cwd/link_P10
ln -s `readlink -f Lab3/K3/K4/K5/P10` $cwd/link_P10_bez

cwd=Lab3/K6
for idx in {1..9}; do
    ln -s ../K1/K2/P$idx $cwd/link_P$idx
done
ln -s `readlink -f Lab3/K3/K4` $cwd/link_K4_bez
