1.	 Win32Project1.exe 파일을 실행시킴

2.	 새로운 Input 을 Generate 하고자 하면 파일 -> New_Input 을 누른다
	그러면 3 - 100 개의 랜덤한 점들이 생성되고
	해당 점은 실행 위치에 Input.txt 파일로 저장된다
	그리고 자동으로 Brutal, Graham Algorithm 이 실행되고
	같이 켜지는 Consol 창에 걸리는 시간이 표시된다.

3.	 다른 파일을 Load 해서 이용하려면 파일 -> Load_Input 을 누른다
	그러면 실행 위치에 있는 Input.txt 라는 파일을 읽어서
	해당 알고리즘을 수행한다.
	Input.txt 는 <124, 123> 이런 식으로 표시된 점들의 집합이여야 한다.

4.	 알고리즘을 수행하면 Output_Brutal.txt 와 Output_Graham.txt 이
	생성되는데 이 것은 해당 알고리즘을 수행했을 때
	외곽선이 되는 점들을 출력하였고, 실행 시간을 Cycle 로 표시했다.

5.	 Random Generate 점은 MAX_POINT_COUNT 라는 변수를 수정하면 된다.