本Demo为使用OpenGL ES Shader将yuv数据转换为rgb并显示
需要传入y数据和uv数据

其中assets下的data.log文件包含为960*720的yuv数据，数据格式为NV21，即w*h的y数据＋w*h/2的vu交错数据，其中v在前，u在后，具体可参考NV21数据格式