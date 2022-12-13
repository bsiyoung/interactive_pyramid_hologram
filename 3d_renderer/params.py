# Window
title = 'Interactive Pyramid Hologram'
full_screen = True
window_size = [None, None]  # None : Monitor Size

# Shaders
vert_shader = './shaders/vert.vs'
frag_shader = './shaders/frag.fs'

# Camera
view_angle = 65.0  # deg
view_min_depth = 0.1
view_max_depth = 100

# Models
# Object File : {model_path}/{name}/model.obj
# Texture File : {model_path}/{name}/texture.jpg
model_path = './res'
models = [
	'cube'
]
