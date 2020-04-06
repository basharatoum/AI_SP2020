for i in {1..42}
do
    declare -a array=( $( { time python maxconnect4.py one-move input1.txt o.txt $i; } 2>&1 >/dev/null ))
    echo ${i} ' gave us ' ${array[3]}>>i.txt
done