# interactive_pyramid_hologram
![KakaoTalk_20221216_003103777](https://user-images.githubusercontent.com/39472306/208486203-b6bcd818-35c0-4204-94a5-bbda7ff98875.jpg)

## Modules
<details>
<summary>3D Renderer</summary>

### 3d_renderer
OpenGL을 사용해 피라미드 홀로그램에 사용되는 3차원 입체 영상을 실시간으로 렌더링한다.
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
