if [ $# -ne 2 ]; then
    echo "nope, bc of args num != 2"
fi

if [ ! -d $1 ]; then
    echo "nope, bc of '$1' isn't dir"
    exit 1
fi

if [ ! -d $2 ]; then
    echo "nope, bc of '$2' isn't dir"
    exit 1
fi

for file in $1/*; do
    if [ -f $file ] && [ -x $file ]; then
        echo $file $2/$(basename $file)
        mv $file $2/$(basename $file)
    fi
done
