from OpenGL.GL import *

from PIL import Image
import numpy as np
import pyrr

import params

class ObjLoader:
    def __init__(self):
        self.vert_coords = []
        self.text_coords = []
        self.norm_coords = []

        self.vertex_index = []
        self.texture_index = []
        self.normal_index = []

        self.model = []

    def load_model(self, file):
        for line in open(file, 'r'):
            if line.startswith('#'): continue
            values = line.split()
            if not values: continue

            if values[0] == 'v':
                self.vert_coords.append(values[1:4])
            if values[0] == 'vt':
                self.text_coords.append(values[1:3])
            if values[0] == 'vn':
                self.norm_coords.append(values[1:4])

            if values[0] == 'f':
                face_i = []
                text_i = []
                norm_i = []
                for v in values[1:4]:
                    w = v.split('/')
                    face_i.append(int(w[0])-1)
                    text_i.append(int(w[1])-1)
                    norm_i.append(int(w[2])-1)
                self.vertex_index.append(face_i)
                self.texture_index.append(text_i)
                self.normal_index.append(norm_i)

        self.vertex_index = [y for x in self.vertex_index for y in x]
        self.texture_index = [y for x in self.texture_index for y in x]
        self.normal_index = [y for x in self.normal_index for y in x]

        for i in self.vertex_index:
            self.model.extend(self.vert_coords[i])

        for i in self.texture_index:
            self.model.extend(self.text_coords[i])

        for i in self.normal_index:
            self.model.extend(self.norm_coords[i])

        self.model = np.array(self.model, dtype='float32')


def load_object(obj_name):
    path = params.model_path + '/' + obj_name + '/'
    obj_path = path + 'model.obj'
    texture_path = path + 'texture.jpg'
    
    obj = ObjLoader()
    obj.load_model(obj_path)

    texture_offset = len(obj.vertex_index)*12

    VBO = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, VBO)
    glBufferData(GL_ARRAY_BUFFER, obj.model.itemsize * len(obj.model), obj.model, GL_STATIC_DRAW)

    #Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, obj.model.itemsize * 3, ctypes.c_void_p(0))
    glEnableVertexAttribArray(0)

    #Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, obj.model.itemsize * 2, ctypes.c_void_p(texture_offset))
    glEnableVertexAttribArray(1)

    texture = glGenTextures(1)
    glBindTexture(GL_TEXTURE_2D, texture)

    # Set Texture Params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

    # Load Texture Image
    try:
        image = Image.open(texture_path)
        flipped_image = image.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
        img_data = np.array(list(flipped_image.getdata()), np.uint8)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data)

        glEnable(GL_TEXTURE_2D)
    except Exception as e:
        return obj, None

    return obj, texture


def draw_obj(view_pos, rot_bias, obj_rotate, pov, obj, shader):
        view = pyrr.matrix44.create_from_translation(pyrr.Vector3(view_pos))
        projection = pyrr.matrix44.create_perspective_projection_matrix(params.view_angle,
                                                                        pov,
                                                                        params.view_min_depth,
                                                                        params.view_max_depth)
        obj_pos = [0, 0, 0]
        model = pyrr.matrix44.create_from_translation(pyrr.Vector3(obj_pos))

        view_loc = glGetUniformLocation(shader, "view")
        proj_loc = glGetUniformLocation(shader, "projection")
        model_loc = glGetUniformLocation(shader, "model")

        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view)
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection)
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
        
        b_rot_x = pyrr.Matrix44.from_x_rotation(rot_bias[0])
        b_rot_y = pyrr.Matrix44.from_y_rotation(rot_bias[1])
        b_rot_z = pyrr.Matrix44.from_z_rotation(rot_bias[2])
        b_rot = b_rot_x * b_rot_y * b_rot_z

        rot_x = pyrr.Matrix44.from_x_rotation(obj_rotate[0])
        rot_y = pyrr.Matrix44.from_y_rotation(obj_rotate[1])
        rot_z = pyrr.Matrix44.from_z_rotation(obj_rotate[2])
        rot = rot_x * rot_y * rot_z

        transformLoc = glGetUniformLocation(shader, "transform")
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, b_rot * rot)

        glDrawArrays(GL_TRIANGLES, 0, len(obj.vertex_index))
