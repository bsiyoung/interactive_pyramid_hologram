import posix_ipc as pi

import params

TYPE_QUIT = 0
TYPE_MODEL_NO = 1
TYPE_MODEL_ZOOM = 2
TYPE_MODEL_ROT_YAW = 3
TYPE_MODEL_ROT_PITCH = 4
TYPE_MODEL_ROT_ROLL = 5

def ipc_read_th(running, model_no, model_zoom, model_rot):
    mq = pi.MessageQueue('/holo_ipc', flags=pi.O_CREAT, read=True)
    
    while running[0] is True:
        data_type = int.from_bytes(mq.receive()[0], byteorder=params.byteorder)
        data = int.from_bytes(mq.receive()[0], byteorder=params.byteorder)
        
        # Not Sure
        '''
        if params.byteorder == 'big':
            tmp = data_type
            data_type = data
            data = tmp
        '''
            
        print('[IPC Read] {}, {}'.format(data_type, data))
        
        if data_type == TYPE_QUIT:
            running[0] = False
        elif data_type == TYPE_MODEL_NO:
            model_no[0] = data
        elif data_type == TYPE_MODEL_ZOOM:
            model_zoom[0] = max(0.0, min(1.0, data / 100))
        elif data_type == TYPE_MODEL_ROT_ROLL:
            model_rot[0] = data / 100
        elif data_type == TYPE_MODEL_ROT_PITCH:
            model_rot[1] = data / 100
        elif data_type == TYPE_MODEL_ROT_YAW:
            model_rot[2] = data / 100
