# Commandes avec beaucoup d'allocations/désallocations
echo "test" | cat | grep "test" | wc -l
ls -la | head -5 | tail -3 | grep -v "total"
find /tmp -name "*.tmp" 2>/dev/null | head -10 | xargs ls -l 2>/dev/null

# Variables d'environnement multiples
export VAR1="value1" VAR2="value2" VAR3="value3"
echo $VAR1 $VAR2 $VAR3
unset VAR1 VAR2 VAR3

# Commandes avec redirections multiples
echo "line1" > /tmp/test1.txt && echo "line2" >> /tmp/test1.txt
cat /tmp/test1.txt | grep "line" > /tmp/test2.txt
rm -f /tmp/test1.txt /tmp/test2.txt

# Pipes complexes avec erreurs potentielles
cat /dev/null | cat | cat | cat | wc -l
echo "data" | grep "data" | sed 's/data/result/' | cat

# Commandes avec arguments nombreux
echo arg1 arg2 arg3 arg4 arg5 arg6 arg7 arg8 arg9 arg10
ls /home /tmp /var /usr /bin 2>/dev/null | head -20

# Boucles implicites avec wildcards
echo /bin/c* | wc -w
ls /usr/bin/[a-c]* 2>/dev/null | head -5

# Commandes avec chemins longs
cd /tmp && mkdir -p a/b/c/d/e && cd a/b/c/d/e && pwd && cd - && rm -rf /tmp/a

# Gestion d'erreurs et signaux
sleep 0.1 &
jobs
kill %1 2>/dev/null

# Commandes avec quotes complexes
echo 'single "quotes" test' | cat
echo "double 'quotes' test" | wc -c
echo "$HOME/test" | grep "$USER"

# Heredoc simulation
cat << EOF | grep "test"
This is a test
Another test line
EOF

# Variables avec expansions
TEST_VAR="hello world"
echo ${TEST_VAR} | tr ' ' '_'
unset TEST_VAR

# Commandes conditionnelles
test -d /tmp && echo "tmp exists" || echo "tmp missing"
[ -f /etc/passwd ] && head -3 /etc/passwd

# Processus en arrière-plan
sleep 0.2 &
wait

# Redirections complexes
echo "stdout" 1>/tmp/out.txt 2>/tmp/err.txt
cat /tmp/out.txt /tmp/err.txt 2>/dev/null
rm -f /tmp/out.txt /tmp/err.txt

# Commandes avec escape sequences
echo -e "line1\nline2\tindented" | cat -n
printf "formatted %s %d\n" "string" 42

# Tests de limites
seq 1 100 | head -10 | tail -5
yes "test" | head -50 | wc -l

# Commandes avec subshells
(cd /tmp && pwd && ls | head -3)
echo $(echo "nested command substitution")

# Gestion de fichiers temporaires
mktemp /tmp/minishell_test.XXXXXX | xargs rm -f
