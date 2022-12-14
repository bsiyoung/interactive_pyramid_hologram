# Constant
PI = 3.14159265

# IPC Message Queue
ipc_msgq_name = '/holo_ipc'
byteorder = 'little'  # Endian{'little', 'big'}

# Window
title = 'Interactive Pyramid Hologram'
full_screen = False
window_size = [None, None]  # None : Monitor Size

# Shaders
vert_shader = './shaders/video_17_vert.vs'
frag_shader = './shaders/video_17_frag.fs'

# Camera
view_angle = 70.0  # deg
view_min_depth = 0.1
view_max_depth = 100

# OpenGL
max_fps = 60

# Control
min_depth = 10.0
max_depth = 25.0
elev_angle = 23 # deg

# Models
# Object File : {model_path}/{name}/model.obj
# Texture File : {model_path}/{name}/texture.jpg
model_path = './res'
models = [
    # [model name, depth coefficient]
	['cube', 0.9]
]
