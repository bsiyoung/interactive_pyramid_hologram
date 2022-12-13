import glfw
from OpenGL.GL import *

import ShaderLoader
from ObjLoader import load_object

import pyrr
import math
import time

from screeninfo import get_monitors

import params


running = True

# Window Size
w_width = 1920
w_height = 1080

# Model Status
model_no = 0
model_zoom = 0.0
model_rotate = [0.0, 0.0, 0.0]

# Bias


def window_resize(window, width, height):
    glViewport(0, 0, width, height)


def key_event(window, key, scancode, action, mods):
    if action == glfw.PRESS and key == glfw.KEY_ESCAPE:
        global running
        running = False


def draw_model(view_pos, obj_pos, obj_rotate, obj, shader):
        view = pyrr.matrix44.create_from_translation(pyrr.Vector3(view_pos))
        projection = pyrr.matrix44.create_perspective_projection_matrix(params.view_angle,
                                                                        w_width / w_height,
                                                                        params.view_min_depth,
                                                                        params.view_max_depth)
        model = pyrr.matrix44.create_from_translation(pyrr.Vector3(obj_pos))

        view_loc = glGetUniformLocation(shader, "view")
        proj_loc = glGetUniformLocation(shader, "projection")
        model_loc = glGetUniformLocation(shader, "model")

        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view)
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection)
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)

        rot_x = pyrr.Matrix44.from_x_rotation(obj_rotate[0])
        rot_y = pyrr.Matrix44.from_y_rotation(obj_rotate[1])
        rot_z = pyrr.Matrix44.from_z_rotation(obj_rotate[2])

        transformLoc = glGetUniformLocation(shader, "transform")
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, rot_x * rot_y * rot_z)

        glDrawArrays(GL_TRIANGLES, 0, len(obj.vertex_index))



def main():
    # Init Window
    if not glfw.init():
        return

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
    
    print(w_width, w_height)
    
    window = glfw.create_window(w_width, w_height, params.title, fullscreen, None)

    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    # glfw.set_window_size_callback(window, window_resize)
    
    glfw.set_input_mode(window, glfw.STICKY_KEYS, GL_TRUE)
    glfw.set_key_callback(window, key_event)

    # Compile Shader
    shader = ShaderLoader.compile_shader(params.vert_shader, params.frag_shader)
    glUseProgram(shader)

    # Init GL
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glEnable(GL_DEPTH_TEST)

    obj, texture = load_object(params.models[model_no])

    global running
    while running is True:
        glfw.poll_events()
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        # Center
        draw_model([0, 0, -8], [0, 3, 0], [0, 0, 0], obj, shader)
        
        # Left
        draw_model([0, 0, -8], [-5, -2.5, 0], [0, 3.14159265 / 2 + 0.1 * glfw.get_time(), 0], obj, shader)
        
        # Right
        draw_model([0, 0, -8], [5, -2.5, 0], [0, 3.14159265 / 2 + 0.1 * glfw.get_time(), 0], obj, shader)

        glfw.swap_buffers(window)
        time.sleep(1 / 50)
        

    glfw.terminate()

if __name__ == "__main__":
    main()
