import glfw
from OpenGL.GL import *
import ShaderLoader
import numpy
import pyrr
from PIL import Image
from ObjLoader import *
from screeninfo import get_monitors


running = True


def window_resize(window, width, height):
    glViewport(0, 0, width, height)


def key_event(window, key, scancode, action, mods):
    if action == glfw.PRESS and key == glfw.KEY_ESCAPE:
        global running
        running = False


def main():
    # initialize glfw
    if not glfw.init():
        return

    monitor = get_monitors()[0]
    window = glfw.create_window(monitor.width, monitor.height, "My OpenGL window", glfw.get_primary_monitor(), None)

    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    glfw.set_window_size_callback(window, window_resize)

    obj = ObjLoader()
    obj.load_model("res/cube.obj")

    texture_offset = len(obj.vertex_index)*12


    shader = ShaderLoader.compile_shader("shaders/vert.vs", "shaders/frag.fs")

    VBO = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, VBO)
    glBufferData(GL_ARRAY_BUFFER, obj.model.itemsize * len(obj.model), obj.model, GL_STATIC_DRAW)

    #position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, obj.model.itemsize * 3, ctypes.c_void_p(0))
    glEnableVertexAttribArray(0)
    #texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, obj.model.itemsize * 2, ctypes.c_void_p(texture_offset))
    glEnableVertexAttribArray(1)


    texture = glGenTextures(1)
    glBindTexture(GL_TEXTURE_2D, texture)
    # Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
    # Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    # load image
    image = Image.open("res/cube_texture.jpg")
    flipped_image = image.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
    img_data = numpy.array(list(flipped_image.getdata()), numpy.uint8)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data)
    glEnable(GL_TEXTURE_2D)


    glUseProgram(shader)

    glClearColor(0.0, 0.0, 0.0, 1.0)
    glEnable(GL_DEPTH_TEST)

    glfw.set_input_mode(window, glfw.STICKY_KEYS, GL_TRUE)
    glfw.set_key_callback(window, key_event)

    global running
    while running is True:
        glfw.poll_events()

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        view = pyrr.matrix44.create_from_translation(pyrr.Vector3([0.0, 0.0, -5]))
        projection = pyrr.matrix44.create_perspective_projection_matrix(65.0, monitor.width / monitor.height, 0.1, 100.0)
        model = pyrr.matrix44.create_from_translation(pyrr.Vector3([0.0, 0.0, 0.0]))

        view_loc = glGetUniformLocation(shader, "view")
        proj_loc = glGetUniformLocation(shader, "projection")
        model_loc = glGetUniformLocation(shader, "model")

        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view)
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection)
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)

        rot_x = pyrr.Matrix44.from_x_rotation(0.0 * glfw.get_time()) #0.5 * glfw.get_time() )
        rot_y = pyrr.Matrix44.from_y_rotation(0.0 * glfw.get_time()) #0.8 * glfw.get_time() )

        transformLoc = glGetUniformLocation(shader, "transform")
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, rot_x * rot_y)

        glDrawArrays(GL_TRIANGLES, 0, len(obj.vertex_index))

        #==========================================================================

        view = pyrr.matrix44.create_from_translation(pyrr.Vector3([0.0, 0.0, -5]))
        projection = pyrr.matrix44.create_perspective_projection_matrix(65.0, monitor.width / monitor.height, 0.1, 100.0)
        model = pyrr.matrix44.create_from_translation(pyrr.Vector3([1.0, 3.0, 0.0]))

        view_loc = glGetUniformLocation(shader, "view")
        proj_loc = glGetUniformLocation(shader, "projection")
        model_loc = glGetUniformLocation(shader, "model")

        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view)
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection)
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)

        rot_x = pyrr.Matrix44.from_x_rotation(0.0 * glfw.get_time()) #0.5 * glfw.get_time() )
        rot_y = pyrr.Matrix44.from_y_rotation(0.0 * glfw.get_time()) #0.8 * glfw.get_time() )

        transformLoc = glGetUniformLocation(shader, "transform")
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, rot_x * rot_y)

        glDrawArrays(GL_TRIANGLES, 0, len(obj.vertex_index))

        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()
