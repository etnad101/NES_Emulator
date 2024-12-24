cd build && cmake .. && make

if [ -f ../out.txt ]; then
    rm ../out.txt 
fi

./nes_emulator >> ../out.txt && cd ..

if [ -f diff.txt ]; then
    rm diff.txt 
fi

diff -iw out.txt nestest.log.txt >> diff.txt