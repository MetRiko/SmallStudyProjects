#dir="/home/stud/jmajewsk/Pulpit/lab4/dir"

echo "--- A ---"

find . -type f -name "*.txt" | sort | tail -n 5

echo "--- B ---"

find . -type f -size +1k -and -size -5k | xargs -I{} readlink -f {} | tr '/' '\\' 

echo "--- C ---"

pgs=`find . -maxdepth 2 -and \( -name '*.jpg' -or -name '*.png' \)`

find . -maxdepth 2 -and \( -name '*.jpg' -or -name '*.png' \) -exec du -ch {} + | tail -n 1

echo "--- D ---"

#-mmin -3 
echo "" > "result.txt"
find . -type f -name "*.dat" -mmin -3  | xargs -I{} readlink -f {} | while read file; do
    echo "$file"`date -r "$file" '+|%d-%m-%Y|%H:%M:%S'`
done | tee "result.txt"

echo "--- E ---"

text=`cat "./result.txt"`
for line in $text; do
    echo "$line" | cut -d'|' -f1 | rev | cut -d'/' -f1 | rev 
    echo "$line" | cut -d'|' -f2 | cut -d'-' -f1
    echo "$line" | cut -d'|' -f3 | cut -d':' -f3
    echo ""
    #line=$(line)
    #echo $line
done
