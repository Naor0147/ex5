#!/bin/bash

TEST_DIR="$1"

# בדיקה שהתיקייה קיימת
if [ -z "$TEST_DIR" ] || [ ! -d "$TEST_DIR" ]; then
  echo "Usage: $0 /path/to/myTest"
  exit 1
fi

echo "🔍 Scanning tests in directory: $TEST_DIR"
echo "----------------------------------------"

# משתנה לספירת כישלונות
FAIL_COUNT=0

# רוץ על כל הקבצים בתיקייה
for testfile in "$TEST_DIR"/*; do
  if [ -f "$testfile" ]; then
    
    # הרצת Valgrind
    valgrind \
      --leak-check=full \
      --show-leak-kinds=all \
      --track-origins=yes \
      --error-exitcode=1 \
      ./ex5.out < "$testfile" > /dev/null 2> vg_tmp.log

    EXIT_CODE=$?
    FILENAME=$(basename "$testfile")

    if [ $EXIT_CODE -ne 0 ]; then
      # אם יש שגיאה - הדפס באדום
      echo -e "❌ \033[0;31mFAIL\033[0m: $FILENAME"
      FAIL_COUNT=$((FAIL_COUNT+1))
      
      # הדפסת השורות החשובות בלבד (Summary ושגיאות ספציפיות)
      grep -A 5 "ERROR SUMMARY" vg_tmp.log
      grep "definitely lost" vg_tmp.log
      grep "Invalid read" vg_tmp.log
      echo "----------------------------------------"
    else
      # אם עבר בהצלחה - הדפס בירוק
      echo -e "✅ \033[0;32mPASS\033[0m: $FILENAME"
    fi
  fi
done

# סיכום סופי
echo "----------------------------------------"
if [ $FAIL_COUNT -eq 0 ]; then
    echo -e "\033[0;32mAll tests passed successfully! Great job!\033[0m"
else
    echo -e "\033[0;31mFound errors in $FAIL_COUNT tests.\033[0m"
fi

rm -f vg_tmp.log