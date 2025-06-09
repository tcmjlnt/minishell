files=$(ls *.txt)
count=$(echo $files | wc -w)
echo "Found $count text files: $files"