import os

os.system("g++ -c -I./project/lib/fizzbuzz $(find . -type f -name '*.cpp')")