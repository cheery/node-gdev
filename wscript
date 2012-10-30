# video/wscript
import Options

def set_options(opt):
    opt.tool_options("compiler_cxx")

def configure(conf):
    conf.check_tool("compiler_cxx")
    conf.check_tool("node_addon")

def build(bld):
    obj = bld.new_task_gen("cxx", "shlib", "node_addon")
    obj.cxxflags = [
        "-I/opt/vc/include/",
        "-I/home/pi/node-video/rpi/include/",
#        "-I/opt/vc/include/",
#        "-I/opt/vc/include/interface/vcos/pthreads",
#        "-DSTANDALONE",
#        "-D__STDC_CONSTANT_MACROS",
#        "-D__STDC_LIMIT_MACROS",
#        "-DTARGET_POSIX",
#        "-D_LINUX",
#        "-fPIC",
#        "-DPIC",
#        "-D_REENTRANT",
#        "-D_LARGEFILE64_SOURCE",
#        "-D_FILE_OFFSET_BITS=64",
#        "-U_FORTIFY_SOURCE",
#        "-Wall",
#        "-g",
#        "-DHAVE_LIBOPENMAX=2",
#        "-DOMX",
#        "-DOMX_SKIP64BIT",
#        "-ftree-vectorize",
#        "-pipe",
#        "-DUSE_EXTERNAL_OMX",
#        "-DHAVE_LIBBCM_HOST",
#        "-DUSE_EXTERNAL_LIBBCM_HOST",
#        "-DUSE_VCHIQ_ARM",
#        "-Wno-psabi",
    ]
    obj.linkflags = [
        "-L/opt/vc/lib/",
#        "-lEGL",
        "-lGLESv2",
#        "-lbcm_host",
#        "-lvcos",
#        "-lvchiq_arm",
        "/home/pi/node-video/rpi/libvideo.so",
    ]
    obj.target = "video"
    obj.source = "src/init.cc src/webgl/renderer.cc src/webgl/object.cc"
