import math
import time
import threading

from screeninfo import get_monitors

import glfw
from OpenGL.GL import *
import ShaderLoader
import GLObj

import ipc_msgq

import params


running = [True]

# Window
window = None
w_width = 1920
w_height = 1080

# Model Status
model_no = [0]
model_zoom = [0.5]  # 0 ~ 1
model_rot = [0.0, 0.0, 0.0]


def deg2rad(deg):
    return params.PI / 180 * deg


def window_resize(window, width, height):
    w_width = width
    w_height = height
    glViewport(0, 0, width, height)


def key_event(window, key, scancode, action, mods):
    if action != glfw.PRESS:
        return
    
    # Quit Program
    if key == glfw.KEY_ESCAPE:
        global running
        running[0] = False
        return
    
    # Model Zoom
    global model_zoom
    if key == glfw.KEY_Z:
        model_zoom = max(0.0, model_zoom - 0.1)
        return
    elif key == glfw.KEY_X:
        model_zoom = min(1.0, model_zoom + 0.1)
        return
    
    # Model Rotate
    global model_rot
    rot_rad = 0.2
    rot_vals = {
        glfw.KEY_Q: [0, 1], glfw.KEY_A: [0, -1],  # X
        glfw.KEY_W: [1, 1], glfw.KEY_S: [1, -1],  # Y
        glfw.KEY_E: [2, 1], glfw.KEY_D: [2, -1]  # Z
    }
    if key in rot_vals.keys():
        rot_val = rot_vals[key]
        model_rot[rot_val[0]] += rot_val[1] * rot_rad
        return


def init_glfw():
    if not glfw.init():
        print('[Error] glfw.init()')
        return None

    monitor = get_monitors()[0]
    fullscreen = None
    global w_width, w_height
    if params.full_screen is True:
        w_width = monitor.width
        w_height = monitor.height
        fullscreen = glfw.get_primary_monitor()
    else:
        w_width = params.window_size[0] if params.window_size[0] is not None else monitor.width
        w_height = params.window_size[1] if params.window_size[1] is not None else monitor.height
    
    window = glfw.create_window(w_width, w_height, params.title, fullscreen, None)

    if not window:
        glfw.terminate()
        print('[Error] glfw.create_window()')
        return None

    glfw.make_context_current(window)
    glfw.set_window_size_callback(window, window_resize)
    
    glfw.window_hint(glfw.DECORATED, 0)
    glfw.set_input_mode(window, glfw.STICKY_KEYS, GL_TRUE)
    glfw.set_key_callback(window, key_event)
    
    return window


def main():
    # Init GLFW
    window = init_glfw()
    if window is None:
        return
    
    # Init GL
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glEnable(GL_DEPTH_TEST)
    
    # Compile Shader
    shader = ShaderLoader.compile_shader(params.vert_shader, params.frag_shader)
    glUseProgram(shader)

    # Load Initial Model
    obj, texture = GLObj.load_object(params.models[model_no[0]][0])
    
    # Run IPC Thread
    ipc_th = threading.Thread(target=ipc_msgq.ipc_read_th,
                              args=[running, model_no, model_zoom, model_rot])
    ipc_th.daemon = True
    ipc_th.start()

    curr_model_no = model_no[0]
    while running[0] is True:
        if curr_model_no != model_no[0]: # Model Is Changed
            obj, texture = GLObj.load_object(params.models[model_no[0]][0])
            curr_model_no = model_no[0]
        
        glfw.poll_events()
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        
        depth = params.min_depth + model_zoom[0] * (params.max_depth - params.min_depth)
        depth *= params.models[model_no[0]][1]  # Depth Coefficient
        proj_dist = depth * math.cos(deg2rad(params.elev_angle))
        alt = depth * math.sin(deg2rad(params.elev_angle))
        
        pov = w_width / w_height
        
        # Top
        GLObj.draw_obj([0, alt, -proj_dist],
                       [0, 0, 0],
                       model_rot,
                       pov, obj, shader)
        
        # Left
        GLObj.draw_obj([-alt, 0, -proj_dist],
                       [-params.PI / 2, params.PI, params.PI / 2],
                       model_rot,
                       pov, obj, shader)
        
        # Right
        GLObj.draw_obj([alt, 0, -proj_dist],
                       [params.PI / 2, 0, params.PI / 2],
                       model_rot,
                       pov, obj, shader)
        
        # Bottom
        GLObj.draw_obj([0, -alt, -proj_dist],
                       [params.PI, 0, 0],
                       model_rot,
                       pov, obj, shader)       

        glfw.swap_buffers(window)
        time.sleep(1 / params.max_fps)
        
    glfw.terminate()


if __name__ == "__main__":
    main()
