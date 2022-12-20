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
</details>
