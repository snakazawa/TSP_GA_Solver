CC = g++
CFLAGS = -O2 -std=c++11 -Wall -Wshadow -Wextra
CFLAGS_DEBUG = -O0 -g -std=c++11 -Wall -Wshadow -Wextra
INIT1_DEF = -DINIT_TYPE='"rand"' -DRESULT_DIR_PATH='"results_init1"'
OX_DEF = -DCROSSOVER_TYPE='"ox"'
ER_DEF = -DCROSSOVER_TYPE='"er"'

all: \
    main \
    main_ox_init1 \
    main_er_init1 \
    main_ox_init2 \
    main_er_init2 \
    main_ox_init3 \
    main_er_init3 

all_init2: main_ox_init2 main_er_init2

all_dev: \
    main_er_init1_pm0 \
    main_er_init1_pm1 \
    main_er_init1_pm10 \
    main_ox_init1_grp1 \
    main_ox_init1_grp2 \
    main_ox_init1_grp3 \
    main_ox_init1_grp6 \
    main_ox_init1_grp13 

main: 
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
    -DLOG_FILE_ID='"dev"' \
    main.cpp -o main

main_dev:
	${CC} ${CFLAGS} ${INIT1_DEF} ${OX_DEF} \
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

main_er_init1_pm0:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
	-DP_MUTATION=0 \
	-DLOG_FILE_ID='"er_init1_pm0"' \
	main.cpp -o main_er_init1_pm0

main_er_init1_pm1:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
	-DP_MUTATION=0.01 \
	-DLOG_FILE_ID='"er_init1_pm1"' \
	main.cpp -o main_er_init1_pm1

main_er_init1_pm10:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
	-DP_MUTATION=0.1 \
	-DLOG_FILE_ID='"er_init1_pm10"' \
	main.cpp -o main_er_init1_pm10

main_ox_init1_grp1:
	${CC} ${CFLAGS} ${INIT1_DEF} ${OX_DEF} \
    -DGROUP_NUM=1 \
	-DLOG_FILE_ID='"ox_init1_grp1"' \
	main.cpp -o main_ox_init1_grp1

main_ox_init1_grp2:
	${CC} ${CFLAGS} ${INIT1_DEF} ${OX_DEF} \
    -DGROUP_NUM=2 \
	-DLOG_FILE_ID='"ox_init1_grp2"' \
	main.cpp -o main_ox_init1_grp2

main_ox_init1_grp3:
	${CC} ${CFLAGS} ${INIT1_DEF} ${OX_DEF} \
    -DGROUP_NUM=3 \
	-DLOG_FILE_ID='"ox_init1_grp3"' \
	main.cpp -o main_ox_init1_grp3

main_ox_init1_grp6:
	${CC} ${CFLAGS} ${INIT1_DEF} ${OX_DEF} \
    -DGROUP_NUM=6 \
	-DLOG_FILE_ID='"ox_init1_grp6"' \
	main.cpp -o main_ox_init1_grp6

main_er_init1_grp13:
	${CC} ${CFLAGS} ${INIT1_DEF} ${ER_DEF} \
    -DLOOP_NUM=1000 \
    -DGROUP_NUM=13 \
	-DLOG_FILE_ID='"er_init1_grp13"' \
	main.cpp -o main_er_init1_grp13

main_ox_init1_grp13:
	${CC} ${CFLAGS} ${INIT1_DEF} ${OX_DEF} \
    -DLOOP_NUM=1000 \
    -DGROUP_NUM=13 \
	-DLOG_FILE_ID='"ox_init1_grp13"' \
	main.cpp -o main_ox_init1_grp13

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
	${RM} \
    main \
    main_ox_init1 \
    main_er_init1 \
    main_ox_init2 \
    main_er_init2 \
    main_ox_init3 \
    main_er_init3 

clean_init2: 
	${RM} main_ox_init2 main_er_init2

clean_dev:
	${RM} \
    main_er_init1_pm0 \
    main_er_init1_pm1 \
    main_er_init1_pm10 \
    main_ox_init1_grp1 \
    main_ox_init1_grp2 \
    main_ox_init1_grp3 \
    main_ox_init1_grp6

