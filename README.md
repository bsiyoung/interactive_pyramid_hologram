# Interactive Pyramid Hologram
<img src="https://user-images.githubusercontent.com/39472306/208486988-b94a9edf-2f89-4494-9d59-0e5b670b0c8d.jpg" width="500" height="333"/>
이 프로젝트는 라즈베리파이와 각종 센서들을 사용해 피라미드 홀로그램의 형상을 실시간으로 제어하는 것을 목표로 한다. 제

피라미드 홀로그램 제작 방법 : [Youtube](https://www.youtube.com/watch?v=FnUrI_3LBuc)

## System Structure
<img src="https://user-images.githubusercontent.com/39472306/208497367-192fdc5a-169c-4ba7-a09a-bb8eae7ae46d.png" width="600" height="350"/>

## Modules
<details>
<summary>3D Renderer</summary>

### 3d_renderer
OpenGL을 사용해 3차원 입체 영상을 피라미드 홀로그램에 사용되는 형식으로 실시간 렌더링한다.
#### Steps To Run 3d_renderer
1. Install packages
```
pip install -r requirements.txt
```

2. Run 3d_renderer as sudo
```
sudo python run.py
```

#### Fix Bugs
GLFW 관련 에러
```
sudo apt install libglfw3 libglfw3-dev
```

Numpy 관련 에러
```
sudo apt install libatlas-base-dev
```

GLSL 3.30 Is Not Supported 에러
```
export MESA_GL_VERSION_OVERRIDE=3.3
```
  

#### 프로그램 구조
1. 메인 스레드
https://github.com/bsiyoung/interactive_pyramid_hologram/blob/3758711d55ba512472403f88236e39f6535c63b5/3d_renderer/run.py#L106
  
GLFW와 OpenGL을 사용해 윈도우를 생성하고 피라미드 홀로그램에 사용되는 영상 형식대로 3D 오브젝트를 렌더링한다.

<img src="https://user-images.githubusercontent.com/39472306/208679937-336b8c62-8395-4678-a26c-747c1146ffe0.png" width="500" height="333"/>
  
Callback 함수를 사용해 사용자의 키보드 입력을 받아들인다(수동 오브젝트 조작 & 프로그램 종료).
  
|Key|동작|
|------|---|
|Q|Quit|
|Z|Zoom Out|
|X|Zoom In|
|W/S|X축 회전|
|E/D|Y축 회전|
|R/F|Z축 회전|
  
2. IPC 통신 스레드
https://github.com/bsiyoung/interactive_pyramid_hologram/blob/3758711d55ba512472403f88236e39f6535c63b5/3d_renderer/ipc_msgq.py#L14
백그라운드에서 실행되는 스레드. POSIX IPC Message Queue를 사용해 Core 프로그램에서 전송하는 데이터를 받아들여 처리한다.

</details>

<details>
<summary>Core</summary>

### Core
센서와 블루투스로부터 값을 받아 3D renderer 프로세스로 보내고, stdin을 통해 모드를 변경하거나 모델 변경한다.
### Steps To Run core
1. Compile core
```
gcc -o prj_3 prj_3.c -lm -lrt -lwiringPi -lpthread
```

2. Run core as sudo
```
sudo ./prj_3
```
### Structure
stdin을 받아 특정 기능을 수행하는 main thread, 센서와 블루투스 값을 받아 3D renderer 프로세스로 값을 보내는 Send thread 두 개의 스레드로 동작.<br>
특정 기능에 대해서는 아래의 항목을 참고바람.

### Function
아래의 문자를 입력 후 enter를 누르면 해당 기능을 실행<br><br>
m : 모드 토글 (센서의 값을 받을지 블루투스의 값을 받을지 변경)<br>
0~2 : 3D renderer에서 표시하는 모델 변경<br>
q : 프로그램 종료

### Precautions
초음파 센서가 연결되어 있지 않으면 프로그램이 정상적으로 동작하지 않아 프로그램을 실행할 때는 반드시 초음파 센서가 연결된 상태여야 함.
</details>

<details>
<summary>Android App</summary>

  
### Andorid APP
휴대폰의 내장된 자이로센서 값을 자이로센서 api를 통하여 값을 받아온다. 값을 받아온후 적절히 데이터 파싱을 하여 블루투스(Uart)통신을 이용하여 라즈베리파이에 값을 전달한다.

### App Environment
실행환경: android studio(feat.라즈베리파이)<br>
디버깅 및 컴파일: 실행환경에서 휴대폰 또는 가상시뮬레이터에 어플이 자동으로 깔리고 컴파일 및 실행이 진행된다. 

### Mannual
1. 안드로이드 스튜디오에서 깃 클론을 진행하여 해당 파일을 받아온다.<br>
2. 해당파일을 휴대폰과 연결하여 실행(어플 설치)<br>
3. 블루투스 켜기 버튼을 누른다.<br>
4. 블루투스가 활성화되었다는 메세지(토스트)를 받았으면 연결버튼을 눌러 연결하고 싶은 기기를 찾아 연결한다.<br>
5. 연결이 완료되었다는 메세지를 받았으면 전송 버튼을 눌러 휴대폰 자이로센서 값을 전송한다.(pitch, roll, yaw값)<br>
  
### Limitation
1. 라즈베리파이 IPC통신 프로그램에서 쓰레드에 값을 넣고 시리얼 통신을 진행하니 자꾸 SerialRead가 정상적으로 작동하지 않았다.
그래서 메인문에 값을 넣어 임시 조치를 취하였다.
  
2. 자이로센서로 읽어들인 데이터 값을 write함수를 통하여 값을 연속적으로 바로 전달하려고 했지만 제대로 된 값이 나오지 않았다. 
  ->버튼을 한번 누를때마다 데이터값이 갱신되어 한번씩 보내진다.


### Improvement
1. 라즈베리파이에서 값을 1바이트씩 읽기때문에 pitch, roll, yaw값을 A,B,C와 같은 문자열로 구분해서 보내주었다.

### Ui Flow

[블루투스 ON 클릭시]<br>
![image](https://user-images.githubusercontent.com/93969640/208674914-74de2574-13d4-4b44-bfdd-f9085bf88235.png)<br>

[허용]<br>
![image](https://user-images.githubusercontent.com/93969640/208675074-1abc6dc5-a51d-42bb-8784-8d09961468b2.png)<br>
상태를 나타내는 텍스트에딧이 활성화로 바뀜.<br>

[연결버튼]<br>
![image](https://user-images.githubusercontent.com/93969640/208675103-60497989-aa14-4e32-a5e9-2b3d9e17909d.png)<br>

![image](https://user-images.githubusercontent.com/93969640/208675352-04efff0c-c3f3-4e0b-b05e-839e3c25c60d.png)<br>

</details>
