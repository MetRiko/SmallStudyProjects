# a)

./links_1_2.sh

# b)

cwd=Lab3/K6/K7
cp $cwd/link_P10 Lab3
mv Lab3/link_P10 Lab3/link_P10_kopia
cp $cwd/link_P10_bez Lab3
mv Lab3/link_P10_bez Lab3/link_P10_bez_kopia
cp -P $cwd/link_P10 Lab3
cp -P $cwd/link_P10_bez Lab3

# c)
cwd=Lab3/K6
rm `readlink -f $cwd/link_P1`

# d)
paths="Lab3 Lab3/K1 Lab3/K1/K2 Lab3/K3 Lab3/K3/K4 Lab3/K3/K4/K5 Lab3/K6 Lab3/K6/K7"
target=`readlink -f Lab3/K3/K4/K5/P10`

echo ""
echo "Znalezione linki do P10:"
for dir in $paths; do
    files=`ls ./$dir`
#     echo `ls $dir`
    for file in $files; do
        file=$dir/$file
        if [ -h $file ]; then
            fullpath=`readlink -f $file`
            if [ "$fullpath" = "$target" ]; then
                echo "> "$file
            fi
        fi
    done
done

echo ""
