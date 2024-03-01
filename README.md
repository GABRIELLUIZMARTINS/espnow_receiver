# Log record

To generate log, run:
idf.py -p port monitor | tee debug/data.log

Tests:

To flash:
get_idf && idf.py -p /dev/ttyACM0 flash
To monitor:    
    get_idf && idf.py -p /dev/ttyACM0 monitor | tee Teste_vida/data.log
