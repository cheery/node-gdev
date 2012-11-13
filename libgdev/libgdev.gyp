{
    'targets': [
        {
            'target_name': 'libgdev',
            'type': 'shared_library',
            'include_dirs': [
                'include',
                '/opt/vc/include',
                '/opt/vc/include/interface/vcos/pthreads'
            ],
            'defines': [
                'EGL_EGLEXT_PROTOTYPES',
                'GL_GLEXT_PROTOTYPES',
            ],
            'ldflags': [
                '-L/opt/vc/lib',
                '-lEGL',
                '-lGLESv2',
                '-lbcm_host',
                '-lvcos',
                '-lvchiq_arm',
            ],
            'sources': [
                'src/display.c',
                'src/gdev.c',
            ],
        },
        {
            'target_name': 'demo',
            'dependencies': [
                'libgdev',
            ],
            'type': 'executable',
            'include_dirs': [
                'include',
                '/opt/vc/include',
            ],
            'linkflags': [
                'libgdev',
            ],
            'ldflags': [
                '-L/opt/vc/lib',
                '-lGLESv2',
            ],
            'sources': [
                'demo.c',
            ]
        },
    ],
}
