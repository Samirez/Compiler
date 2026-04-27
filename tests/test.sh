#!/bin/sh

echo PL/0 compiler tests
echo =========================

failures=0

if [ -x ../pl0c ]; then
  COMPILER=../pl0c
elif [ -x ../pl0c.exe ]; then
  COMPILER=../pl0c.exe
else
  echo "Compiler binary not found (expected ../pl0c or ../pl0c.exe)"
  exit 1
fi

# Run the tests
for i in *.pl0; do
    echo -n "${i%.pl0}... "
  "$COMPILER" $i
    if [ $? -eq 0 ] ; then
      echo ok
    else
        echo fail
    failures=$((failures + 1))
    fi
done

if [ $failures -eq 0 ] ; then
  echo "All tests passed!"
else
  echo "$failures test(s) failed!"
  exit $failures
fi