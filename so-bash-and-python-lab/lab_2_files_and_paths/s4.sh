if [ $# -ne 1 ]; then
    echo "nope, bc of args num != 1"
fi

if [ ! -d $1 ]; then
    echo "nope, bc of '$1' isn't dir"
    exit 1
fi

idx=1
list=`ls -S $1`
for file in $list; do
    if [ -f $1/$file ] && [ -x $1/$file ]; then
        #echo $1/$file
        mv $1/$file $1/$file.$idx
        let ++idx
    fi
done
