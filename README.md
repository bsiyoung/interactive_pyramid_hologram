# core - IPC

reception.c 는 prj에서 보내는 데이터가 다른 프로세스로 전달이 잘 되는지 확인하기 위해 넣은 파일.

같은 터미널에서 확인하고 싶으면 reception 파일을 백그라운드에서 실행 -> ./reception &

*백그라운드 실행 종료할 때는 -> ps 쳐서 pid 확인 후 "kill -9 pid번호"


확인할 필요가 없으면 reception.c 파일은 삭제해도 됨
