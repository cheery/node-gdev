/*
Copyright (c) 2012 Cheery <cheery@boxbase.org>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include "GLES2/gl2.h"
#include "../videocontext.h"
#include "object.h"
#include "renderer.h"

#define SET_PROTOTYPE_CONSTANT(tpl, name, value) \
    tpl->PrototypeTemplate()->Set(String::NewSymbol(name), \
        Integer::New(value));

#define ArrayData(obj) \
    (obj->GetIndexedPropertiesExternalArrayData())

#define ArraySize(obj) \
    (obj->GetIndexedPropertiesExternalArrayDataLength() * Sizeof(obj))

namespace webgl {

    Persistent<Function> Renderer::constructor;

    void Renderer::Init(Handle<Object> target) {
        Local<FunctionTemplate> t = FunctionTemplate::New(New);
        t->SetClassName(String::NewSymbol("WebGLRenderer"));
        t->InstanceTemplate()->SetInternalFieldCount(1);
        NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
        NODE_SET_PROTOTYPE_METHOD(t, "swapBuffers", SwapBuffers);
        ////Constants
        //
        SET_PROTOTYPE_CONSTANT(t, "COLOR_BUFFER_BIT", GL_COLOR_BUFFER_BIT);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_BUFFER_BIT", GL_DEPTH_BUFFER_BIT);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BUFFER_BIT", GL_STENCIL_BUFFER_BIT);
        SET_PROTOTYPE_CONSTANT(t, "POINTS", GL_POINTS);
        SET_PROTOTYPE_CONSTANT(t, "LINES", GL_LINES);
        SET_PROTOTYPE_CONSTANT(t, "LINE_LOOP", GL_LINE_LOOP);
        SET_PROTOTYPE_CONSTANT(t, "LINE_STRIP", GL_LINE_STRIP);
        SET_PROTOTYPE_CONSTANT(t, "TRIANGLES", GL_TRIANGLES);
        SET_PROTOTYPE_CONSTANT(t, "TRIANGLE_STRIP", GL_TRIANGLE_STRIP);
        SET_PROTOTYPE_CONSTANT(t, "TRIANGLE_FAN", GL_TRIANGLE_FAN);
        SET_PROTOTYPE_CONSTANT(t, "ZERO", GL_ZERO);
        SET_PROTOTYPE_CONSTANT(t, "ONE", GL_ONE);
        SET_PROTOTYPE_CONSTANT(t, "SRC_COLOR", GL_SRC_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "ONE_MINUS_SRC_COLOR", GL_ONE_MINUS_SRC_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "SRC_ALPHA", GL_SRC_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "SRC_ONE_MINUS_ALPHA", GL_ONE_MINUS_SRC_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "DST_ALPHA", GL_DST_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "DST_ONE_MINUS_ALPHA", GL_ONE_MINUS_DST_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "DST_COLOR", GL_DST_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "ONE_MINUS_DST_COLOR", GL_ONE_MINUS_DST_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "SRC_ALPHA_SATURATE", GL_SRC_ALPHA_SATURATE);
        SET_PROTOTYPE_CONSTANT(t, "FUNC_ADD", GL_FUNC_ADD);
        SET_PROTOTYPE_CONSTANT(t, "BLEND_EQUATION", GL_BLEND_EQUATION);
        SET_PROTOTYPE_CONSTANT(t, "BLEND_EQUATION_RGB", GL_BLEND_EQUATION_RGB);
        SET_PROTOTYPE_CONSTANT(t, "BLEND_EQUATION_ALPHA", GL_BLEND_EQUATION_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "FUNC_SUBTRACT", GL_FUNC_SUBTRACT);
        SET_PROTOTYPE_CONSTANT(t, "FUNC_REVERSE_SUBTRACT", GL_FUNC_REVERSE_SUBTRACT);
        SET_PROTOTYPE_CONSTANT(t, "BLEND_DST_RGB", GL_BLEND_DST_RGB);
        SET_PROTOTYPE_CONSTANT(t, "BLEND_SRC_RGB", GL_BLEND_SRC_RGB);
        SET_PROTOTYPE_CONSTANT(t, "BLEND_DST_ALPHA", GL_BLEND_DST_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "BLEND_SRC_ALPHA", GL_BLEND_SRC_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "CONSTANT_COLOR", GL_CONSTANT_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "ONE_MINUS_CONSTANT_COLOR", GL_ONE_MINUS_CONSTANT_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "CONSTANT_ALPHA", GL_CONSTANT_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "ONE_MINUS_CONSTANT_ALPHA", GL_ONE_MINUS_CONSTANT_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "BLEND_COLOR", GL_BLEND_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "ARRAY_BUFFER", GL_ARRAY_BUFFER);
        SET_PROTOTYPE_CONSTANT(t, "ELEMENT_ARRAY_BUFFER", GL_ELEMENT_ARRAY_BUFFER);
        SET_PROTOTYPE_CONSTANT(t, "ARRAY_BUFFER_BINDING", GL_ARRAY_BUFFER_BINDING);
        SET_PROTOTYPE_CONSTANT(t, "ELEMENT_ARRAY_BUFFER_BINDING", GL_ELEMENT_ARRAY_BUFFER_BINDING);
        SET_PROTOTYPE_CONSTANT(t, "STREAM_DRAW", GL_STREAM_DRAW);
        SET_PROTOTYPE_CONSTANT(t, "STATIC_DRAW", GL_STATIC_DRAW);
        SET_PROTOTYPE_CONSTANT(t, "DYNAMIC_DRAW", GL_DYNAMIC_DRAW);
        SET_PROTOTYPE_CONSTANT(t, "BUFFER_SIZE", GL_BUFFER_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "BUFFER_USAGE", GL_BUFFER_USAGE);
        SET_PROTOTYPE_CONSTANT(t, "CURRENT_VERTEX_ATTRIB", GL_CURRENT_VERTEX_ATTRIB);
        SET_PROTOTYPE_CONSTANT(t, "FRONT", GL_FRONT);
        SET_PROTOTYPE_CONSTANT(t, "BACK", GL_BACK);
        SET_PROTOTYPE_CONSTANT(t, "FRONT_AND_BACK", GL_FRONT_AND_BACK);
        SET_PROTOTYPE_CONSTANT(t, "CULL_FACE", GL_CULL_FACE);
        SET_PROTOTYPE_CONSTANT(t, "BLEND", GL_BLEND);
        SET_PROTOTYPE_CONSTANT(t, "DITHER", GL_DITHER);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_TEST", GL_STENCIL_TEST);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_TEST", GL_DEPTH_TEST);
        SET_PROTOTYPE_CONSTANT(t, "SCISSOR_TEST", GL_SCISSOR_TEST);
        SET_PROTOTYPE_CONSTANT(t, "POLYGON_OFFSET_FILL", GL_POLYGON_OFFSET_FILL);
        SET_PROTOTYPE_CONSTANT(t, "SAMPLE_ALPHA_TO_COVERAGE", GL_SAMPLE_ALPHA_TO_COVERAGE);
        SET_PROTOTYPE_CONSTANT(t, "SAMPLE_COVERAGE", GL_SAMPLE_COVERAGE);
        SET_PROTOTYPE_CONSTANT(t, "NO_ERROR", GL_INVALID_ENUM);
        SET_PROTOTYPE_CONSTANT(t, "INVALID_ENUM", GL_INVALID_ENUM);
        SET_PROTOTYPE_CONSTANT(t, "INVALID_VALUE", GL_INVALID_VALUE);
        SET_PROTOTYPE_CONSTANT(t, "INVALID_OPERATION", GL_INVALID_OPERATION);
        SET_PROTOTYPE_CONSTANT(t, "OUT_OF_MEMORY", GL_OUT_OF_MEMORY);
        SET_PROTOTYPE_CONSTANT(t, "CW", GL_CW);
        SET_PROTOTYPE_CONSTANT(t, "CCW", GL_CCW);
        SET_PROTOTYPE_CONSTANT(t, "LINE_WIDTH", GL_LINE_WIDTH);
        SET_PROTOTYPE_CONSTANT(t, "ALIASED_POINT_SIZE_RANGE", GL_ALIASED_POINT_SIZE_RANGE);
        SET_PROTOTYPE_CONSTANT(t, "ALIASED_LINE_WIDTH_RANGE", GL_ALIASED_LINE_WIDTH_RANGE);
        SET_PROTOTYPE_CONSTANT(t, "CULL_FACE_MODE", GL_CULL_FACE_MODE);
        SET_PROTOTYPE_CONSTANT(t, "FRONT_FACE", GL_FRONT_FACE);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_RANGE", GL_DEPTH_RANGE);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_WRITEMASK", GL_DEPTH_WRITEMASK);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_CLEAR_VALUE", GL_DEPTH_CLEAR_VALUE);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_FUNC", GL_DEPTH_FUNC);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_CLEAR_VALUE", GL_STENCIL_CLEAR_VALUE);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_FUNC", GL_STENCIL_FUNC);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_FAIL", GL_STENCIL_FAIL);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_PASS_DEPTH_FAIL", GL_STENCIL_PASS_DEPTH_FAIL);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_PASS_DEPTH_PASS", GL_STENCIL_PASS_DEPTH_PASS);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_REF", GL_STENCIL_REF);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_VALUE_MASK", GL_STENCIL_VALUE_MASK);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_WRITEMASK", GL_STENCIL_WRITEMASK);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BACK_FUNC", GL_STENCIL_BACK_FUNC);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BACK_FAIL", GL_STENCIL_BACK_FAIL);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BACK_PASS_DEPTH_FAIL", GL_STENCIL_BACK_PASS_DEPTH_FAIL);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BACK_PASS_DEPTH_PASS", GL_STENCIL_BACK_PASS_DEPTH_PASS);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BACK_REF", GL_STENCIL_BACK_REF);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BACK_VALUE_MASK", GL_STENCIL_BACK_VALUE_MASK);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BACK_WRITEMASK", GL_STENCIL_BACK_WRITEMASK);
        SET_PROTOTYPE_CONSTANT(t, "VIEWPORT", GL_VIEWPORT);
        SET_PROTOTYPE_CONSTANT(t, "SCISSOR_BOX", GL_SCISSOR_BOX);
        SET_PROTOTYPE_CONSTANT(t, "COLOR_CLEAR_VALUE", GL_COLOR_CLEAR_VALUE);
        SET_PROTOTYPE_CONSTANT(t, "COLOR_WRITEMASK", GL_COLOR_WRITEMASK);
        SET_PROTOTYPE_CONSTANT(t, "UNPACK_ALIGNMENT", GL_UNPACK_ALIGNMENT);
        SET_PROTOTYPE_CONSTANT(t, "PACK_ALIGNMENT", GL_PACK_ALIGNMENT);
        SET_PROTOTYPE_CONSTANT(t, "MAX_TEXTURE_SIZE", GL_MAX_TEXTURE_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "MAX_VIEWPORT_DIMS", GL_MAX_VIEWPORT_DIMS);
        SET_PROTOTYPE_CONSTANT(t, "SUBPIXEL_BITS", GL_SUBPIXEL_BITS);
        SET_PROTOTYPE_CONSTANT(t, "RED_BITS", GL_RED_BITS);
        SET_PROTOTYPE_CONSTANT(t, "GREEN_BITS", GL_GREEN_BITS);
        SET_PROTOTYPE_CONSTANT(t, "BLUE_BITS", GL_BLUE_BITS);
        SET_PROTOTYPE_CONSTANT(t, "ALPHA_BITS", GL_ALPHA_BITS);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_BITS", GL_DEPTH_BITS);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BITS", GL_STENCIL_BITS);
        SET_PROTOTYPE_CONSTANT(t, "POLYGON_OFFSET_UNITS", GL_POLYGON_OFFSET_UNITS);
        SET_PROTOTYPE_CONSTANT(t, "POLYGON_OFFSET_FACTOR", GL_POLYGON_OFFSET_FACTOR);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_BINDING_2D", GL_TEXTURE_BINDING_2D);
        SET_PROTOTYPE_CONSTANT(t, "SAMPLE_BUFFERS", GL_SAMPLE_BUFFERS);
        SET_PROTOTYPE_CONSTANT(t, "SAMPLES", GL_SAMPLES);
        SET_PROTOTYPE_CONSTANT(t, "SAMPLE_COVERAGE_VALUE", GL_SAMPLE_COVERAGE_VALUE);
        SET_PROTOTYPE_CONSTANT(t, "SAMPLE_COVERAGE_INVERT", GL_SAMPLE_COVERAGE_INVERT);
        SET_PROTOTYPE_CONSTANT(t, "NUM_COMPRESSED_TEXTURE_FORMATS", GL_NUM_COMPRESSED_TEXTURE_FORMATS);
        SET_PROTOTYPE_CONSTANT(t, "COMPRESSED_TEXTURE_FORMATS", GL_COMPRESSED_TEXTURE_FORMATS);
        SET_PROTOTYPE_CONSTANT(t, "DONT_CARE", GL_DONT_CARE);
        SET_PROTOTYPE_CONSTANT(t, "FASTEST", GL_FASTEST);
        SET_PROTOTYPE_CONSTANT(t, "NICEST", GL_NICEST);
        SET_PROTOTYPE_CONSTANT(t, "GENERATE_MIPMAP_HINT", GL_GENERATE_MIPMAP_HINT);
        SET_PROTOTYPE_CONSTANT(t, "BYTE", GL_BYTE);
        SET_PROTOTYPE_CONSTANT(t, "UNSIGNED_BYTE", GL_UNSIGNED_BYTE);
        SET_PROTOTYPE_CONSTANT(t, "SHORT", GL_SHORT);
        SET_PROTOTYPE_CONSTANT(t, "UNSIGNED_SHORT", GL_UNSIGNED_SHORT);
        SET_PROTOTYPE_CONSTANT(t, "INT", GL_INT);
        SET_PROTOTYPE_CONSTANT(t, "UNSIGNED_INT", GL_UNSIGNED_INT);
        SET_PROTOTYPE_CONSTANT(t, "FLOAT", GL_FLOAT);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_COMPONENT", GL_DEPTH_COMPONENT);
        SET_PROTOTYPE_CONSTANT(t, "ALPHA", GL_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "RGB", GL_RGB);
        SET_PROTOTYPE_CONSTANT(t, "RGBA", GL_RGBA);
        SET_PROTOTYPE_CONSTANT(t, "LUMINANCE", GL_LUMINANCE);
        SET_PROTOTYPE_CONSTANT(t, "LUMINANCE_ALPHA", GL_LUMINANCE_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "UNSIGNED_SHORT_4_4_4_4", GL_UNSIGNED_SHORT_4_4_4_4);
        SET_PROTOTYPE_CONSTANT(t, "UNSIGNED_SHORT_5_5_5_1", GL_UNSIGNED_SHORT_5_5_5_1);
        SET_PROTOTYPE_CONSTANT(t, "UNSIGNED_SHORT_5_6_5", GL_UNSIGNED_SHORT_5_6_5);
        SET_PROTOTYPE_CONSTANT(t, "FRAGMENT_SHADER", GL_FRAGMENT_SHADER);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_SHADER", GL_VERTEX_SHADER);
        SET_PROTOTYPE_CONSTANT(t, "MAX_VERTEX_ATTRIBS", GL_MAX_VERTEX_ATTRIBS);
        SET_PROTOTYPE_CONSTANT(t, "MAX_VERTEX_UNIFORM_VECTORS", GL_MAX_VERTEX_UNIFORM_VECTORS);
        SET_PROTOTYPE_CONSTANT(t, "MAX_VARYING_VECTORS", GL_MAX_VARYING_VECTORS);
        SET_PROTOTYPE_CONSTANT(t, "MAX_COMBINED_TEXTURE_IMAGE_UNITS", GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
        SET_PROTOTYPE_CONSTANT(t, "MAX_VERTEX_TEXTURE_IMAGE_UNITS", GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
        SET_PROTOTYPE_CONSTANT(t, "MAX_TEXTURE_IMAGE_UNITS", GL_MAX_TEXTURE_IMAGE_UNITS);
        SET_PROTOTYPE_CONSTANT(t, "MAX_FRAGMENT_UNIFORM_VECTORS", GL_MAX_FRAGMENT_UNIFORM_VECTORS);
        SET_PROTOTYPE_CONSTANT(t, "SHADER_TYPE", GL_SHADER_TYPE);
        SET_PROTOTYPE_CONSTANT(t, "DELETE_STATUS", GL_DELETE_STATUS);
        SET_PROTOTYPE_CONSTANT(t, "LINK_STATUS", GL_LINK_STATUS);
        SET_PROTOTYPE_CONSTANT(t, "VALIDATE_STATUS", GL_VALIDATE_STATUS);
        SET_PROTOTYPE_CONSTANT(t, "ATTACHED_SHADERS", GL_ATTACHED_SHADERS);
        SET_PROTOTYPE_CONSTANT(t, "ACTIVE_UNIFORMS", GL_ACTIVE_UNIFORMS);
        SET_PROTOTYPE_CONSTANT(t, "ACTIVE_ATTRIBUTES", GL_ACTIVE_ATTRIBUTES);
        SET_PROTOTYPE_CONSTANT(t, "SHADING_LANGUAGE_VERSION", GL_SHADING_LANGUAGE_VERSION);
        SET_PROTOTYPE_CONSTANT(t, "CURRENT_PROGRAM", GL_CURRENT_PROGRAM);
        SET_PROTOTYPE_CONSTANT(t, "NEVER", GL_NEVER);
        SET_PROTOTYPE_CONSTANT(t, "LESS", GL_LESS);
        SET_PROTOTYPE_CONSTANT(t, "EQUAL", GL_EQUAL);
        SET_PROTOTYPE_CONSTANT(t, "LEQUAL", GL_LEQUAL);
        SET_PROTOTYPE_CONSTANT(t, "GREATER", GL_GREATER);
        SET_PROTOTYPE_CONSTANT(t, "NOTEQUAL", GL_NOTEQUAL);
        SET_PROTOTYPE_CONSTANT(t, "GEQUAL", GL_GEQUAL);
        SET_PROTOTYPE_CONSTANT(t, "ALWAYS", GL_ALWAYS);
        SET_PROTOTYPE_CONSTANT(t, "KEEP", GL_KEEP);
        SET_PROTOTYPE_CONSTANT(t, "REPLACE", GL_REPLACE);
        SET_PROTOTYPE_CONSTANT(t, "INCR", GL_INCR);
        SET_PROTOTYPE_CONSTANT(t, "DECR", GL_DECR);
        SET_PROTOTYPE_CONSTANT(t, "INVERT", GL_INVERT);
        SET_PROTOTYPE_CONSTANT(t, "INCR_WRAP", GL_INCR_WRAP);
        SET_PROTOTYPE_CONSTANT(t, "DECR_WRAP", GL_DECR_WRAP);
        SET_PROTOTYPE_CONSTANT(t, "VENDOR", GL_VENDOR);
        SET_PROTOTYPE_CONSTANT(t, "RENDERER", GL_RENDERER);
        SET_PROTOTYPE_CONSTANT(t, "VERSION", GL_VERSION);
        SET_PROTOTYPE_CONSTANT(t, "NEAREST", GL_NEAREST);
        SET_PROTOTYPE_CONSTANT(t, "LINEAR", GL_LINEAR);
        SET_PROTOTYPE_CONSTANT(t, "NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST);
        SET_PROTOTYPE_CONSTANT(t, "LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST);
        SET_PROTOTYPE_CONSTANT(t, "NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR);
        SET_PROTOTYPE_CONSTANT(t, "LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_MAG_FILTER", GL_TEXTURE_MAG_FILTER);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_MIN_FILTER", GL_TEXTURE_MIN_FILTER);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_WRAP_S", GL_TEXTURE_WRAP_S);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_WRAP_T", GL_TEXTURE_WRAP_T);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_2D", GL_TEXTURE_2D);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE", GL_TEXTURE);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_CUBE_MAP", GL_TEXTURE_CUBE_MAP);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_BINDING_CUBE_MAP", GL_TEXTURE_BINDING_CUBE_MAP);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_CUBE_MAP_POSITIVE_X", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_CUBE_MAP_NEGATIVE_X", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_CUBE_MAP_POSITIVE_Y", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_CUBE_MAP_NEGATIVE_Y", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_CUBE_MAP_POSITIVE_Z", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE_CUBE_MAP_NEGATIVE_Z", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
        SET_PROTOTYPE_CONSTANT(t, "MAX_CUBE_MAP_TEXTURE_SIZE", GL_MAX_CUBE_MAP_TEXTURE_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE0", GL_TEXTURE0);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE1", GL_TEXTURE1);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE2", GL_TEXTURE2);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE3", GL_TEXTURE3);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE4", GL_TEXTURE4);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE5", GL_TEXTURE5);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE6", GL_TEXTURE6);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE7", GL_TEXTURE7);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE8", GL_TEXTURE8);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE9", GL_TEXTURE9);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE10", GL_TEXTURE10);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE11", GL_TEXTURE11);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE12", GL_TEXTURE12);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE13", GL_TEXTURE13);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE14", GL_TEXTURE14);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE15", GL_TEXTURE15);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE16", GL_TEXTURE16);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE17", GL_TEXTURE17);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE18", GL_TEXTURE18);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE19", GL_TEXTURE19);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE20", GL_TEXTURE20);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE21", GL_TEXTURE21);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE22", GL_TEXTURE22);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE23", GL_TEXTURE23);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE24", GL_TEXTURE24);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE25", GL_TEXTURE25);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE26", GL_TEXTURE26);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE27", GL_TEXTURE27);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE28", GL_TEXTURE28);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE29", GL_TEXTURE29);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE30", GL_TEXTURE30);
        SET_PROTOTYPE_CONSTANT(t, "TEXTURE31", GL_TEXTURE31);
        SET_PROTOTYPE_CONSTANT(t, "ACTIVE_TEXTURE", GL_ACTIVE_TEXTURE);
        SET_PROTOTYPE_CONSTANT(t, "REPEAT", GL_REPEAT);
        SET_PROTOTYPE_CONSTANT(t, "CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE);
        SET_PROTOTYPE_CONSTANT(t, "MIRRORED_REPEAT", GL_MIRRORED_REPEAT);
        SET_PROTOTYPE_CONSTANT(t, "FLOAT_VEC2", GL_FLOAT_VEC2);
        SET_PROTOTYPE_CONSTANT(t, "FLOAT_VEC3", GL_FLOAT_VEC3);
        SET_PROTOTYPE_CONSTANT(t, "FLOAT_VEC4", GL_FLOAT_VEC4);
        SET_PROTOTYPE_CONSTANT(t, "INT_VEC2", GL_INT_VEC2);
        SET_PROTOTYPE_CONSTANT(t, "INT_VEC3", GL_INT_VEC3);
        SET_PROTOTYPE_CONSTANT(t, "INT_VEC4", GL_INT_VEC4);
        SET_PROTOTYPE_CONSTANT(t, "BOOL", GL_BOOL);
        SET_PROTOTYPE_CONSTANT(t, "BOOL_VEC2", GL_BOOL_VEC2);
        SET_PROTOTYPE_CONSTANT(t, "BOOL_VEC3", GL_BOOL_VEC3);
        SET_PROTOTYPE_CONSTANT(t, "BOOL_VEC4", GL_BOOL_VEC4);
        SET_PROTOTYPE_CONSTANT(t, "FLOAT_MAT2", GL_FLOAT_MAT2);
        SET_PROTOTYPE_CONSTANT(t, "FLOAT_MAT3", GL_FLOAT_MAT3);
        SET_PROTOTYPE_CONSTANT(t, "FLOAT_MAT4", GL_FLOAT_MAT4);
        SET_PROTOTYPE_CONSTANT(t, "SAMPLER_2D", GL_SAMPLER_2D);
        SET_PROTOTYPE_CONSTANT(t, "SAMPLER_CUBE", GL_SAMPLER_CUBE);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_ENABLED", GL_VERTEX_ATTRIB_ARRAY_ENABLED);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_SIZE", GL_VERTEX_ATTRIB_ARRAY_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_STRIDE", GL_VERTEX_ATTRIB_ARRAY_STRIDE);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_TYPE", GL_VERTEX_ATTRIB_ARRAY_TYPE);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_NORMALIZED", GL_VERTEX_ATTRIB_ARRAY_NORMALIZED);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_POINTER", GL_VERTEX_ATTRIB_ARRAY_POINTER);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_BUFFER_BINDING", GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING);
        SET_PROTOTYPE_CONSTANT(t, "COMPILE_STATUS", GL_COMPILE_STATUS);
        SET_PROTOTYPE_CONSTANT(t, "LOW_FLOAT", GL_LOW_FLOAT);
        SET_PROTOTYPE_CONSTANT(t, "MEDIUM_FLOAT", GL_MEDIUM_FLOAT);
        SET_PROTOTYPE_CONSTANT(t, "HIGH_FLOAT", GL_HIGH_FLOAT);
        SET_PROTOTYPE_CONSTANT(t, "LOW_INT", GL_LOW_INT);
        SET_PROTOTYPE_CONSTANT(t, "MEDIUM_INT", GL_MEDIUM_INT);
        SET_PROTOTYPE_CONSTANT(t, "HIGH_INT", GL_HIGH_INT);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER", GL_FRAMEBUFFER);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER", GL_RENDERBUFFER);
        SET_PROTOTYPE_CONSTANT(t, "RGBA4", GL_RGBA4);
        SET_PROTOTYPE_CONSTANT(t, "RGB5_A1", GL_RGB5_A1);
        SET_PROTOTYPE_CONSTANT(t, "RGB565", GL_RGB565);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_COMPONENT16", GL_DEPTH_COMPONENT16);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_INDEX", GL_STENCIL_INDEX);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_INDEX8", GL_STENCIL_INDEX8);
        //SET_PROTOTYPE_CONSTANT(t, "DEPTH_STENCIL", GL_DEPTH_STENCIL);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER_WIDTH", GL_RENDERBUFFER_WIDTH);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER_HEIGHT", GL_RENDERBUFFER_HEIGHT);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER_INTERNAL_FORMAT", GL_RENDERBUFFER_INTERNAL_FORMAT);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER_RED_SIZE", GL_RENDERBUFFER_RED_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER_GREEN_SIZE", GL_RENDERBUFFER_GREEN_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER_BLUE_SIZE", GL_RENDERBUFFER_BLUE_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER_ALPHA_SIZE", GL_RENDERBUFFER_ALPHA_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER_DEPTH_SIZE", GL_RENDERBUFFER_DEPTH_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER_STENCIL_SIZE", GL_RENDERBUFFER_STENCIL_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE", GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER_ATTACHMENT_OBJECT_NAME", GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL", GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE", GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE);
        SET_PROTOTYPE_CONSTANT(t, "COLOR_ATTACHMENT0", GL_COLOR_ATTACHMENT0);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_ATTACHMENT", GL_DEPTH_ATTACHMENT);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_ATTACHMENT", GL_STENCIL_ATTACHMENT);
        //SET_PROTOTYPE_CONSTANT(t, "DEPTH_STENCIL_ATTACHMENT", GL_DEPTH_STENCIL_ATTACHMENT);
        SET_PROTOTYPE_CONSTANT(t, "NONE", GL_NONE);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER_COMPLETE", GL_FRAMEBUFFER_COMPLETE);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER_INCOMPLETE_ATTACHMENT", GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT", GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER_INCOMPLETE_DIMENSIONS", GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER_UNSUPPORTED", GL_FRAMEBUFFER_UNSUPPORTED);
        SET_PROTOTYPE_CONSTANT(t, "FRAMEBUFFER_BINDING", GL_FRAMEBUFFER_BINDING);
        SET_PROTOTYPE_CONSTANT(t, "RENDERBUFFER_BINDING", GL_RENDERBUFFER_BINDING);
        SET_PROTOTYPE_CONSTANT(t, "MAX_RENDERBUFFER_SIZE", GL_MAX_RENDERBUFFER_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "INVALID_FRAMEBUFFER_OPERATION", GL_INVALID_FRAMEBUFFER_OPERATION);
        //SET_PROTOTYPE_CONSTANT(t, "UNPACK_FLIP_Y_WEBGL", GL_UNPACK_FLIP_Y_WEBGL);
        //SET_PROTOTYPE_CONSTANT(t, "UNPACK_PREMULTIPLY_ALPHA_WEBGL", GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL);
        //SET_PROTOTYPE_CONSTANT(t, "CONTEXT_LOST_WEBGL", GL_CONTEXT_LOST_WEBGL);
        //SET_PROTOTYPE_CONSTANT(t, "UNPACK_COLORSPACE_CONVERSION_WEBGL", GL_UNPACK_COLORSPACE_CONVERSION_WEBGL);
        //SET_PROTOTYPE_CONSTANT(t, "BROWSER_DEFAULT_WEBGL", GL_BROWSER_DEFAULT_WEBGL);
        ////Per-Fragment Operations
        //
        NODE_SET_PROTOTYPE_METHOD(t, "blendColor", BlendColor);
        NODE_SET_PROTOTYPE_METHOD(t, "blendEquation", BlendEquation);
        NODE_SET_PROTOTYPE_METHOD(t, "blendEquationSeparate", BlendEquationSeparate);
        NODE_SET_PROTOTYPE_METHOD(t, "blendFunc", BlendFunc);
        NODE_SET_PROTOTYPE_METHOD(t, "blendFuncSeparate", BlendFuncSeparate);
        NODE_SET_PROTOTYPE_METHOD(t, "depthFunc", DepthFunc);
        NODE_SET_PROTOTYPE_METHOD(t, "sampleCoverage", SampleCoverage);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilFunc", StencilFunc);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilFuncSeparate", StencilFuncSeparate);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilOp", StencilOp);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilOpSeparate", StencilOpSeparate);
        ////Whole Framebuffer Operations
        //
        NODE_SET_PROTOTYPE_METHOD(t, "clear", Clear);
        NODE_SET_PROTOTYPE_METHOD(t, "clearColor", ClearColor);
        NODE_SET_PROTOTYPE_METHOD(t, "clearDepth", ClearDepth);
        NODE_SET_PROTOTYPE_METHOD(t, "clearStencil", ClearStencil);
        NODE_SET_PROTOTYPE_METHOD(t, "colorMask", ColorMask);
        NODE_SET_PROTOTYPE_METHOD(t, "depthMask", DepthMask);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilMask", StencilMask);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilMaskSeparate", StencilMaskSeparate);

        ////Buffer Objects
        //
        NODE_SET_PROTOTYPE_METHOD(t, "bindBuffer", BindBuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "bufferData", BufferData);
        NODE_SET_PROTOTYPE_METHOD(t, "bufferSubData", BufferSubData);
        NODE_SET_PROTOTYPE_METHOD(t, "createBuffer", CreateBuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteBuffer", DeleteBuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "getBufferParameter", GetBufferParameter);
        NODE_SET_PROTOTYPE_METHOD(t, "isBuffer", IsBuffer);
        ////View and Clip
        //
        NODE_SET_PROTOTYPE_METHOD(t, "depthRange", DepthRange);
        NODE_SET_PROTOTYPE_METHOD(t, "scissor", Scissor);
        NODE_SET_PROTOTYPE_METHOD(t, "viewport", Viewport);
        ////Rasterization                           
        //                                          
        NODE_SET_PROTOTYPE_METHOD(t, "cullFace", CullFace);
        NODE_SET_PROTOTYPE_METHOD(t, "frontFace", FrontFace);
        NODE_SET_PROTOTYPE_METHOD(t, "lineWidth", LineWidth);
        NODE_SET_PROTOTYPE_METHOD(t, "polygonOffset", PolygonOffset);
        ////Detect and Enable Extensions
        //
        ////Detect context lost events
        //
        ////Programs and Shaders
        //
        NODE_SET_PROTOTYPE_METHOD(t, "attachShader", AttachShader);
        NODE_SET_PROTOTYPE_METHOD(t, "bindAttribLocation", BindAttribLocation);
        NODE_SET_PROTOTYPE_METHOD(t, "compileShader", CompileShader);
        NODE_SET_PROTOTYPE_METHOD(t, "createProgram", CreateProgram);
        NODE_SET_PROTOTYPE_METHOD(t, "createShader", CreateShader);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteProgram", DeleteProgram);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteShader", DeleteShader);
        NODE_SET_PROTOTYPE_METHOD(t, "detachShader", DetachShader);
        NODE_SET_PROTOTYPE_METHOD(t, "getAttachedShaders", GetAttachedShaders);
        NODE_SET_PROTOTYPE_METHOD(t, "getProgramParameter", GetProgramParameter);
        NODE_SET_PROTOTYPE_METHOD(t, "getProgramInfoLog", GetProgramInfoLog);
        NODE_SET_PROTOTYPE_METHOD(t, "getShaderParameter", GetShaderParameter);
        NODE_SET_PROTOTYPE_METHOD(t, "getShaderInfoLog", GetShaderInfoLog);
        NODE_SET_PROTOTYPE_METHOD(t, "getShaderSource", GetShaderSource);
        NODE_SET_PROTOTYPE_METHOD(t, "isProgram", IsProgram);
        NODE_SET_PROTOTYPE_METHOD(t, "isShader", IsShader);
        NODE_SET_PROTOTYPE_METHOD(t, "linkProgram", LinkProgram);
        NODE_SET_PROTOTYPE_METHOD(t, "shaderSource", ShaderSource);
        NODE_SET_PROTOTYPE_METHOD(t, "useProgram", UseProgram);
        NODE_SET_PROTOTYPE_METHOD(t, "validateProgram", ValidateProgram);
        ////Uniforms and Attributes
        //
        NODE_SET_PROTOTYPE_METHOD(t, "disableVertexAttribArray", DisableVertexAttribArray);
        NODE_SET_PROTOTYPE_METHOD(t, "enableVertexAttribArray", EnableVertexAttribArray);
        NODE_SET_PROTOTYPE_METHOD(t, "getActiveAttrib", GetActiveAttrib);
        NODE_SET_PROTOTYPE_METHOD(t, "getActiveUniform", GetActiveUniform);
        NODE_SET_PROTOTYPE_METHOD(t, "getAttribLocation", GetAttribLocation);
        NODE_SET_PROTOTYPE_METHOD(t, "getUniform", GetUniform);
        NODE_SET_PROTOTYPE_METHOD(t, "getUniformLocation", GetUniformLocation);
        NODE_SET_PROTOTYPE_METHOD(t, "getVertexAttrib", GetVertexAttrib);
        NODE_SET_PROTOTYPE_METHOD(t, "getVertexAttribOffset", GetVertexAttribOffset);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform1f", Uniform1f);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform2f", Uniform2f);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform3f", Uniform3f);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform4f", Uniform4f);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform1i", Uniform1i);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform2i", Uniform2i);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform3i", Uniform3i);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform4i", Uniform4i);

        NODE_SET_PROTOTYPE_METHOD(t, "uniform1fv", Uniform1fv);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform2fv", Uniform2fv);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform3fv", Uniform3fv);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform4fv", Uniform4fv);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform1iv", Uniform1iv);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform2iv", Uniform2iv);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform3iv", Uniform3iv);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform4iv", Uniform4iv);
        NODE_SET_PROTOTYPE_METHOD(t, "uniformMatrix2fv", UniformMatrix2fv);
        NODE_SET_PROTOTYPE_METHOD(t, "uniformMatrix3fv", UniformMatrix3fv);
        NODE_SET_PROTOTYPE_METHOD(t, "uniformMatrix4fv", UniformMatrix4fv);
        NODE_SET_PROTOTYPE_METHOD(t, "vertexAttrib1f", VertexAttrib1f);
        NODE_SET_PROTOTYPE_METHOD(t, "vertexAttrib2f", VertexAttrib2f);
        NODE_SET_PROTOTYPE_METHOD(t, "vertexAttrib3f", VertexAttrib3f);
        NODE_SET_PROTOTYPE_METHOD(t, "vertexAttrib4f", VertexAttrib4f);
        NODE_SET_PROTOTYPE_METHOD(t, "vertexAttrib1fv", VertexAttrib1fv);
        NODE_SET_PROTOTYPE_METHOD(t, "vertexAttrib2fv", VertexAttrib2fv);
        NODE_SET_PROTOTYPE_METHOD(t, "vertexAttrib3fv", VertexAttrib3fv);
        NODE_SET_PROTOTYPE_METHOD(t, "vertexAttrib4fv", VertexAttrib4fv);

        NODE_SET_PROTOTYPE_METHOD(t, "vertexAttribPointer", VertexAttribPointer);
        ////Texture Objects
        //
        NODE_SET_PROTOTYPE_METHOD(t, "activeTexture", ActiveTexture);
        NODE_SET_PROTOTYPE_METHOD(t, "bindTexture", BindTexture);
        NODE_SET_PROTOTYPE_METHOD(t, "copyTexImage2D", CopyTexImage2D);
        NODE_SET_PROTOTYPE_METHOD(t, "copyTexSubImage2D", CopyTexSubImage2D);
        NODE_SET_PROTOTYPE_METHOD(t, "createTexture", CreateTexture);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteTexture", DeleteTexture);
        NODE_SET_PROTOTYPE_METHOD(t, "generateMipmap", GenerateMipmap);
        NODE_SET_PROTOTYPE_METHOD(t, "getTexParameter", GetTexParameter);
        NODE_SET_PROTOTYPE_METHOD(t, "isTexture", IsTexture);
        NODE_SET_PROTOTYPE_METHOD(t, "texImage2D", TexImage2D);
        NODE_SET_PROTOTYPE_METHOD(t, "texParameterf", TexParameterf);
        NODE_SET_PROTOTYPE_METHOD(t, "texParameteri", TexParameteri);
        NODE_SET_PROTOTYPE_METHOD(t, "texSubImage2D", TexSubImage2D);
        ////Special Functions
        //
        NODE_SET_PROTOTYPE_METHOD(t, "disable", Disable);
        NODE_SET_PROTOTYPE_METHOD(t, "enable", Enable);
        NODE_SET_PROTOTYPE_METHOD(t, "finish", Finish);
        NODE_SET_PROTOTYPE_METHOD(t, "flush", Flush);
        NODE_SET_PROTOTYPE_METHOD(t, "getError", GetError);
        NODE_SET_PROTOTYPE_METHOD(t, "getParameter", GetParameter);
        NODE_SET_PROTOTYPE_METHOD(t, "hint", Hint);
        NODE_SET_PROTOTYPE_METHOD(t, "isEnabled", IsEnabled);
        NODE_SET_PROTOTYPE_METHOD(t, "pixelStorei", PixelStorei);
        ////Renderbuffer Objects
        //
        NODE_SET_PROTOTYPE_METHOD(t, "bindRenderbuffer", BindRenderbuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "createRenderbuffer", CreateRenderbuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteRenderbuffer", DeleteRenderbuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "getRenderbufferParameter", GetRenderbufferParameter);
        NODE_SET_PROTOTYPE_METHOD(t, "isRenderbuffer", IsRenderbuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "renderbufferStorage", RenderbufferStorage);
        ////Writing to the Draw Buffer
        //
        NODE_SET_PROTOTYPE_METHOD(t, "drawArrays", DrawArrays);
        NODE_SET_PROTOTYPE_METHOD(t, "drawElements", DrawElements);
        ////Read Back Pixels
        //
        NODE_SET_PROTOTYPE_METHOD(t, "readPixels", ReadPixels);
        ////Framebuffer Objects
        //
        NODE_SET_PROTOTYPE_METHOD(t, "bindFramebuffer", BindFramebuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "checkFramebufferStatus", CheckFramebufferStatus);
        NODE_SET_PROTOTYPE_METHOD(t, "createFramebuffer", CreateFramebuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteFramebuffer", DeleteFramebuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "framebufferRenderbuffer", FramebufferRenderbuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "isFramebuffer", IsFramebuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "framebufferTexture2D", FramebufferTexture2D);
        NODE_SET_PROTOTYPE_METHOD(t, "getFramebufferAttachmentParameter", GetFramebufferAttachmentParameter);
        constructor = Persistent<Function>::New(t->GetFunction());
    }

    Handle<Value> Renderer::Synthesize(IVideoContext* context) {
        HandleScope scope;
        const unsigned argc = 0;
        Handle<Value> argv[argc] = { };
        Local<Object> instance = constructor->NewInstance(argc, argv);
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(instance);
        obj->context = context;
        return scope.Close(instance);
    }

    Handle<Value> Renderer::New(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = new Renderer();
        obj->context = NULL; // I need to set (IVideoContext) to this variable.
        obj->Wrap(args.This());
        return args.This();
    }

    Handle<Value> Renderer::Close(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->context->Close();
        obj->context = NULL;
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::SwapBuffers(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->context->SwapBuffers();
        return scope.Close(Undefined());
    }
    ////Per-Fragment Operations
    //
    //
    Handle<Value> Renderer::BlendColor(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        double r = args[0]->NumberValue();
        double g = args[1]->NumberValue();
        double b = args[2]->NumberValue();
        double a = args[3]->NumberValue();
        glBlendColor(r,g,b,a);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::BlendEquation(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBlendEquation(args[0]->IntegerValue());
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::BlendEquationSeparate(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBlendEquationSeparate(
            args[0]->IntegerValue(),
            args[1]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::BlendFunc(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBlendFunc(
            args[0]->IntegerValue(),
            args[1]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::BlendFuncSeparate(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBlendFuncSeparate(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::DepthFunc(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDepthFunc(
            args[0]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::SampleCoverage(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glSampleCoverage(
            args[0]->NumberValue(),
            args[1]->BooleanValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::StencilFunc(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilFunc(
            args[0]->IntegerValue(),
            args[1]->Int32Value(),
            args[2]->Uint32Value()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::StencilFuncSeparate(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilFuncSeparate(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->Int32Value(),
            args[3]->Uint32Value()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::StencilOp(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilOp(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::StencilOpSeparate(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilOpSeparate(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    ////Whole Framebuffer Operations
    //
    Handle<Value> Renderer::Clear(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        uint32_t flags = args[0]->IntegerValue();
        glClear(flags);
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::ClearColor(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        double r = args[0]->NumberValue();
        double g = args[1]->NumberValue();
        double b = args[2]->NumberValue();
        double a = args[3]->NumberValue();
        glClearColor(r,g,b,a);
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::ClearDepth(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glClearDepthf(
            args[0]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::ClearStencil(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glClearStencil(
            args[0]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::ColorMask(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glColorMask(
            args[0]->BooleanValue(),
            args[1]->BooleanValue(),
            args[2]->BooleanValue(),
            args[3]->BooleanValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::DepthMask(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDepthMask(
            args[0]->BooleanValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::StencilMask(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilMask(
            args[0]->Uint32Value()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::StencilMaskSeparate(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilMaskSeparate(
            args[0]->IntegerValue(),
            args[1]->Uint32Value()
        );
        return scope.Close(Undefined());
    }
    ////Buffer Objects
    //
    Handle<Value> Renderer::BindBuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = 0;
        if (!args[1]->IsUndefined() && !args[1]->IsNull()) {
            id = GLObject::GetId(args[1]->ToObject());
        }
        glBindBuffer(args[0]->IntegerValue(), id);
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::BufferData(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLsizeiptr size;
        GLvoid* data = NULL;
        if (args[1]->IsInt32()) {
            size = args[1]->IntegerValue();
        } else {
            Local<Object> buffer_obj = args[1]->ToObject();
            data = ArrayData(buffer_obj);
            size = ArraySize(buffer_obj);
        }
        glBufferData(args[0]->IntegerValue(), size, data, args[2]->IntegerValue());
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::BufferSubData(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        Local<Object> buffer_obj = args[2]->ToObject();
        glBufferSubData(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            ArraySize(buffer_obj),
            ArrayData(buffer_obj)
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::CreateBuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id;
        glGenBuffers(1, &id);
        CatchError();
        return scope.Close(GLObject::New(id));
    }

    Handle<Value> Renderer::DeleteBuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glDeleteBuffers(1, &id);
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::GetBufferParameter(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLenum target = args[0]->IntegerValue();
        GLenum pname = args[1]->IntegerValue();
        switch (pname) {
            case GL_BUFFER_SIZE:
            case GL_BUFFER_USAGE:
            {
                GLint data;
                glGetBufferParameteriv(target, pname, &data);
                return scope.Close(Integer::New(data));
            }
        }
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::IsBuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        return scope.Close(Boolean::New(glIsBuffer(id)));
    }
    ////View and Clip
    //
    Handle<Value> Renderer::DepthRange(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDepthRangef(
            args[0]->NumberValue(),
            args[1]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Scissor(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glScissor(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Viewport(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glViewport(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    ////Rasterization
    //
    Handle<Value> Renderer::CullFace(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glCullFace(
            args[0]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::FrontFace(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glFrontFace(
            args[0]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::LineWidth(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glLineWidth(
            args[0]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::PolygonOffset(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glPolygonOffset(
            args[0]->NumberValue(),
            args[1]->NumberValue()
        );
        return scope.Close(Undefined());
    }
    ////Detect and Enable Extensions
    //
    ////Detect context lost events
    //
    ////Programs and Shaders
    //
    Handle<Value> Renderer::AttachShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLuint shader_id = GLObject::GetId(args[1]->ToObject());
        glAttachShader(id, shader_id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::BindAttribLocation(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBindAttribLocation(
            GLObject::GetId(args[0]->ToObject()),
            args[1]->IntegerValue(),
            *(String::AsciiValue)(args[2]->ToString())
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::CompileShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glCompileShader(GLObject::GetId(args[0]->ToObject()));
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::CreateProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        return scope.Close(GLObject::New(glCreateProgram()));
    }
    Handle<Value> Renderer::CreateShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        return scope.Close(GLObject::New(glCreateShader(
            args[0]->IntegerValue()
        )));
    }
    Handle<Value> Renderer::DeleteProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glDeleteProgram(id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::DeleteShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glDeleteShader(id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::DetachShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLuint shader_id = GLObject::GetId(args[1]->ToObject());
        glDetachShader(id, shader_id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetAttachedShaders(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert (false); // unimplemented, fairly odd thing to query.
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetProgramParameter(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLint res;
        glGetProgramiv(id, args[1]->IntegerValue(), &res);
        return scope.Close(Integer::New(res));
    }
    Handle<Value> Renderer::GetProgramInfoLog(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLsizei sz;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &sz);
        char log[sz];
        glGetProgramInfoLog(id, sz, NULL, log);
        return scope.Close(String::New(log));
    }
    Handle<Value> Renderer::GetShaderParameter(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLint res;
        glGetShaderiv(id, args[1]->IntegerValue(), &res);
        return scope.Close(Integer::New(res));
    }
    Handle<Value> Renderer::GetShaderInfoLog(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLsizei sz;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &sz);
        char log[sz];
        glGetShaderInfoLog(id, sz, NULL, log);
        printf("GetShaderInfoLog().4\n");
        return scope.Close(String::New(log));
    }
    Handle<Value> Renderer::GetShaderSource(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLsizei sz;
        glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &sz);
        char log[sz];
        glGetShaderSource(id, sz, NULL, log);
        return scope.Close(String::New(log));
    }
    Handle<Value> Renderer::IsProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        return scope.Close(Boolean::New(glIsProgram(id)));
    }
    Handle<Value> Renderer::IsShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        return scope.Close(Boolean::New(glIsShader(id)));
    }
    Handle<Value> Renderer::LinkProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glLinkProgram(id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::ShaderSource(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        String::AsciiValue ascii(args[1]->ToString());
        const GLint len = ascii.length();
        const GLchar* src = *ascii;
        glShaderSource(id, 1, &src, &len);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::UseProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glUseProgram(id);
        CatchError();
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::ValidateProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glValidateProgram(id);
        return scope.Close(Undefined());
    }
    ////Uniforms and Attributes
    //
    Handle<Value> Renderer::DisableVertexAttribArray(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDisableVertexAttribArray(args[0]->IntegerValue());
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::EnableVertexAttribArray(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glEnableVertexAttribArray(args[0]->IntegerValue());
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetActiveAttrib(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false); // I'm not sure how to implement this yet.
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetActiveUniform(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false); // I'm not sure how to implement this yet.
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetAttribLocation(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        String::AsciiValue ascii(args[1]->ToString());
        GLuint loc = glGetAttribLocation(id, *ascii);
        return scope.Close(Integer::New(loc));
    }
    Handle<Value> Renderer::GetUniform(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false); // I'm not sure how to implement this yet.
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetUniformLocation(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        String::AsciiValue ascii(args[1]->ToString());
        GLuint loc = glGetUniformLocation(id, *ascii);
        return scope.Close(Integer::New(loc));
    }
    Handle<Value> Renderer::GetVertexAttrib(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(0);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetVertexAttribOffset(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        void* offset;
        glGetVertexAttribPointerv(
            args[0]->Uint32Value(),
            args[1]->IntegerValue(),
            &offset
        );
        return scope.Close(Integer::New((long)offset));
    }
    Handle<Value> Renderer::Uniform1f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform1f(
            args[0]->IntegerValue(),
            args[1]->NumberValue()
        );
        CatchError();
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform2f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform2f(
            args[0]->IntegerValue(),
            args[1]->NumberValue(),
            args[2]->NumberValue()
        );
        CatchError();
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform3f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform3f(
            args[0]->IntegerValue(),
            args[1]->NumberValue(),
            args[2]->NumberValue(),
            args[3]->NumberValue()
        );
        CatchError();
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform4f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform4f(
            args[0]->IntegerValue(),
            args[1]->NumberValue(),
            args[2]->NumberValue(),
            args[3]->NumberValue(),
            args[4]->NumberValue()
        );
        CatchError();
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform1i(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform1i(
            args[0]->IntegerValue(),
            args[1]->IntegerValue()
        );
        CatchError();
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform2i(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform2i(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue()
        );
        CatchError();
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform3i(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform3i(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue()
        );
        CatchError();
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform4i(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform4i(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue(),
            args[4]->IntegerValue()
        );
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Uniform1fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 1 * sizeof(GLfloat);
        Local<Object> buffer_obj = args[1]->ToObject();
        glUniform1fv(args[0]->IntegerValue(), ArraySize(buffer_obj)/n, (GLfloat*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Uniform2fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 2 * sizeof(GLfloat);
        Local<Object> buffer_obj = args[1]->ToObject();
        glUniform2fv(args[0]->IntegerValue(), ArraySize(buffer_obj)/n, (GLfloat*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Uniform3fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 3 * sizeof(GLfloat);
        Local<Object> buffer_obj = args[1]->ToObject();
        glUniform3fv(args[0]->IntegerValue(), ArraySize(buffer_obj)/n, (GLfloat*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Uniform4fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 4 * sizeof(GLfloat);
        Local<Object> buffer_obj = args[1]->ToObject();
        glUniform4fv(args[0]->IntegerValue(), ArraySize(buffer_obj)/n, (GLfloat*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Uniform1iv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 1 * sizeof(GLint);
        Local<Object> buffer_obj = args[1]->ToObject();
        glUniform1iv(args[0]->IntegerValue(), ArraySize(buffer_obj)/n, (GLint*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Uniform2iv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 2 * sizeof(GLint);
        Local<Object> buffer_obj = args[1]->ToObject();
        glUniform2iv(args[0]->IntegerValue(), ArraySize(buffer_obj)/n, (GLint*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Uniform3iv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 3 * sizeof(GLint);
        Local<Object> buffer_obj = args[1]->ToObject();
        glUniform3iv(args[0]->IntegerValue(), ArraySize(buffer_obj)/n, (GLint*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Uniform4iv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 4 * sizeof(GLint);
        Local<Object> buffer_obj = args[1]->ToObject();
        glUniform4iv(args[0]->IntegerValue(), ArraySize(buffer_obj)/n, (GLint*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::UniformMatrix2fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 2 * sizeof(GLfloat);
        Local<Object> buffer_obj = args[2]->ToObject();
        glUniformMatrix2fv(
            args[0]->IntegerValue(),
            ArraySize(buffer_obj) / (n*n),
            args[1]->BooleanValue()?GL_TRUE:GL_FALSE,
            (GLfloat*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::UniformMatrix3fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 3 * sizeof(GLfloat);
        Local<Object> buffer_obj = args[2]->ToObject();
        glUniformMatrix3fv(
            args[0]->IntegerValue(),
            ArraySize(buffer_obj) / (n*n),
            args[1]->BooleanValue()?GL_TRUE:GL_FALSE,
            (GLfloat*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::UniformMatrix4fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        const int n = 4 * sizeof(GLfloat);
        Local<Object> buffer_obj = args[2]->ToObject();
        glUniformMatrix4fv(
            args[0]->IntegerValue(),
            ArraySize(buffer_obj) / (n*n),
            args[1]->BooleanValue()?GL_TRUE:GL_FALSE,
            (GLfloat*)ArrayData(buffer_obj));
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::VertexAttrib1f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glVertexAttrib1f(
            args[0]->IntegerValue(),
            args[1]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::VertexAttrib2f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glVertexAttrib2f(
            args[0]->IntegerValue(),
            args[1]->NumberValue(),
            args[2]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::VertexAttrib3f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glVertexAttrib3f(
            args[0]->IntegerValue(),
            args[1]->NumberValue(),
            args[2]->NumberValue(),
            args[3]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::VertexAttrib4f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glVertexAttrib4f(
            args[0]->IntegerValue(),
            args[1]->NumberValue(),
            args[2]->NumberValue(),
            args[3]->NumberValue(),
            args[4]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::VertexAttrib1fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        Local<Object> buffer_obj = args[2]->ToObject();
        glVertexAttrib1fv(
            args[0]->IntegerValue(),
            (GLfloat*)ArrayData(buffer_obj));
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::VertexAttrib2fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        Local<Object> buffer_obj = args[2]->ToObject();
        glVertexAttrib2fv(
            args[0]->IntegerValue(),
            (GLfloat*)ArrayData(buffer_obj));
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::VertexAttrib3fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        Local<Object> buffer_obj = args[2]->ToObject();
        glVertexAttrib3fv(
            args[0]->IntegerValue(),
            (GLfloat*)ArrayData(buffer_obj));
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::VertexAttrib4fv(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        Local<Object> buffer_obj = args[2]->ToObject();
        glVertexAttrib4fv(
            args[0]->IntegerValue(),
            (GLfloat*)ArrayData(buffer_obj));
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::VertexAttribPointer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint    indx       = args[0]->IntegerValue();
        GLint     size       = args[1]->IntegerValue();
        GLenum    type       = args[2]->IntegerValue();
        GLboolean normalized = (args[3]->BooleanValue())?GL_TRUE:GL_FALSE;
        GLsizei   stride     = args[4]->IntegerValue();
        GLvoid*   offset     = (GLvoid*)args[5]->IntegerValue();
        glVertexAttribPointer(indx, size, type, normalized, stride, offset);
        CatchError();
        return scope.Close(Undefined());
    }
    ////Texture Objects
    //
    Handle<Value> Renderer::ActiveTexture(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glActiveTexture(args[0]->IntegerValue());
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::BindTexture(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[1]->ToObject());
        glBindTexture(args[0]->IntegerValue(), id);
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::CopyTexImage2D(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glCopyTexImage2D(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::CopyTexSubImage2D(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glCopyTexSubImage2D(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::CreateTexture(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id;
        glGenTextures(1, &id);
        return scope.Close(GLObject::New(id));
    }

    Handle<Value> Renderer::DeleteTexture(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glDeleteTextures(1, &id);
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::GenerateMipmap(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glGenerateMipmap(args[0]->IntegerValue());
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::GetTexParameter(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glGetTexParameter(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::IsTexture(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        GLuint id = GLObject::GetId(args[0]->ToObject());
        return scope.Close(Boolean::New(glIsTexture(id)));
    }

    Handle<Value> Renderer::TexImage2D(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        if (args.Length() == 2) {
            Local<Object> buffer_obj = args[1]->ToObject();
            void *buffer_data = node::Buffer::Data(buffer_obj);
            size_t buffer_length = node::Buffer::Length(buffer_obj);
            assert(buffer_length == sizeof(int) * 5);
            obj->context->GlobalImageTexture2D(
                args[0]->IntegerValue(), /* target */
                buffer_data
            );
        } else {
            Local<Object> buffer_obj = args[8]->ToObject();
            glTexImage2D(
                args[0]->IntegerValue(), /* target */
                args[1]->IntegerValue(), /* level */
                args[2]->IntegerValue(), /* internalformat */
                args[3]->IntegerValue(), /* width */
                args[4]->IntegerValue(), /* height */
                args[5]->IntegerValue(), /* border */
                args[6]->IntegerValue(), /* format */
                args[7]->IntegerValue(), /* type */
                ArrayData(buffer_obj) /* pixels */
            );
        }
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::TexParameterf(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glTexParameterf(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::TexParameteri(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glTexParameteri(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::TexSubImage2D(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        Local<Object> buffer_obj = args[8]->ToObject();
        glTexSubImage2D(
            args[0]->IntegerValue(), /* target */
            args[1]->IntegerValue(), /* level */
            args[2]->IntegerValue(), /* xoffset */
            args[3]->IntegerValue(), /* yoffset */
            args[4]->IntegerValue(), /* width */
            args[5]->IntegerValue(), /* height */
            args[6]->IntegerValue(), /* format */
            args[7]->IntegerValue(), /* type */
            ArrayData(buffer_obj) /* pixels */
        );
        CatchError();
        return scope.Close(Undefined());
    }
    ////Special Functions
    //
    Handle<Value> Renderer::Disable(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDisable(args[0]->IntegerValue());
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Enable(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glEnable(args[0]->IntegerValue());
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Finish(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glFinish();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Flush(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glFlush();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::GetError(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLint e = glGetError();
        return scope.Close(Integer::New(e));
    }

    Handle<Value> Renderer::GetParameter(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glGetParameter(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Hint(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glHint(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::IsEnabled(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        return scope.Close(Boolean::New(glIsEnabled(args[0]->IntegerValue())));
    }

    Handle<Value> Renderer::PixelStorei(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glPixelStorei(
        );*/
        return scope.Close(Undefined());
    }

    ////Renderbuffer Objects
    // 
    Handle<Value> Renderer::BindRenderbuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glBindRenderbuffer(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::CreateRenderbuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id;
        glGenRenderbuffers(1, &id);
        return scope.Close(GLObject::New(id));
    }

    Handle<Value> Renderer::DeleteRenderbuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glDeleteRenderbuffers(1, &id);
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::GetRenderbufferParameter(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glGetRenderbufferParameter(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::IsRenderbuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        return scope.Close(Boolean::New(glIsRenderbuffer(id)));
    }

    Handle<Value> Renderer::RenderbufferStorage(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glRenderbufferStorage(
        );*/
        return scope.Close(Undefined());
    }
    ////Writing to the Draw Buffer
    //
    Handle<Value> Renderer::DrawArrays(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDrawArrays(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue()
        );
        CatchError();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::DrawElements(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDrawElements(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            (void*)args[3]->IntegerValue()
        );
        CatchError();
        return scope.Close(Undefined());
    }
    ////Read Back Pixels
    //
    Handle<Value> Renderer::ReadPixels(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glReadPixels(
        );*/
        return scope.Close(Undefined());
    }
    ////Framebuffer Objects
    //
    Handle<Value> Renderer::BindFramebuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glBindFramebuffer(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::CheckFramebufferStatus(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glCheckFramebufferStatus(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::CreateFramebuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glCreateFramebuffer(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::DeleteFramebuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glDeleteFramebuffer(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::FramebufferRenderbuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glFramebufferRenderbuffer(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::IsFramebuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        return scope.Close(Boolean::New(glIsFramebuffer(id)));
    }

    Handle<Value> Renderer::FramebufferTexture2D(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glFramebufferTexture2D(
        );*/
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::GetFramebufferAttachmentParameter(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false);
        /*glGetFramebufferAttachmentParameter(
        );*/
        return scope.Close(Undefined());
    }

    static void ThrowError(GLenum error) {
        const char* name = NULL;
        switch (error) {
            case GL_INVALID_ENUM: name = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE: name = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: name = "INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: name = "INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY: name = "OUT_OF_MEMORY"; break;
        }
        if (name) {
            ThrowException(String::New(name));
        } else {
            ThrowException(Integer::New(error));
        }
    }

    inline void Renderer::CatchError() {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            ThrowError(error);
        }
    }

    size_t Renderer::Sizeof(Local<Object> object) {
        ExternalArrayType type = object->GetIndexedPropertiesExternalArrayDataType();
        switch (type) {
            case kExternalByteArray:
            case kExternalUnsignedByteArray:
                return 1;
            case kExternalShortArray:
            case kExternalUnsignedShortArray:
                return 2;
            case kExternalIntArray:
            case kExternalUnsignedIntArray:
            case kExternalFloatArray:
                return 4;
            case kExternalDoubleArray:
                return 8;
            case kExternalPixelArray:
                ThrowException(Exception::TypeError(String::New("Sizeof(PixelArray) unimplemented")));
        }
        ThrowException(Exception::TypeError(String::New("unknown ExternalArrayType")));
    }
}
