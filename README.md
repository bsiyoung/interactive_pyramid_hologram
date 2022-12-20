# interactive_pyramid_hologram
![KakaoTalk_20221216_003103777](https://user-images.githubusercontent.com/39472306/208472619-4a1fec7d-3d26-4d13-9dd2-87052452ce7d.jpg)

<details>
<summary>3D Renderer</summary>

### 3d_renderer
OpenGL을 사용해 피라미드 홀로그램에 사용되는 3차원 입체 영상을 실시간으로 렌더링한다.
### Steps To Run 3d_renderer
1. Install packages
```
pip install -r requirements.txt
```

2. Run 3d_renderer as sudo
```
sudo python run.py
```

### Fix Bugs
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
