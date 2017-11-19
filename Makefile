CC = g++
CFLAGS = -O2 -std=c++11 -Wall -Wshadow -Wextra
CFLAGS_DEBUG = -O0 -g -std=c++11 -Wall -Wshadow -Wextra
INIT1_DEF = -DINIT_TYPE='"rand"' -DRESULT_DIR_PATH='"results_init1"'
OX_DEF = -DCROSSOVER_TYPE='"ox"'
ER_DEF = -DCROSSOVER_TYPE='"er"'

all: main main_ox_init1 main_er_init1 main_ox_init2 main_er_init2 main_ox_init3 main_er_init3

all_dev: main_er_init1_grp10 main_er_init1_grp5 main_er_init1_pm1 main_ox_init1_pm1 main_er_init1_pm10 main_ox_init1_pm10 main_er_init1_pm50 main_ox_init1_pm50  

main: 
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
	-DP_MUTATION=0.1 \
    -DLOG_FILE_ID='"dev"' \
    main.cpp -o main

main_dev:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
    -DGROUP_NUM=3 \
    -DLOOP_NUM=1000 \
    -DCONTINUOUS_MAX_CNT=200 \
	-DP_MUTATION=0.1 \
	-DLOG_FILE_ID='"dev"' \
	main.cpp -o main

main_ox_init1:
	${CC} ${CFLAGS} ${INIT1_DEF} ${OX_DEF} \
    -DLOG_FILE_ID='"ox_init1"' \
    main.cpp -o main_ox_init1

main_er_init1:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
    -DLOG_FILE_ID='"er_init1"' \
    main.cpp -o main_er_init1

main_er_init1_grp5:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
    -DGROUP_NUM=5 \
	-DP_MUTATION=0.1 \
	-DLOG_FILE_ID='"er_init1_grp5"' \
	main.cpp -o main_er_init1_grp5

main_er_init1_grp10:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
    -DGROUP_NUM=10 \
	-DP_MUTATION=0.1 \
	-DLOG_FILE_ID='"er_init1_grp10"' \
	main.cpp -o main_er_init1_grp10

main_ox_init1_pm1:
	${CC} ${CFLAGS} ${INIT1_DEF} ${OX_DEF} \
	-DP_MUTATION=0.01 \
	-DLOG_FILE_ID='"ox_init1_pm1"' \
	main.cpp -o main_ox_init1_pm1

main_er_init1_pm1:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
	-DP_MUTATION=0.01 \
	-DLOG_FILE_ID='"er_init1_pm1"' \
	main.cpp -o main_er_init1_pm1

main_ox_init1_pm10:
	${CC} ${CFLAGS} ${INIT1_DEF} ${OX_DEF} \
	-DP_MUTATION=0.1 \
	-DLOG_FILE_ID='"ox_init1_pm10"' \
	main.cpp -o main_ox_init1_pm10

main_er_init1_pm10:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
	-DP_MUTATION=0.1 \
	-DLOG_FILE_ID='"er_init1_pm10"' \
	main.cpp -o main_er_init1_pm10

main_er_init1_pm50:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
	-DP_MUTATION=0.5 \
	-DLOG_FILE_ID='"er_init1_pm50"' \
	main.cpp -o main_er_init1_pm50

main_ox_init2:
	${CC} ${CFLAGS} \
	-DINIT_TYPE='"src"' \
	-DCROSSOVER_TYPE='"ox"' \
	-DRESULT_DIR_PATH='"results_init2"' \
    -DSRC_RESULT_DIR_PATH='"results_init1"' \
	-DLOG_FILE_ID='"ox_init2"' \
	main.cpp -o main_ox_init2

main_er_init2:
	${CC} ${CFLAGS} \
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
	${RM} main main_ox_init1 main_er_init1 main_ox_init1_pm10 main_er_init1_pm10 mmain_ox_init2 main_er_init2 main_ox_init3 main_er_init3

clean_dev:
	${RM} main_er_init1_grp10 main_er_init1_grp5 main_er_init1_pm1 main_ox_init1_pm1 main_er_init1_pm10 main_ox_init1_pm10 main_er_init1_pm50 main_ox_init1_pm50 

