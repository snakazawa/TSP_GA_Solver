CC = g++
CFLAGS = -O3 -std=c++11 -Wall -Wshadow -Wextra -fopenmp
CFLAGS_DEBUG = -O0 -g -std=c++11 -Wall -Wshadow -Wextra
INIT1_DEF = -DINIT_TYPE='"rand"' -DRESULT_DIR_PATH='"results_init1"'
OX_DEF = -DCROSSOVER_TYPE='"ox"'
ER_DEF = -DCROSSOVER_TYPE='"er"'

main: 
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
    -DK_INDIVIDUAL_NUM=5 \
    -DCONTINUOUS_MAX_CNT=50 \
    -DP_MUTATION=0.01 \
    -DLOG_FILE_ID='"dev"' \
    main.cpp -o main

main_dev:
	${CC} ${CFLAGS} ${INIT1_DEF} ${OX_DEF} \
	-DLOG_FILE_ID='"dev"' \
    main.cpp -o main_er_init1

main_ox_init2:
	${CC} ${CFLAGS} \
    -DCONTINUOUS_MAX_CNT=10000 \
	-DINIT_TYPE='"src"' \
	-DCROSSOVER_TYPE='"ox"' \
	-DRESULT_DIR_PATH='"results_init2"' \
    -DSRC_RESULT_DIR_PATH='"results_init1"' \
	-DLOG_FILE_ID='"ox_init2"' \
	main.cpp -o main_ox_init2

main_er_init2:
	${CC} ${CFLAGS} \
    -DCONTINUOUS_MAX_CNT=10000 \
	-DINIT_TYPE='"src"' \
	-DCROSSOVER_TYPE='"er"' \
	-DRESULT_DIR_PATH='"results_init2"' \
    -DSRC_RESULT_DIR_PATH='"results_init1"' \
	-DLOG_FILE_ID='"er_init2"' \
	main.cpp -o main_er_init2

main_ox_init3:
	${CC} ${CFLAGS} \
	-DINIT_TYPE='"src"' \
	-DCROSSOVER_TYPE='"ox"' \
	-DRESULT_DIR_PATH='"results_init3"' \
    -DSRC_RESULT_DIR_PATH='"results_init2"' \
	-DLOG_FILE_ID='"ox_init3"' \
	main.cpp -o main_ox_init3

main_er_init3:
	${CC} ${CFLAGS} \
	-DINIT_TYPE='"src"' \
	-DCROSSOVER_TYPE='"er"' \
	-DRESULT_DIR_PATH='"results_init3"' \
    -DSRC_RESULT_DIR_PATH='"results_init2"' \
	-DLOG_FILE_ID='"er_init3"' \
	main.cpp -o main_er_init3

clean:
	${RM} main

clean_init2: 
	${RM} main_ox_init2 main_er_init2

